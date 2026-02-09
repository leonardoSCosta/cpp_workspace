#ifndef PrSBC_H
#define PrSBC_H

#include <cmath>
#include <complex>
#include <iostream>
#include <chrono>

#include "Eigen/Dense"
#include "Eigen/Sparse"
#include "Eigen/src/Core/Matrix.h"
#include "Eigen/src/SparseCore/SparseMatrix.h"
#include "Eigen/src/Core/ArithmeticSequence.h"
// #include "src/PrSBC/src/osqp/include/OsqpEigen/OsqpEigen.h"

// For cmake
#include "OsqpEigen/OsqpEigen.h"

using Eigen::MatrixXd;
using Eigen::seq;

class PrSBC
{
    bool configured;
    float gamma;
    float safetyRadius;
    float confidence;
    int N;

    typedef struct TrapezoidalCDF
    {
        double b2;
        double b1;
    } TrapezoidalCDF;

    long nchoosek(const int n, const int k)
    {
        long prevSolution = n - k + 1, solution;

        for(int i = 1; i < k; ++i)
        {
            solution = prevSolution * (n - k + 1 + i) / (i + 1);
            prevSolution = solution;
        }

        return prevSolution;
    }

    /**
     * @brief This function constructs the trap distribution resultant from
     * convolution of two different central uniform distribution.
     *
     * i.e. from measurements of two robots positions
     * bot 1 : uniformly distributed between [-a,a]
     * bot 2 : uniformly distributed between [-c,c]
     * sigma: requred confidence level (>50%)
     *
     * @param a should be positive
     * @param c should be positive
     * @param delta x_bot1 - x_bot2 error between the two noisy measurements
     *
     * @return when sigma >.5
     *         b2: <b1 whose CDF corresponds to 1-sigma
     *         b1: >b2 whose CDF corresponds to sigma
     *         when sigma < .5
     *         b2: >b1 whose CDF corresponds to 1-sigma
     *         b1: <b2 whose CDF corresponds to sigma

     */
    TrapezoidalCDF trapezoidalCDFInverse(double a, double c, double delta)
    {
        TrapezoidalCDF result;
        double sigma = confidence;
        double b1, b2;
        double A, C;

        // [-A A] is the large one, and [-C C] is the smaller one
        if(a > c)
        {
            A = a;
            C = c;
        }
        else
        {
            A = c;
            C = a;
        }

        if(A == 0 && C == 0)
        {
            result.b2 = delta;
            result.b1 = delta;
            return result;
        }

        // O_vec = [-(A+C) -(A-C) (A-C) (A+C)]; // vector of vertices on the trap distribution pdf
        double h = 1.0 / (2.0 * A); // height of the trap distribution
        Eigen::Matrix<double, 1, 3> area_seq;
        area_seq << 1.0/2 * 2.0 * C * h, 2.0 * (A - C) * h, 1.0/2 * 2.0 * C * h;

        Eigen::Matrix<double, 1, 2> area_vec;
        area_vec << area_seq(0, 0), area_seq(0, 1) + area_seq(0, 2);

        if(abs(A - C) < 1e-5) // then is triangle
        {
            // assuming sigma > 50%
            // 1-area_vec(0, 1) should be very close to 0.5
            b1 = (A + C) - 2 * C * sqrt((1.0 - sigma) / (1.0 - area_vec(0, 1)));
            b2 = -b1;

            b1 = b1 + delta;
            b2 = b2 + delta; // apply shift here due to xi-xj
        }
        else // is trapezoidal
        {
            if(sigma > area_vec(0, 1)) // right triangle area
            {
                b1 =  (A + C) - 2 * C * sqrt((1.0 - sigma) / (1.0 - area_vec(0, 1)));
                b2 = -(A + C) + 2 * C * sqrt((1.0 - sigma) / (1.0 - area_vec(0, 1)));

                b1 = b1 + delta;
                b2 = b2 + delta; // apply shift here due to xi-xj
            }
            else if(sigma > area_vec(0, 0) && sigma <= area_vec(0, 1)) // in between the triangle part
            {
                b1 = -(A - C) + (sigma - area_vec(0, 0)) / h; // assuming >50%, then b1 should >0
                b2 = -b1;

                b1 = b1 + delta;
                b2 = b2 + delta; // apply shift here due to xi-xj
                // note that b1 could be > or < b2, depending on whether sigma > or < .5
            }
            else if(sigma <= area_vec(0, 0))
            {
                b1 = -(A + C) + 2 * C * sqrt(sigma / area_vec(0, 0)); // assuming >50%, then b1 should >0
                b2 = -b1;

                b1 = b1 + delta;
                b2 = b2 + delta; // apply shift here due to xi-xj
            }
            else // first triangle, which is not allowed as long as we assume sigma > 50%
            {
                std::cout << "bug in trap_cdf_inv, what is wrong?\n";
            }
        }

        result.b1 = b1;
        result.b2 = b2;
        return result;
    }

