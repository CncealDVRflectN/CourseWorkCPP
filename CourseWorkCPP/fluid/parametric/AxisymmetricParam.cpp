#include "AxisymmetricParam.h"


AxisymmetricParam::AxisymmetricParam() : Parametric()
{
	
}

// Protected

void AxisymmetricParam::calcInitialApproximationX(Params & params)
{
	auto nodesSize = nodes.size();
	for (auto i = 0U; i < nodesSize; i++)
	{
		nextApproxX[i] = sin(params.alpha * i * step) / params.alpha;
	}
}

void AxisymmetricParam::calcInitialApproximationY(Params & params)
{
	auto nodesSize = nodes.size();
	for (auto i = 0U; i < nodesSize; i++)
	{
		nextApproxY[i] = (cos(params.alpha * i * step) - cos(params.alpha)) / params.alpha;
	}
}

void AxisymmetricParam::calcNextApproximationX(Params &params, std::vector<double> &valY, std::vector<double> &prevValY)
{
	double integralCbrt = cbrt(calcIntegralTrapeze(curApproxX, prevValY));
	double integralCbrtSq = pow(integralCbrt, 2.0);
	double q = calcQ(params, curApproxX, integralCbrt);
	double tmp = sin(params.alpha);

	coefsMainDiagonal[0] = 1.0;
	coefsUpperDiagonal[0] = 0.0;
	coefsLowerDiagonal[params.splitNum - 1] = -1.0;
	coefsMainDiagonal[params.splitNum] = 1.0;

	rightVect[0] = 0.0;
	rightVect[params.splitNum] = step * (cos(params.alpha) - 0.5 * step * tmp * (tmp / curApproxX.back() + q));

	for (auto i = 1U; i < params.splitNum; i++)
	{
		coefsLowerDiagonal[i - 1] = 1.0;
		coefsMainDiagonal[i] = -2.0;
		coefsUpperDiagonal[i] = 1.0;

		tmp = 0.5 * (valY[i + 1] - valY[i - 1]);
		rightVect[i] = -step * tmp * (params.bond * prevValY[i] / integralCbrtSq - tmp / (step * curApproxX[i]) + q);
	}

	rightSweep.calcRightSweep(coefsLowerDiagonal, coefsMainDiagonal, coefsUpperDiagonal, rightVect, nextApproxX);
}

void AxisymmetricParam::calcNextApproximationY(Params &params, std::vector<double> &valX, std::vector<double> &prevValX)
{
	double integralCbrt = cbrt(calcIntegralTrapeze(prevValX, curApproxY));
	double integralCbrtSq = pow(integralCbrt, 2.0);
	double q = calcQ(params, prevValX, integralCbrt);
	double tmp = 0.25 * step * step;

	coefsMainDiagonal[0] = -(1.0 + tmp * params.bond / integralCbrtSq);
	coefsUpperDiagonal[0] = 1.0;
	coefsLowerDiagonal[params.splitNum - 1] = 0.0;
	coefsMainDiagonal[params.splitNum] = 1.0;

	rightVect[0] = tmp * q;
	rightVect[params.splitNum] = 0.0;

	for (auto i = 1U; i < params.splitNum; i++)
	{
		coefsLowerDiagonal[i - 1] = 1.0 + valX[i - 1] / valX[i];
		coefsMainDiagonal[i] = -(2.0 + (valX[i + 1] + valX[i - 1]) / valX[i]);
		coefsUpperDiagonal[i] = 1.0 + valX[i + 1] / valX[i];

		rightVect[i] = step * (valX[i + 1] - valX[i - 1]) * (params.bond * curApproxY[i] / integralCbrtSq + q);
	}

	rightSweep.calcRightSweep(coefsLowerDiagonal, coefsMainDiagonal, coefsUpperDiagonal, rightVect, nextApproxY);
}

double AxisymmetricParam::calcVolumeNondimMul(std::vector<double> &valX, std::vector<double> &valY)
{
	return 1.0 / cbrt(calcIntegralTrapeze(valX, valY));
}

// Private

double AxisymmetricParam::calcIntegralTrapeze(std::vector<double> &valX, std::vector<double> &valY)
{
	double result = 0.0;
	auto limit = valX.size() - 1;

	for (auto i = 1U; i < limit; i++)
	{
		result += valX[i] * valY[i] * (valX[i + 1] - valX[i - 1]);
	}

	return M_PI * result;
}

double AxisymmetricParam::calcQ(Params &params, std::vector<double> &valX, double integralCbrt)
{
	return -(2.0 * sin(params.alpha) + params.bond * integralCbrt / (M_PI * valX.back())) / valX.back();
}