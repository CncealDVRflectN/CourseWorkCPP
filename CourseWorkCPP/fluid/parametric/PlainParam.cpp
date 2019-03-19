#include "PlainParam.h"


PlainParam::PlainParam() : Parametric()
{

}

// Protected

void PlainParam::calcInitialApproximationX(Params & params)
{
	auto nodesSize = nodes.size();
	for (auto i = 0U; i < nodesSize; i++)
	{
		nextApproxX[i] = sin(params.alpha * i * step) / params.alpha;
	}
}

void PlainParam::calcInitialApproximationY(Params & params)
{
	auto nodesSize = nodes.size();

	for (auto i = 0U; i < nodesSize; i++)
	{
		nextApproxY[i] = (cos(params.alpha * i * step) - cos(params.alpha)) / params.alpha;
	}
}

void PlainParam::calcNextApproximationX(Params &params, std::vector<double> &valY, std::vector<double> &prevValY)
{	
	double q = calcQ(params, curApproxX);
	double integralTrapeze = calcIntegralTrapeze(curApproxX, curApproxY);
	double tmp;

	coefsMainDiagonal[0] = 1.0;
	coefsUpperDiagonal[0] = 0.0;
	coefsLowerDiagonal[params.splitNum - 1] = -1.0;
	coefsMainDiagonal[params.splitNum] = 1.0;
	
	rightVect[0] = 0;//1.0 - 0.5 * step * (curApproxY[1] - curApproxY[0]) * calcF(params, curApproxY[0], integralTrapeze, q);
	rightVect[params.splitNum] = step * cos(params.alpha) + 0.5 * step * (curApproxY[params.splitNum] - curApproxY[params.splitNum - 1]) * calcF(params, curApproxY[params.splitNum], integralTrapeze, q);

	for (auto i = 1U; i < params.splitNum; i++)
	{
		coefsLowerDiagonal[i - 1] = 1.0 ;
		coefsMainDiagonal[i] = -2.0;
		coefsUpperDiagonal[i] = 1.0;

		tmp = 0.5 * (curApproxY[i + 1] - curApproxY[i - 1]);
		rightVect[i] = -step * tmp * calcF(params, curApproxY[i], integralTrapeze, q);
	}

	rightSweep.calcRightSweep(coefsLowerDiagonal, coefsMainDiagonal, coefsUpperDiagonal, rightVect, nextApproxX);
}

void PlainParam::calcNextApproximationY(Params &params, std::vector<double> &valX, std::vector<double> &prevValX)
{	
	double q = calcQ(params, prevValX);
	double integralTrapeze = calcIntegralTrapeze(curApproxX, curApproxY);
	double tmp;

	coefsMainDiagonal[0] = -1.0;
	coefsUpperDiagonal[0] = 1.0;
	coefsLowerDiagonal[params.splitNum - 1] = 0.0;
	coefsMainDiagonal[params.splitNum] = 1.0;

	rightVect[0] = 0.5 * step * (curApproxX[1] - curApproxX[0]) * calcF(params, curApproxY[0], integralTrapeze, q);
	rightVect[params.splitNum] = 0;//- step * sin(params.alpha) - 0.5 * step * (curApproxX[params.splitNum] - curApproxX[params.splitNum - 1]) * calcF(params, curApproxY[params.splitNum], integralTrapeze, q);

	for (auto i = 1U; i < params.splitNum; i++)
	{
		coefsLowerDiagonal[i - 1] = 1.0;
		coefsMainDiagonal[i] = -2.0;
		coefsUpperDiagonal[i] = 1.0;

		tmp = 0.5 * (curApproxX[i + 1] - curApproxX[i - 1]);
		rightVect[i] = step * tmp * calcF(params, curApproxY[i], integralTrapeze, q);
	}

	rightSweep.calcRightSweep(coefsLowerDiagonal, coefsMainDiagonal, coefsUpperDiagonal, rightVect, nextApproxY);
}

double PlainParam::calcVolumeNondimMul(std::vector<double> &valX, std::vector<double> &valY)
{
	return 1.0 / calcIntegralTrapeze(valX, valY);
}

// Private

double PlainParam::calcIntegralTrapeze(std::vector<double> &valX, std::vector<double> &valY)
{
	double result = 0.0;
	auto limit = valX.size();	
	for (auto i = 0U; i < limit - 1; i++)
	{
		result += (valY[i] + valY[i + 1]) * (valX[i + 1] - valX[i]);
	}

	return result;
}

double PlainParam::calcQ(Params &params, std::vector<double> &valX)
{
	return -(2.0 * sin(params.alpha) + params.bond) / ( 2 * valX[params.splitNum]);
}

double PlainParam::calcF(Params &params, double y, double integral, double q)
{
	return params.bond * y / integral + q;
}

