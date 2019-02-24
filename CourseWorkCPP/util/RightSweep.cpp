#include "RightSweep.h"
#include <cmath>
#include <iostream>


void RightSweep::calcRightSweep(
        std::vector<double> &lowerDiagonal,
        std::vector<double> &mainDiagonal,
        std::vector<double> &upperDiagonal,
        std::vector<double> &vect,
        std::vector<double> &solutionDest)
{
    if (!isCorrect(lowerDiagonal, mainDiagonal, upperDiagonal))
    {
        std::cerr << "Warning: right sweep matrix incorrect!" << std::endl;
    }

    auto curBetaSize = beta.size();
    auto mainDiagSize = mainDiagonal.size();

    if (curBetaSize != mainDiagSize)
    {
        alpha.resize(mainDiagSize - 1);
        beta.resize(mainDiagSize);
    }

    calcAlpha(lowerDiagonal, mainDiagonal, upperDiagonal);
    calcBeta(lowerDiagonal, mainDiagonal, vect);

    return calcSolution(solutionDest);
}


void RightSweep::calcAlpha(
        std::vector<double> &lowerDiagonal,
        std::vector<double> &mainDiagonal,
        std::vector<double> &upperDiagonal)
{
    auto size = alpha.size();

    alpha[0] = -upperDiagonal[0] / mainDiagonal[0];
    for (auto i = 1; i < size; i++)
    {
        alpha[i] = -upperDiagonal[i] / (mainDiagonal[i] + lowerDiagonal[i - 1] * alpha[i - 1]);
    }
}

void RightSweep::calcBeta(
        std::vector<double> &lowerDiagonal,
        std::vector<double> &mainDiagonal,
        std::vector<double> &vect)
{
    auto size = beta.size();

    beta[0] = vect[0] / mainDiagonal[0];
    for (auto i = 1; i < size; i++)
    {
        beta[i] = (vect[i] - lowerDiagonal[i - 1] * beta[i - 1]) /
                  (mainDiagonal[i] + lowerDiagonal[i - 1] * alpha[i - 1]);
    }
}

void RightSweep::calcSolution(std::vector<double> &solutionDest)
{
    auto size = solutionDest.size();

    solutionDest[size - 1] = beta[size - 1];
    for (long long i = size - 2; i >= 0; i--)
    {
        solutionDest[i] = alpha[i] * solutionDest[i + 1] + beta[i];
    }
}

bool RightSweep::isCorrect(
        std::vector<double> &lowerDiagonal,
        std::vector<double> &mainDiagonal,
        std::vector<double> &upperDiagonal)
{
    auto mainDiagSize = mainDiagonal.size();
    auto lowerDiagSize = lowerDiagonal.size();
    auto upperDiagSize = upperDiagonal.size();

    if (lowerDiagSize != mainDiagSize - 1 || upperDiagSize != mainDiagSize - 1)
    {
        return false;
    }
    if (fabs(mainDiagonal[0]) < fabs(upperDiagonal[0]))
    {
        return false;
    }
    if (fabs(mainDiagonal[mainDiagSize - 1]) < fabs(lowerDiagonal[lowerDiagSize - 1]))
    {
        return false;
    }
    for (auto i = 1; i < upperDiagSize; i++)
    {
        if (fabs(mainDiagonal[i]) < (fabs(lowerDiagonal[i - 1]) + fabs(upperDiagonal[i])))
        {
            return false;
        }
    }
    return true;
}
