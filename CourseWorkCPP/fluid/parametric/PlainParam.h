#ifndef COURSEWORKCPP_PLAINPARAM_H
#define COURSEWORKCPP_PLAINPARAM_H

#include "Parametric.h"

class PlainParam : public Parametric
{
public:
	PlainParam();

protected:
	void calcInitialApproximationX(Params &params) override;

	void calcInitialApproximationY(Params &params) override;

	void calcNextApproximationX(Params &params, std::vector<double> &valY, std::vector<double> &prevValY) override;

	void calcNextApproximationY(Params &params, std::vector<double> &valX, std::vector<double> &prevValX) override;

	double calcVolumeNondimMul(std::vector<double> &valX, std::vector<double> &valY) override;
};

#endif