#ifndef HUNGARIANALGORITHM_H
#define HUNGARIANALGORITHM_H

#include <QDebug>
#include <QSharedPointer>
#include <QVector2D>

#include <dlib/matrix/matrix.h>
#include <dlib/matrix/matrix_utilities.h>
#include <dlib/optimization/max_cost_assignment.h>

#define MAX_VAL 99999999

class Assignment
{
public:
    int robot;
    int task;
    double cost;
    bool used;

    Assignment()
    {
        robot = -1;
        task  = -1;
        used  = false;
    }

    Assignment(int _robot, int _task, double _cost)
    {
        robot = _robot;
        task  = _task;
        cost  = _cost;
        used  = _cost == 0 ? false : true;
    }
};

class HungarianAlgorithm
{
    quint8 n;
    QVector<Assignment> solution;
    double dSolutionCost;
    dlib::matrix<long> costMatrix;

    long iInvertValue(double _value);

public:
    HungarianAlgorithm(quint8 nRobots);

    void vSetMatrixValue(quint8 row, quint8 column, double value);

    QVector<Assignment> runAlgorithm(double *_solutionCost = nullptr);
};

#endif // HUNGARIANALGORITHM_H
