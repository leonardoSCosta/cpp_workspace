#include "prsbc.h"

int main() {
  PrSBC prsbc;
  const int nRobots = 1;
  prsbc.config(1e4, 0.2, 0.90, nRobots);

  Eigen::Matrix<double, 2, nRobots> u, uSafe;
  u << 1, 0;

  Eigen::Matrix<double, 2, nRobots> x;
  x << 7.9783, 4.8177;

  Eigen::Matrix<double, 2, 10> obstacles;
  obstacles << 6.4336,6.7335,7.0335,7.0376,7.9334,7.9375,8.2293,8.5293,8.8293,9.1293,
               5.5134,5.5094,5.5053,5.8053,5.4932,5.7931,5.1891,5.1851,5.181,5.177;


  Eigen::Matrix<double, 2, 10> stateNoise;
  stateNoise << 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0;

  Eigen::Matrix<double, 2, 10> inputNoise;
  inputNoise << 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0;

  double totalTime = 0;
  int samples = 1;
  for (int i = 0; i < samples; ++i) {
    auto start = std::chrono::high_resolution_clock::now();

    uSafe = prsbc.getSafeInput(u, x, obstacles, stateNoise, inputNoise);

    auto end = std::chrono::high_resolution_clock::now();
    totalTime +=
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
            .count() *
        1e-6;
  }
  std::cout << "\n== Average Time: " << totalTime / samples
            << " ms\t Frequency " << 1 / (totalTime / samples / 1e3) <<
            "Hz\n";
}

// int main(int argc, char *argv[]) {
//   Eigen::MatrixXd H, f, l, A, b;
//   H.resize(2, 2);
//   H << 2, 0, 0, 2;
//
//   f.resize(2, 1);
//   f << -2, 0;
//
//   l.resize(12, 1);
//   l << -1e+30, -1e+30, -1e+30, -1e+30, -1e+30, -1e+30, -1e+30, -1e+30, -1e+30, -1e+30, -0.26, -0.26;
//
//   A.resize(12, 2);
//   A << 
//   -3.0895,   1.3914,
//   -2.4895,   1.3833,
//   -1.8896,   1.3752,
//   -1.8815,   1.9751,
// -0.089752,   1.3509,
// -0.081651,   1.9508,
//   0.50209,  0.74284,
//     1.102,  0.73474,
//     1.702,  0.72664,
//    2.3019,  0.71854,
//         1,        0,
//         0,        1;
//
//   b.resize(12, 1);
//   b <<
//   25502,
//   17078,
//   10454,
//   15403,
//  1382.4,
//  6331.1,
// -1190.2,
//  1185.8,
//  5361.9,
//   11338,
//    0.26,
//    0.26;
//
//   // l.conservativeResize(20, l.cols());
//   // b.conservativeResize(20, b.cols());
//   // A.conservativeResize(20, A.cols());
//   // double vel = 1e30;
//   // l(l.rows()-1) = -vel;
//   // l(l.rows()-2) = -vel;
//   // b(b.rows()-1) = vel;
//   // b(b.rows()-2) = vel;
//   // A(A.rows()-1, 1) = 1;
//   // A(A.rows()-1, 0) = 0;
//   // A(A.rows()-2, 1) = 0;
//   // A(A.rows()-2, 0) = 1;
//   // std::cout << b;
//
//   OsqpEigen::Solver solver;
//   int N = 1;
//   // settings
//   solver.settings()->setVerbosity(true);
//   solver.settings()->setWarmStart(true);
//   solver.settings()->setPrimalInfeasibilityTolerance(1e-15);
//
//   // configuring the QP
//   solver.data()->setNumberOfVariables(N * 2);
//   solver.data()->setNumberOfConstraints(A.rows());
//
//   Eigen::SparseMatrix<double> _A = A.sparseView(), _H = H.sparseView();
//
//   if (!solver.data()->setHessianMatrix(_H)) {
//     exit(1);
//   }
//   if (!solver.data()->setGradient(f)) {
//     exit(1);
//   }
//   if (!solver.data()->setLinearConstraintsMatrix(_A)) {
//     exit(1);
//   }
//   if (!solver.data()->setUpperBound(b)) {
//     exit(1);
//   }
//
//   if (!solver.data()->setLowerBound(l)) {
//     exit(1);
//   }
//   // instantiate the solver
//   if (!solver.initSolver()) {
//     exit(1);
//   }
//
//   // solve the QP problem
//   if (solver.solveProblem() != OsqpEigen::ErrorExitFlag::NoError) {
//     exit(1);
//   }
//
//   MatrixXd solution = solver.getSolution().reshaped(2, N);
//   // std::cout << A*solution - b << "\n";
//   std::cout << solution << "\n";
//   return 0;
// }
