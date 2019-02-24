#include "Plain.h"

void Plain::calcNextApproximation(Params &params)
{
    double coef = 8.0 * step;
    double integral;

    integral = calcIntegralTrapeze(curApprox, step);

    coefsMainDiagonal[0] = -1.0 - step * step * params.bond / (2.0 * integral);
    coefsUpperDiagonal[0] = 1.0;
    coefsMainDiagonal[params.splitNum] = 1.0;

    rightVect[0] = -(step * step / 2.0) * (params.bond / 2.0 + sin(params.alpha));
    rightVect[params.splitNum] = 0.0;

    for (auto i = 1U; i < params.splitNum; i++)
    {
        coefsLowerDiagonal[i - 1] = coef;
        coefsMainDiagonal[i] = -2.0 * coef;
        coefsUpperDiagonal[i] = coef;

        rightVect[i] = pow(4.0 * step * step + pow(curApprox[i + 1] - curApprox[i - 1], 2.0), 3.0 / 2.0) *
                       (params.bond * curApprox[i] / integral - params.bond / 2.0 - sin(params.alpha));
    }

    rightSweep.calcRightSweep(coefsLowerDiagonal, coefsMainDiagonal, coefsUpperDiagonal, rightVect, nextApprox);
}

void Plain::calcInitialApproximation(Params &params)
{
    double sinVal = sin(params.alpha);
    double ctg = 1.0 / tan(params.alpha);
    auto nodesSize = nodes.size();

    for (auto i = 0U; i < nodesSize; i++)
    {
        nextApprox[i] = sqrt(1.0 - pow(nodes[i] * sinVal, 2.0)) / sinVal - ctg;
    }
}

double Plain::calcVolumeNondimMul(std::vector<double> &func)
{
    return 1.0 / sqrt(calcIntegralTrapeze(func, step));
}

double Plain::calcIntegralTrapeze(std::vector<double> values, double step)
{
    double result = values[0] / 2.0;
    auto length = values.size() - 1;

    for (auto i = 1; i < length; i++)
    {
        result += values[i];
    }

    return 2.0 * result * step;
}
