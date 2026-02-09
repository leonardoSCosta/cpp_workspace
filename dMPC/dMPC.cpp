#include "Eigen/Dense"
#include "Eigen/Sparse"
#include "Eigen/src/Core/ArithmeticSequence.h"
#include "Eigen/src/Core/Matrix.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <iostream>

#include <chrono>

#include <fmt/core.h>

using namespace Eigen::indexing;

Eigen::MatrixXd robotKinematics(const Eigen::Matrix<double, 3, 1>& _robot,
                                const Eigen::Matrix<double, 2, 1>& _control,
                                float _dt);

float simulateControl(const Eigen::Matrix<double, 3, 1>& _robot,
                      const Eigen::Matrix<double, 2, 1>& _control,
                      const Eigen::Matrix<double, 3, 1>& _target,
                      const Eigen::Matrix<double, -1, 2>& _obstacles,
                      const Eigen::Matrix<double, 4, 2>& _footprint,
                      float _totalTime, float _timeStep,
                      float& _obstacleDistance);

bool checkCollision(const Eigen::Matrix<double, 3, 1>& _robot,
                    const Eigen::Matrix<double, 4, 2>& _footprint,
                    const Eigen::Matrix<double, -1, 2>& _obstacles);

float distanceToObstacles(const Eigen::Matrix<double, 3, 1>& _pos,
                          const Eigen::Matrix<double, -1, 2>& _obstacles);

Eigen::Vector2d findSafeControl(
    const Eigen::Matrix<double, 3, 1>& _robot,
    const Eigen::VectorXd& _linear, const Eigen::VectorXd& _angular,
    const Eigen::Matrix<double, 3, 1>& _target,
    const Eigen::Matrix<double, 4, 2>& _footprint,
    const Eigen::Matrix<double, -1, 2>& _obstacles);

Eigen::MatrixXd robotKinematics(
    const Eigen::Matrix<double, 3, 1>& _robot,
    const Eigen::Matrix<double, 2, 1>& _control, float _dt)
{
    double theta = _robot(2, 0);

    Eigen::Matrix<double, 3, 2> B;
    B << cos(theta), 0, sin(theta), 0, 0, 1;

    Eigen::Matrix3d A;
    A << 1, 0, 0, 0, 1, 0, 0, 0, 1;

    return A * _robot + B * _control * _dt;
}

float simulateControl(
    const Eigen::Matrix<double, 3, 1>& _robot,
    const Eigen::Matrix<double, 2, 1>& _control,
    const Eigen::Matrix<double, 3, 1>& _target,
    const Eigen::Matrix<double, -1, 2>& _obstacles,
    const Eigen::Matrix<double, 4, 2>& _footprint,
    float _totalTime, float _timeStep,
    float& _obstacleDistance)
{
    int samples = int(ceil(_totalTime / _timeStep));
    Eigen::MatrixXd path(samples, 3);

    Eigen::Matrix<double, 3, 1> simRobot = _robot, point = _robot;
    Eigen::Matrix<double, 4, 2> updatedFootprint = _footprint;
    float targetMinDistance = 1e6;

    for (int i = 0; i < path.rows(); i++)
    {
        simRobot = robotKinematics(simRobot, _control, _timeStep);
        for (int n = 0; n < updatedFootprint.rows(); n++)
        {
            point(seq(0, 1), 0) = updatedFootprint(n, seq(0, 1));
            point(2, 0) = simRobot(2, 0);

            point = robotKinematics(point, _control, _timeStep);
            updatedFootprint.row(n) = point(seq(0, 1), 0).transpose();
        }

        if (checkCollision(simRobot, updatedFootprint, _obstacles) == false || i < 1)
        {
            path.row(i) = simRobot;
            targetMinDistance =
                std::min(targetMinDistance, float((_target(seq(0, 1), 0) - _control).norm()));
        }
        else
        {
            // fmt::print("Unsafe control: ({}, {}) D = {}, O = {}, T = {}\n",
            //             _control(0), _control(1), targetMinDistance,
            //             _obstacleDistance, i * _timeStep);
            if (i * _timeStep >= _totalTime * 0.9f)
            {
                _obstacleDistance = distanceToObstacles(path.row(i), _obstacles);
                return targetMinDistance;
            }
            return -1;
        }
    }
    _obstacleDistance = distanceToObstacles(path.row(samples - 1), _obstacles);
    
    // fmt::print("Safe control: ({}, {}) D = {}, O = {}\n",
    //             _control(0), _control(1), targetMinDistance,
    //             _obstacleDistance);
    return targetMinDistance;
}

