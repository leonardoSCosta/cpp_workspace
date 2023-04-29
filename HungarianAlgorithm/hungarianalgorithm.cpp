#include "hungarianalgorithm.h"

HungarianAlgorithm::HungarianAlgorithm(quint8 nRobots)
{
    n = nRobots;
    costMatrix = dlib::zeros_matrix<long>(n, n);

    solution.clear();
    dSolutionCost = MAX_VAL;
}

long HungarianAlgorithm::iInvertValue(double _value)
{
    return 1e9 * (1.0 / _value);
}

void HungarianAlgorithm::vSetMatrixValue(quint8 row, quint8 column, double value)
{
    if(n <= 0 || row >= n || column >= n)
    {
        return;
    }

    // Inverte o valor, pois queremos minimizar
    costMatrix(row, column) = iInvertValue(value);
}

QVector<Assignment> HungarianAlgorithm::runAlgorithm(double *_solutionCost)
{
    std::vector<long> assignment = max_cost_assignment(costMatrix);

    int task = 0;
    solution.clear();
    for(quint8 row = 0; row < n; ++row)
    {
        for(quint8 column = 0; column < n; ++column)
        {
            costMatrix(row, column) = iInvertValue(costMatrix(row, column));
        }
        task = assignment.at(row);
        solution.append(Assignment(row, task, costMatrix(row, task)));
    }

    dSolutionCost = assignment_cost(costMatrix, assignment);

    if(_solutionCost != nullptr)
    {
        *_solutionCost = dSolutionCost;
    }

    return solution;
}