    void fillRobots(const MatrixXd &_x, const MatrixXd &_uNoiseSpan,
                    const MatrixXd &_xNoiseSpan, MatrixXd &A, MatrixXd &b, int &count)
    {
        for(int i = 0; i < N - 1; ++i)
        {
            for(int j = i+1; j < N; ++j)
            {
                MatrixXd maxDVij = (_uNoiseSpan.col(i) + _uNoiseSpan.col(j)).rowwise().norm();
                MatrixXd maxDXij = (_x.col(i) - _x.col(j)).rowwise().norm() +
                    (_xNoiseSpan.col(i) + _xNoiseSpan.col(j)).rowwise().norm();

                Eigen::Matrix<double, 2, 1> safetyMatrix = MatrixXd::Ones(2, 1);
                safetyMatrix *= pow(safetyRadius, 2);

//                 MatrixXd BB =  -safetyMatrix - 2/gamma * maxDVij.cwiseProduct(maxDXij);

                TrapezoidalCDF bX = trapezoidalCDFInverse(_xNoiseSpan(0, i),
                                                          _xNoiseSpan(0, j),
                                                          _x(0, i) - _x(0, j)),
                               bY = trapezoidalCDFInverse(_xNoiseSpan(1, i),
                                                          _xNoiseSpan(1, j),
                                                          _x(1, i) - _x(1, j));

                double bx, by;
                if((bX.b2 < 0 && bX.b1 > 0) || (bX.b2 > 0 && bX.b1 < 0))
                {
                    // distance between robots on x smaller than error bound!
                    bx = 0;
                }
                else if((bX.b1 < 0 && bX.b2 < bX.b1) || (bX.b2 < 0 && bX.b2 > bX.b1))
                {
                    bx = bX.b1;
                }
                else if((bX.b2 > 0 && bX.b2 < bX.b1) || (bX.b1 > 0 && bX.b2 > bX.b1))
                {
                    bx = bX.b2;
                }
                else
                {
                    // no uncertainty or sigma = 0.5 on x
                    bx = bX.b1;
                }

                if((bY.b2 < 0 && bY.b1 > 0) || (bY.b2 > 0 && bY.b1 < 0))
                {
                    // distance between robots on y smaller than error bound!
                    by = 0;
                }
                else if((bY.b1 < 0 && bY.b2 < bY.b1) || (bY.b2 < 0 && bY.b2 > bY.b1))
                {
                    by = bY.b1;
                }
                else if((bY.b2 > 0 && bY.b2 < bY.b1) || (bY.b1 > 0 && bY.b2 > bY.b1))
                {
                    by = bY.b2;
                }
                else
                {
                    // no uncertainty or sigma = 0.5 on y
                    by = bY.b1;
                }

                Eigen::Matrix<double, 1, 2> bxy;
                bxy << bx, by;
                A(count, seq(2*i, 2*i+1)) = -2 * bxy.transpose();
                A(count, seq(2*j, 2*j+1)) =  2 * bxy.transpose();

                Eigen::Matrix<double, 2, 1> absB = bxy.cwiseAbs();
                MatrixXd absMaxDVij = maxDVij.cwiseAbs(),
                         absMaxDXij = maxDXij.cwiseAbs();
                safetyMatrix.setOnes();
                safetyMatrix *= safetyRadius;

                MatrixXd h = absB.cwiseProduct(absB) - safetyMatrix.cwiseProduct(safetyMatrix)
                    - 2 * absMaxDVij.cwiseProduct(absMaxDXij) / gamma;

                b(count) = gamma * pow(h.sum(), 3);
                count++;
            }
        }
    }

