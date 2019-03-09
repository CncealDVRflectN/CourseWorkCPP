#ifndef COURSEWORKCPP_PLAINNONPARAM_H
#define COURSEWORKCPP_PLAINNONPARAM_H

#include "NonParametric.h"

class PlainNonParam : public NonParametric
{
protected:
    void calcNextApproximation(Params &params) override;

    void calcInitialApproximation(Params &params) override;

    double calcVolumeNondimMul(std::vector<double> &func) override;

private:
    double calcIntegralTrapeze(std::vector<double> values, double step);
};


#endif //COURSEWORKCPP_PLAIN_H
