#ifndef COURSEWORKCPP_AXISYMMETRICPARAM_H
#define COURSEWORKCPP_AXISYMMETRICPARAM_H

#include "Parametric.h"

class AxisymmetricParam : public Parametric
{
protected:
	void calcInitialApproximationX(Params &params) override;

	void calcInitialApproximationY(Params &params) override;

	void calcNextApproximationX(Params &params, std::vector<double> &valY, std::vector<double> &prevValY) override;

	void calcNextApproximationY(Params &params, std::vector<double> &valX, std::vector<double> &prevValX) override;

	double calcVolumeNondimMul(std::vector<double> &valX, std::vector<double> &valY) override;
private:
	double calcIntegralTrapeze(std::vector<double> &valX, std::vector<double> &valY);

	double calcQ(Params &params, std::vector<double> &valX, double integralVal);
};

#endif