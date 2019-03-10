#ifndef COURSEWORKCPP_NONPARAMETRIC_H
#define COURSEWORKCPP_NONPARAMETRIC_H

#include "../EquilibriumFluid.h"

class NonParametric : public EquilibriumFluid
{
public:
	NonParametric();

	std::vector<Result>* calcRelaxation(RelaxationParams &params) override;

	std::vector<Result>* calcRelaxation(RelaxationParams &params, std::vector<Vector2> *heightCoefs) override;

protected:
	RightSweep rightSweep;

	Params lastParams;
	std::vector<double> lastValidResult;

	std::vector<double> nodes;
	std::vector<double> nextApprox;
	std::vector<double> curApprox;
	double step;

	std::vector<double> coefsLowerDiagonal;
	std::vector<double> coefsMainDiagonal;
	std::vector<double> coefsUpperDiagonal;
	std::vector<double> rightVect;

	unsigned int iterationsCounter;


	virtual void calcInitialApproximation(Params &params) = 0;

	virtual void calcNextApproximation(Params &params) = 0;

	virtual double calcVolumeNondimMul(std::vector<double> &func) = 0;

private:
	std::vector<Vector2> getPoints(std::vector<double> &func);

	bool isValid(std::vector<double> &result);

	void calcFunctionValues(Params &params) noexcept(false);
};

#endif //COURSEWORKCPP_NONPARAMETRIC_H