    void fillObstacles(const MatrixXd &_x, const MatrixXd &_obstacles,
                       const MatrixXd &_uNoiseSpan, const MatrixXd &_xNoiseSpan,
                       MatrixXd &A, MatrixXd &b, int &count)
    {
        if(_obstacles.cols() > 0 && _obstacles.rows() > 0)
        {
            for(int i = 0; i < N; ++i)
            {
                for(int j = 0; j < _obstacles.cols(); ++j)
                {
                    MatrixXd maxDVij = (_uNoiseSpan.col(i) + _uNoiseSpan.col(j)).rowwise().norm();
                    MatrixXd maxDXij = (_x.col(i) - _obstacles.col(j)).rowwise().norm() +
                        (_xNoiseSpan.col(i) + _xNoiseSpan.col(j)).rowwise().norm();

                    Eigen::Matrix<double, 2, 1> safetyMatrix = MatrixXd::Ones(2, 1);
                    safetyMatrix *= pow(safetyRadius, 2);

                    //                 MatrixXd BB =  -safetyMatrix - 2/gamma * maxDVij.cwiseProduct(maxDXij);

                    TrapezoidalCDF bX = trapezoidalCDFInverse(_xNoiseSpan(0, i),
                            _xNoiseSpan(0, j),
                            _x(0, i) - _obstacles(0, j)),
                                   bY = trapezoidalCDFInverse(_xNoiseSpan(1, i),
                                           _xNoiseSpan(1, j),
                                           _x(1, i) - _obstacles(1, j));

                    double bx, by;
                    if((bX.b2 < 0 && bX.b1 > 0) || (bX.b2 > 0 && bX.b1 < 0))
                    {
                        // distance between robots on x smaller than error bound!
                        bx = 0;
                    }
                    else if((bX.b1 < 0 && bX.b2 < bX.b1) || (bX.b2 < 0 && bX.b2 > bX.b1))
                    {
                        bx = bX.b1;
                    }
                    else if((bX.b2 > 0 && bX.b2 < bX.b1) || (bX.b1 > 0 && bX.b2 > bX.b1))
                    {
                        bx = bX.b2;
                    }
                    else
                    {
                        // no uncertainty or sigma = 0.5 on x
                        bx = bX.b1;
                    }

                    if((bY.b2 < 0 && bY.b1 > 0) || (bY.b2 > 0 && bY.b1 < 0))
                    {
                        // distance between robots on y smaller than error bound!
                        by = 0;
                    }
                    else if((bY.b1 < 0 && bY.b2 < bY.b1) || (bY.b2 < 0 && bY.b2 > bY.b1))
                    {
                        by = bY.b1;
                    }
                    else if((bY.b2 > 0 && bY.b2 < bY.b1) || (bY.b1 > 0 && bY.b2 > bY.b1))
                    {
                        by = bY.b2;
                    }
                    else
                    {
                        // no uncertainty or sigma = 0.5 on y
                        by = bY.b1;
                    }

                    Eigen::Matrix<double, 1, 2> bxy;
                    bxy << bx, by;
                    A(count, seq(2*i, 2*i+1)) = -2 * bxy.transpose();

                    Eigen::Matrix<double, 2, 1> absB = bxy.cwiseAbs();
                    MatrixXd absMaxDVij = maxDVij.cwiseAbs(),
                             absMaxDXij = maxDXij.cwiseAbs();
                    safetyMatrix.setOnes();
                    safetyMatrix *= safetyRadius;

                    //                 double h = (_x.col(i) - _obstacles.col(j)).squaredNorm() - safetyRadius * safetyRadius;
                    MatrixXd h = absB.cwiseProduct(absB) - safetyMatrix.cwiseProduct(safetyMatrix)
                        - 2 * absMaxDVij.cwiseProduct(absMaxDXij) / gamma;
                    b(count) = gamma * pow(h.sum(), 9);
                    count++;
                }
            }
        }
    }

    void setAmplitudeConstraint(MatrixXd& A, MatrixXd& b, MatrixXd& l,
                                float _maxSpeed)
    {
        A(seq(A.rows()-2*N, A.rows()-1), seq(0, A.cols()-1)) = MatrixXd::Identity(2*N, 2*N);

        b(seq(b.rows()-2*N, b.rows()-1), 0) = MatrixXd::Ones(2*N, 1) * _maxSpeed;

        l.fill(-OsqpEigen::INFTY);
        l(seq(b.rows()-2*N, b.rows()-1), 0) = -MatrixXd::Ones(2*N, 1) * _maxSpeed;
            
        std::cout << "Matriz A:\n" << A << "\n";
        std::cout << "Matriz b (upper bound):\n" << b << "\n";
        std::cout << "Matriz l (lower bound):\n" << l << "\n";
    }

