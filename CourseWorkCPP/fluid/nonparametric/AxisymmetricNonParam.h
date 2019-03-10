#ifndef COURSEWORKCPP_AXISYMMETRICNONPARAM_H
#define COURSEWORKCPP_AXISYMMETRICNONPARAM_H

#include "NonParametric.h"

class AxisymmetricNonParam : public NonParametric
{
public:
	AxisymmetricNonParam();

protected:
    void calcNextApproximation(Params &params) override;

    void calcInitialApproximation(Params &params) override;

    double calcVolumeNondimMul(std::vector<double> &func) override;

private:
    std::vector<double> coefs;


    void calcCoefs(double stepVal);

    double calcIntegralTrapeze(std::vector<double> &values, std::vector<double> &nodesVal, double stepVal);
};


#endif //COURSEWORKCPP_AXISYMMETRIC_H
