#include "Parametric.h"


Parametric::Parametric()
{
	lastParams = { 0.0, 0.0, 0.0, 0.0, 0U, 0U, false };
	step = 0.0;
	iterationsCounter = 0U;
}

std::vector<Result>* Parametric::calcRelaxation(RelaxationParams &params)
{
	return calcRelaxation(params, nullptr);
}


std::vector<Result>* Parametric::calcRelaxation(RelaxationParams &params, std::vector<Vector2> *heightCoefs)
{
	return calcRelaxationXY(params, heightCoefs);
}


std::vector<Result>* Parametric::calcRelaxationXY(RelaxationParams &params, std::vector<Vector2> *heightCoefs)
{
	std::function<void(Params &params)> calcValues = std::bind(&Parametric::calcValuesXY, this, std::placeholders::_1);
	return calcRelaxation(calcValues, params, heightCoefs);
}


std::vector<Result>* Parametric::calcRelaxationYX(RelaxationParams &params, std::vector<Vector2> *heightCoefs)
{
	std::function<void(Params &params)> calcValues = std::bind(&Parametric::calcValuesYX, this, std::placeholders::_1);
	return calcRelaxation(calcValues, params, heightCoefs);
}

// Private

std::vector<Result>* Parametric::calcRelaxation(std::function<void(Params &params)> &calcValues,
												RelaxationParams &params, std::vector<Vector2> *heightCoefs)
{
	Params problemParams = { params.alpha, 0.0, 1.0, params.epsilon, params.splitNum, params.iterationsNumMax,
							params.volumeNondim };
	auto *results = new std::vector<Result>;
	std::vector<double> resultsTargetBond(params.resultsNum);
	double resultsTargetBondStep = params.targetBond / (params.resultsNum - 1);
	unsigned int currentResultIndex = 0;

	if (params.resultsNum == 1)
	{
		resultsTargetBond[currentResultIndex] = params.targetBond;
	}
	else
	{
		for (auto i = 0U; i < params.resultsNum; i++)
		{
			resultsTargetBond[i] = i * resultsTargetBondStep;
		}
	}

	while (currentResultIndex < params.resultsNum)
	{
		calcNextValidApprox(calcValues, problemParams, params.relaxationCoefMin);

		const std::vector<Vector2> &points = getPoints(lastValidResultX, lastValidResultY);

		if (problemParams.bond >= resultsTargetBond[currentResultIndex])
		{
			results->push_back({ std::vector<Vector2>(points), problemParams.alpha,
								problemParams.bond, problemParams.relaxationCoef, iterationsCounter });
			currentResultIndex++;
		}

		if (heightCoefs != nullptr)
		{
			heightCoefs->push_back({ problemParams.bond, points.front().y / points.back().x });
		}

		problemParams.bond = fmin(problemParams.bond + params.bondStep, params.targetBond);
	}

	return results;
}


bool Parametric::isValid(std::vector<double> &vals)
{
	for (auto val : vals)
	{
		if (!std::isfinite(val))
		{
			return false;
		}
	}
	return true;
}


std::vector<Vector2> Parametric::getPoints(std::vector<double> &valX, std::vector<double> &valY)
{
	std::vector<Vector2> result(nodes.size());
	double volumeNondimMul = 1.0f;

	if (lastParams.volumeNondim)
	{
		volumeNondimMul = calcVolumeNondimMul(valX, valY);
	}

	auto resultSize = result.size();
	for (auto i = 0U; i < resultSize; i++)
	{
		result[i] = { (valX[i] * volumeNondimMul), (valY[i] * volumeNondimMul) };
	}

	return result;
}


void Parametric::calcInitialValues(Params &params)
{
	auto nodesSize = nodes.size();

	if (nodesSize != params.splitNum + 1)
	{
		step = 1.0 / params.splitNum;

		nodesSize = params.splitNum + 1;
		nodes.resize(nodesSize);
		for (auto i = 0U; i < nodesSize; i++)
		{
			nodes[i] = i * step;
		}

		nextApproxX.resize(nodesSize);
		nextApproxY.resize(nodesSize);
		curApproxX.resize(nodesSize);
		curApproxY.resize(nodesSize);
		coefsLowerDiagonal.resize(params.splitNum);
		coefsMainDiagonal.resize(nodesSize);
		coefsUpperDiagonal.resize(params.splitNum);
		rightVect.resize(nodesSize);
	}

	calcInitialApproximationX(params);
	calcInitialApproximationY(params);
}


void Parametric::calcNextValidApprox(std::function<void(Params &params)> &calcValues, 
									 Params &params, double minRelaxCoef) noexcept(false)
{
	bool isCalculated = false;

	while (params.relaxationCoef >= minRelaxCoef && !isCalculated)
	{
		try
		{
			calcValues(params);
			isCalculated = true;
		}
		catch (IterationsLimitException &e)
		{
			params.relaxationCoef *= 0.5;
		}
		catch (InvalidResultException &e)
		{
			params.relaxationCoef *= 0.5;
		}
	}

	if (!isCalculated)
	{
		throw TargetBondException();
	}
}


void Parametric::calcValuesXY(Params &params) noexcept(false)
{
	double curEpsilon = params.epsilon * params.relaxationCoef;

	if (params.bond == 0.0 && (lastParams.alpha != params.alpha))
	{
		calcInitialValues(params);
	}
	else
	{
		nextApproxX = lastValidResultX;
		nextApproxY = lastValidResultY;
	}

	iterationsCounter = 0;
	do
	{
		nextApproxX.swap(curApproxX);
		nextApproxY.swap(curApproxY);

		calcNextApproximationX(params, curApproxY, curApproxY);
		relaxation(nextApproxX, curApproxX, params.relaxationCoef);

		calcNextApproximationY(params, nextApproxX, curApproxX);
		relaxation(nextApproxY, curApproxY, params.relaxationCoef);

		iterationsCounter++;
	} while (norm(nextApproxY, curApproxY) > curEpsilon && iterationsCounter < params.iterationsNumMax);

	if (iterationsCounter >= params.iterationsNumMax)
	{
		throw IterationsLimitException();
	}
	else if (!isValid(nextApproxY))
	{
		throw InvalidResultException();
	}
	else
	{
		lastParams = params;
		lastValidResultX = nextApproxX;
		lastValidResultY = nextApproxY;
	}
}


void Parametric::calcValuesYX(Params &params) noexcept(false)
{
	double curEpsilon = params.epsilon * params.relaxationCoef;

	if (params.bond == 0.0 && (lastParams.alpha != params.alpha))
	{
		calcInitialValues(params);
	}
	else
	{
		nextApproxX = lastValidResultX;
		nextApproxY = lastValidResultY;
	}
	
	iterationsCounter = 0;
	do
	{
		nextApproxX.swap(curApproxX);
		nextApproxY.swap(curApproxY);

		calcNextApproximationY(params, curApproxX, curApproxX);
		relaxation(nextApproxY, curApproxY, params.relaxationCoef);

		calcNextApproximationX(params, nextApproxY, curApproxY);
		relaxation(nextApproxX, curApproxX, params.relaxationCoef);

		iterationsCounter++;
	} while (norm(nextApproxY, curApproxY) > curEpsilon && iterationsCounter < params.iterationsNumMax);

	if (iterationsCounter >= params.iterationsNumMax)
	{
		throw IterationsLimitException();
	}
	else if (!isValid(nextApproxY))
	{
		throw InvalidResultException();
	}
	else
	{
		lastParams = params;
		lastValidResultX = nextApproxX;
		lastValidResultY = nextApproxY;
	}
}