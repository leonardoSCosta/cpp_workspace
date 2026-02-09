#include "Eigen/Dense"
#include "Eigen/src/Core/ArithmeticSequence.h"
#include "Eigen/src/Core/Matrix.h"

#include <chrono>
#include <iostream>
#include <math.h>

using Eigen::MatrixXd;
using Eigen::seq;

#define STEP 200
struct ProblemData
{
    int xMin;
    int xMax;
    int yMin;
    int yMax;
    int step;
    int xStep;
    int yStep;
    int minXSolution;
} constexpr ProblemData = {.xMin = -1000, .xMax = 1000, .yMin = -4500,
                           .yMax = 4500, .step = STEP, .xStep = 2000 / STEP + 1,
                           .yStep = 9000 / STEP + 1, .minXSolution = -1000};
#undef STEP

#define X (0)
#define Y (1)

int rowToY(int _row)
{
    return _row * ProblemData.step + ProblemData.yMin;
}

int colToX(int _col)
{
    return _col * ProblemData.step + ProblemData.xMin;
}

int yToRow(int _y)
{
    return fmin(fmax((_y - ProblemData.yMin) / ProblemData.step, 0), ProblemData.yStep);
}

int xToCol(int _x)
{
    return fmin(fmax((_x - ProblemData.xMin) / ProblemData.step, 0), ProblemData.xStep);
}

void fillAround(Eigen::Matrix<float, ProblemData.yStep, ProblemData.xStep> &_costMatrix,
                Eigen::Matrix<int, 2, 1> &_pos, int _range, int _cost)
{
    int row = yToRow(_pos(Y)), col = xToCol(_pos(X));

    for (int y = fmax(row - _range, 0); y <= fmin(row + _range, ProblemData.yStep-1); y++)
    {
        for (int x = fmax(col - _range, 0); x <= fmin(col + _range, ProblemData.xStep-1); x++)
        {
            _costMatrix(y, x) += _cost;
        }
    }
}

void fillShootDirection(Eigen::Matrix<float, ProblemData.yStep, ProblemData.xStep> &_costMatrix,
                        const Eigen::Matrix<float, 2, 1> &_dir,
                        Eigen::Matrix<int, 2, 1> _pos, int _cost)
{
    Eigen::Matrix<float, 2, 1> Pshoot = _pos.cast<float>();
    Eigen::Matrix<int, 2, 1> shootI;

    while (yToRow(Pshoot(Y)) >= 0 && yToRow(Pshoot(Y)) <= ProblemData.yStep - 1 &&
           xToCol(Pshoot(X)) >= 0 && xToCol(Pshoot(X)) <= ProblemData.xStep - 1)
    {
        // _costMatrix(yToRow(Pshoot(Y)), xToCol(Pshoot(X))) += _cost;
        shootI = Pshoot.cast<int>();
        fillAround(_costMatrix, shootI, 1, _cost);
        Pshoot = (Pshoot + _dir);

    }
}

int main(int argc, char *argv[]) 
{ 
    int samples = 10000;
    double totalTime = 0;

    const int nOpponents = 5, nDiscretization = 1, oppActionRadius = 250, oppARSq = oppActionRadius * oppActionRadius;
    Eigen::Matrix<int, 2, 1> Popt, Pat, Pop;
    Eigen::Matrix<float, 2, 1> shootDir;
    Eigen::Matrix<int, 2, nOpponents> Cop, Cpred;
    Eigen::Matrix<int, 2, nOpponents * nDiscretization> Rpred;
    Eigen::Matrix<float, ProblemData.yStep, ProblemData.xStep> fieldCost;

    // clang-format off
    Cop <<  -230,   360, -100, -290, -1100,
           -1430, -1830,  490, -810, -2100;
    Cpred = Cop;
    Rpred = Cop;

    Pat << -2045,
           -2125;
    // clang-format on

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = samples; i > 0; i--)
    {
        fieldCost.setZero();

        for (int row = 0; row < ProblemData.yStep; row++)
        {
            for (int col = 0; col < ProblemData.xStep; col++)
            {
                Popt << colToX(col), rowToY(row);
                fieldCost(row, col) = (Popt - Pat).norm();

                if (colToX(col) < ProblemData.minXSolution)
                {
                    fieldCost(row, col) += 10000;
                }
            }
        }

        for (int n = 0; n < nOpponents; n++)
        {
            Pop = Cop(seq(0, 1), n);
            fillAround(fieldCost, Pop, oppActionRadius / ProblemData.step, 8000);

            shootDir = ((Pop - Pat).cast<float>().normalized()) * ProblemData.step;
            fillShootDirection(fieldCost, shootDir, Pat, 4000);
        }
        int dum, mmy;
        fieldCost.minCoeff(&dum, &mmy);

    }
    auto end = std::chrono::high_resolution_clock::now();
    totalTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-6;

    std::cout << "Cost\n";
    std::cout << fieldCost << "\n";
    std::cout << "Optimization Complete!\n";
    int row, col;
    std::cout << "min FieldCost = " << fieldCost.minCoeff(&row, &col) << "\n";
    std::cout << "col, row = " << col << ", " << row << "\n";
    std::cout << "X, Y = " << colToX(col) << ", " << rowToY(row)<< "\n";

    auto temp = fieldCost.colwise().reverse();
    for (int row = 0; row < ProblemData.yStep; row++)
    {
        for (int col = 0; col < ProblemData.xStep; col++)
        {
             std::cout << int(temp(row, col)) << ",";
        }
         std::cout << "\n";
    }

    std::cout << "\n== Average Time: " << totalTime / samples << " ms\t Frequency " << 1e3/(totalTime/samples) << " Hz\n";

    return 0; 
}
