#ifndef COURSEWORKCPP_PARAMETRIC_H
#define COURSEWORKCPP_PARAMETRIC_H

#include "../EquilibriumFluid.h"
#include <functional>

class Parametric : public EquilibriumFluid
{
public:
	std::vector<Result>* calcRelaxation(RelaxationParams &params) override;

	std::vector<Result>* calcRelaxation(RelaxationParams &params, std::vector<Vector2> *heightCoefs) override;

	std::vector<Result>* calcRelaxationXY(RelaxationParams &params, std::vector<Vector2> *heightCoefs);

	std::vector<Result>* calcRelaxationYX(RelaxationParams &params, std::vector<Vector2> *heightCoefs);

protected:
	RightSweep rightSweep;

	Params lastParams;

	std::vector<double> lastValidResultX;
	std::vector<double> lastValidResultY;

	std::vector<double> nodes;
	std::vector<double> nextApproxX;
	std::vector<double> nextApproxY;
	std::vector<double> curApproxX;
	std::vector<double> curApproxY;
	double step;

	std::vector<double> coefsLowerDiagonal;
	std::vector<double> coefsMainDiagonal;
	std::vector<double> coefsUpperDiagonal;
	std::vector<double> rightVect;

	unsigned int iterationsCounter;


	virtual void calcInitialApproximationX(Params &params) = 0;

	virtual void calcInitialApproximationY(Params &params) = 0;

	virtual void calcNextApproximationX(Params &params, std::vector<double> &valY, std::vector<double> &prevValY) = 0;

	virtual void calcNextApproximationY(Params &params, std::vector<double> &valX, std::vector<double> &prevValX) = 0;

	virtual double calcVolumeNondimMul(std::vector<double> &valX, std::vector<double> &valY) = 0;

private:
	std::vector<Result>* calcRelaxation(std::function<void(Params &params)> &calcValues, 
										RelaxationParams &params, std::vector<Vector2> *heightCoefs);

	bool isValid(std::vector<double> &vals);

	std::vector<Vector2> getPoints(std::vector<double> &valX, std::vector<double> &valY);

	void calcInitialValues(Params &params);

	void calcNextValidApprox(std::function<void(Params &params)> &calcValues, Params &params, double minRelaxCoef) noexcept(false);

	void calcValuesXY(Params &params) noexcept(false);

	void calcValuesYX(Params &params) noexcept(false);
};

#endif //COURSEWORKCPP_PARAMETRIC_H