    MatrixXd solveQP(int _nConstraints, const Eigen::SparseMatrix<double> &_A,
                     const Eigen::SparseMatrix<double> &_H, MatrixXd &_f,
                     MatrixXd &_l, MatrixXd &_b)
    {
        OsqpEigen::Solver solver;
        // settings
        solver.settings()->setVerbosity(true);
        solver.settings()->setWarmStart(true);
        solver.settings()->setPrimalInfeasibilityTolerance(1e-15);

        // configuring the QP
        solver.data()->setNumberOfVariables(N * 2);
        solver.data()->setNumberOfConstraints(_nConstraints);
        if(!solver.data()->setHessianMatrix(_H))
        {
            exit(1);
        }
        if(!solver.data()->setGradient(_f))
        {
            exit(1);
        }
        if(!solver.data()->setLinearConstraintsMatrix(_A))
        {
            exit(1);
        }
        if(!solver.data()->setUpperBound(_b))
        {
            exit(1);
        }

        if(!solver.data()->setLowerBound(_l))
        {
            exit(1);
        }
        // instantiate the solver
        if(!solver.initSolver())
        {
            exit(1);
        }

        // solve the QP problem
        if(solver.solveProblem() != OsqpEigen::ErrorExitFlag::NoError)
        {
            exit(1);
        }

        MatrixXd solution = solver.getSolution().reshaped(2, N);
        std::cout << "Solver status: " << int(solver.getStatus()) << '\n';
        return solution;
    }

public:
    PrSBC()
    {
        std::cout.precision(5);
        configured = false;
        gamma = safetyRadius = confidence = 0;
    }

    ~PrSBC()
    {
        configured = false;
    }

    void config(float _barrierGain = 1e4, float _safetyRadius = 0.1,
                float _confidence = 1, int _robots = 6)
    {
        configured = true;
        gamma = _barrierGain;
        safetyRadius = _safetyRadius * 2;
        confidence = _confidence;
        N = _robots;
    }

    /**
     * @brief
     *
     * @param _u
     * @param _states - [2xN] Estados dos robôs. Somente x e y, sem posição angular.
     * @param _obstacles - [2xN] Estados dos obstáculos. Somente x e y, sem posição angular.
     * @param _xNoiseSpan
     * @param _uNoiseSpan
     *
     * @return
     */
    MatrixXd getSafeInput(MatrixXd _u, MatrixXd _x, MatrixXd _obstacles,
                          MatrixXd _xNoiseSpan, MatrixXd _uNoiseSpan)
    {
        if(!configured)
        {
            MatrixXd none;
            std::cout << "Trying to use without configuring!\n";
            return none;
        }

        int nConstraints = N - 1;
        if(N > 2)
        {
            nConstraints = nchoosek(N, 2);
        }
        nConstraints += _obstacles.cols() * N;
        nConstraints += 2 * N; // amplitude constraint

        MatrixXd A = MatrixXd::Zero(nConstraints, 2*N),
                 b = MatrixXd::Zero(nConstraints, 1),
                 l = MatrixXd::Ones(nConstraints, 1);

        int count = 0;
        fillRobots(_x, _uNoiseSpan, _xNoiseSpan, A, b, count);
        fillObstacles(_x, _obstacles, _uNoiseSpan, _xNoiseSpan, A, b, count);
        setAmplitudeConstraint(A, b, l, 0.26);

        MatrixXd vhat = _u.reshaped(2*N, 1),
                 f = -2*vhat,
                 H = 2 * MatrixXd::Identity(2*N, 2*N);

        if(b.minCoeff() < 0)
        {
            std::cout << "\nu:\n" << _u << "\n";
            std::cout << "\nx:\n" << _x << "\n";
            std::cout << "\nx noise:\n" << _xNoiseSpan << "\n";
            std::cout << "\nu noise:\n" << _uNoiseSpan << "\n";
            std::cout << "\nA:\n" << A << "\n";
            std::cout << "\nb:\n" << b << "\n";
        }
        MatrixXd uSafe = solveQP(nConstraints, A.sparseView(), H.sparseView(), f, l, b);
        std::cout << "\nH:\n" << H << "\n";
        std::cout << "\nf:\n" << f << "\n";
        std::cout << "\nA:\n" << A << "\n";
        std::cout << "\nb:\n" << b << "\n";
        std::cout << "\nSolution:\n" << uSafe << "\n";

        return uSafe;
    }
};


#endif // PrSBC_H
