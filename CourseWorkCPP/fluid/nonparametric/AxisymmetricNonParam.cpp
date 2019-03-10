#include "AxisymmetricNonParam.h"


AxisymmetricNonParam::AxisymmetricNonParam() : NonParametric()
{
	
}

// Protected

void AxisymmetricNonParam::calcNextApproximation(Params &params)
{
    double integralCbrt;
    double q;

    if (coefs.size() != params.splitNum + 1)
    {
        coefs.resize(params.splitNum + 1);
    }

    calcCoefs(step);

    integralCbrt = cbrt(calcIntegralTrapeze(curApprox, nodes, step));
    q = -2.0 * sin(params.alpha) - params.bond * integralCbrt / M_PI;

    coefsMainDiagonal[0] = -(1.0 / step + (step / 4.0) * (params.bond / pow(integralCbrt, 2.0)));
    coefsUpperDiagonal[0] = 1.0 / step;
    coefsMainDiagonal[params.splitNum] = 1.0;

    rightVect[0] = step * q / 4.0;
    rightVect[params.splitNum] = 0.0;

    for (auto i = 1U; i < params.splitNum; i++)
    {
        coefsLowerDiagonal[i - 1] = coefs[i];
        coefsMainDiagonal[i] = -(coefs[i] + coefs[i + 1]);
        coefsUpperDiagonal[i] = coefs[i + 1];

        rightVect[i] = nodes[i] * step * step * (params.bond * curApprox[i] / pow(integralCbrt, 2.0) + q);
    }

    rightSweep.calcRightSweep(coefsLowerDiagonal, coefsMainDiagonal, coefsUpperDiagonal, rightVect, nextApprox);
}

void AxisymmetricNonParam::calcInitialApproximation(Params &params)
{
    double sinVal = sin(params.alpha);
    double ctg = 1.0 / tan(params.alpha);
    auto nodesSize = nodes.size();

    for (auto i = 0U; i < nodesSize; i++)
    {
        nextApprox[i] = sqrt(1.0 - pow(nodes[i] * sinVal, 2.0)) / sinVal - ctg;
    }
}

double AxisymmetricNonParam::calcVolumeNondimMul(std::vector<double> &func)
{
    return 1.0 / cbrt(calcIntegralTrapeze(func, nodes, step));
}


void AxisymmetricNonParam::calcCoefs(double stepVal)
{
    double halfStep = stepVal / 2.0;
    auto coefsSize = coefs.size();

    coefs[0] = 0.0;

    for (auto i = 1U; i < coefsSize; i++)
    {
        coefs[i] = (nodes[i] - halfStep) / sqrt(1.0 + pow((curApprox[i] - curApprox[i - 1]) / stepVal, 2.0));
    }
}

double AxisymmetricNonParam::calcIntegralTrapeze(std::vector<double> &values, std::vector<double> &nodesVal, double stepVal)
{
    double result = values[0] * nodesVal[0] / 2.0;
    auto length = values.size() - 1;

    for (auto i = 1; i < length; i++)
    {
        result += values[i] * nodesVal[i];
    }

    return 2.0 * M_PI * result * stepVal;
}
