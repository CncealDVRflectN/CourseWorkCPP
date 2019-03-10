#include "NonParametric.h"


NonParametric::NonParametric()
{
	lastParams = { 0.0, 0.0, 0.0, 0.0, 0U, 0U, false };
	step = 0.0;
	iterationsCounter = 0U;
}


std::vector<Result>* NonParametric::calcRelaxation(RelaxationParams &params) noexcept(false)
{
	return calcRelaxation(params, nullptr);
}

std::vector<Result>* NonParametric::calcRelaxation(RelaxationParams &params, std::vector<Vector2> *heightCoefs) noexcept(false)
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

	do
	{
		try
		{
			calcFunctionValues(problemParams);

			const std::vector<Vector2> &points = getPoints(lastValidResult);

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

			if (currentResultIndex >= params.resultsNum)
			{
				break;
			}

			problemParams.bond = fmin(problemParams.bond + params.bondStep, params.targetBond);
		}
		catch (IterationsLimitException &e)
		{
			problemParams.relaxationCoef /= 2.0;
		}
		catch (InvalidResultException &e)
		{
			problemParams.relaxationCoef /= 2.0;
		}
	} while (problemParams.relaxationCoef >= params.relaxationCoefMin);

	if (results->empty() || results->back().bond < params.targetBond)
	{
		throw TargetBondException();
	}

	return results;
}


std::vector<Vector2> NonParametric::getPoints(std::vector<double> &func)
{
	std::vector<Vector2> result(func.size());
	double volumeNondimMul = 1.0f;

	if (lastParams.volumeNondim)
	{
		volumeNondimMul = calcVolumeNondimMul(func);
	}

	auto resultSize = result.size();
	for (auto i = 0U; i < resultSize; i++)
	{
		result[i] = { (nodes[i] * volumeNondimMul), (func[i] * volumeNondimMul) };
	}

	return result;
}


bool NonParametric::isValid(std::vector<double> &result)
{
	for (auto point : result)
	{
		if (!std::isfinite(point))
		{
			return false;
		}
	}
	return true;
}

void NonParametric::calcFunctionValues(Params &params) noexcept(false)
{
	double curEpsilon = params.epsilon * params.relaxationCoef;
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

		nextApprox.resize(nodesSize);
		curApprox.resize(nodesSize);
		coefsLowerDiagonal.resize(params.splitNum);
		coefsMainDiagonal.resize(nodesSize);
		coefsUpperDiagonal.resize(params.splitNum);
		rightVect.resize(nodesSize);
	}

	if (lastParams.splitNum != params.splitNum || lastParams.bond > params.bond || lastParams.alpha != params.alpha ||
		lastParams.epsilon != params.epsilon)
	{
		calcInitialApproximation(params);
		lastValidResult.resize(params.splitNum + 1);
	}
	else
	{
		nextApprox = lastValidResult;
	}

	iterationsCounter = 0;
	do
	{
		nextApprox.swap(curApprox);
		calcNextApproximation(params);

		auto nextApproxSize = nextApprox.size();
		for (auto i = 0U; i < nextApproxSize; i++)
		{
			nextApprox[i] = (1.0 - params.relaxationCoef) * curApprox[i] + params.relaxationCoef * nextApprox[i];
		}

		iterationsCounter++;
	} while (norm(nextApprox, curApprox) > curEpsilon && iterationsCounter < params.iterationsNumMax);

	if (iterationsCounter >= params.iterationsNumMax)
	{
		throw IterationsLimitException();
	}

	if (!isValid(nextApprox))
	{
		throw InvalidResultException();
	}

	lastParams = params;
	lastValidResult = nextApprox;
}