bool checkCollision(
    const Eigen::Matrix<double, 3, 1>&,
    const Eigen::Matrix<double, 4, 2>& _footprint,
    const Eigen::Matrix<double, -1, 2>& _obstacles)
{
    Eigen::Vector2d reference = _footprint.row(0).transpose(),
                    va = _footprint.row(1).transpose() - reference,
                    vb = _footprint.row(3).transpose() - reference,
                    vObstacle = va;

    float threshold = -0.00, insideA, insideB;

    for (int i = 0; i < _obstacles.rows(); i++)
    {
        vObstacle = _obstacles.row(i).transpose() - reference;
        insideA = vObstacle.dot(va) / va.dot(va);
        insideB = vObstacle.dot(vb) / vb.dot(vb);
        if (threshold <= insideA && insideA <= 1.0 - threshold &&
            threshold <= insideB && insideB <= 1.0 - threshold)
        {
            return true;
        }
    }
    return false;
}

float distanceToObstacles(
    const Eigen::Matrix<double, 3, 1>& _pos,
    const Eigen::Matrix<double, -1, 2>& _obstacles)
{
    Eigen::Vector2d xy(_pos(0), _pos(1));
    return (_obstacles.rowwise() - xy.transpose()).rowwise().norm().minCoeff();
}

Eigen::Vector2d findSafeControl(
    const Eigen::Matrix<double, 3, 1>& _robot, const Eigen::VectorXd& _linear,
    const Eigen::VectorXd& _angular, const Eigen::Matrix<double, 3, 1>& _target,
    const Eigen::Matrix<double, 4, 2>& _footprint,
    const Eigen::Matrix<double, -1, 2>& _obstacles)
{
    float simulationTime = 10, timeStep = 1,
          obstacleDistance, targetDistance,
          obstacleSolution, targetSolution = 1e10;
    Eigen::Matrix<double, 2, 1> control, controlSolution;
    controlSolution.setZero();


    for (int i = 0; i < _linear.size(); i++)
    {
        for (int j = 0; j < _angular.size(); j++)
        {
            control << _linear(i), _angular(j);

            targetDistance = simulateControl(
                _robot, control, _target, _obstacles, _footprint,
                simulationTime, timeStep, obstacleDistance);

            if (targetDistance > 0 && targetDistance < targetSolution &&
                obstacleDistance > 1.9)
            {
                controlSolution = control;
                targetSolution = targetDistance;
                obstacleSolution = obstacleDistance;
            }
        }
    }

    // std::cout << "Obst:\n" << _obstacles << "\nFoot\n" << _footprint << "\nRobot\n" << _robot << "\nTarget\n" << _target;
    // fmt::print("\nSafe control: ({}, {}) D = {}, O = {}\n",
    //             controlSolution(0), controlSolution(1), targetSolution,
    //             obstacleSolution);
    return Eigen::Vector2d(controlSolution(0), controlSolution(1));
}

Eigen::VectorXd linspace(double _start, double _end,
                                                 int _n)
{
    Eigen::VectorXd data;
    data.resize(_n);

    for (int i = 0; i < _n; i++)
    {
        data(i) = _start + i * (_end - _start) / (_n - 1);
    }
    return data;
}

int main (int, char *[]) 
{
    Eigen::VectorXd linearV = linspace(-0.26, 0.26, 10),
                    angularW = linspace(-1.0, 1.0, 10);
    Eigen::Matrix<double, 3, 1> target, robot;
    robot << 5.46319, 3.39063, 0;
    target << 0.26, 0.22144, 0;

    Eigen::MatrixXd obstacles;
    obstacles.resize(15, 2);
    obstacles << 
 3.95,3.15,
 4.05,2.35,
 4.05,2.95,
 4.05,3.05,
 4.05,3.15,
 4.15,2.45,
 4.15,2.55,
 4.15,2.65,
 4.15,2.75,
 4.15,2.85,
 4.15,4.65,
 4.25,4.65,
 4.35,4.65,
 4.45,4.75,
 4.45,4.85;
    Eigen::Matrix<double, 4, 2> footprint;// = getFootprint();
    footprint << 
 5.35811,3.05439,
 5.11155,3.36937,
 5.56827,3.72687,
 5.81483,3.41189;

    // robot << _pose.pose.position.x, _pose.pose.position.y,
    //     _pose.pose.position.z;
    // target << _goal(0), _goal(1), 0;
    // target += robot;

    // std::cout << "v = " << linearV.transpose() << "\nw = " << angularW.transpose() << "\n";
    using namespace std;
    auto start = chrono::high_resolution_clock::now();
    Eigen::Vector2d u = findSafeControl(robot, linearV, angularW, target, footprint,
                           obstacles);
    auto end = chrono::high_resolution_clock::now();
    std::cout << "Solution:\n" << u << " Time: " << chrono::duration_cast<chrono::nanoseconds>(end - start).count() * 1e-6 << "ms \n";
    return 0;
}
