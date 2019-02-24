#ifndef COURSEWORKCPP_PLAIN_H
#define COURSEWORKCPP_PLAIN_H

#include "EquilibriumFluid.h"

class Plain : public EquilibriumFluid
{
protected:
    void calcNextApproximation(Params &params) override;

    void calcInitialApproximation(Params &params) override;

    double calcVolumeNondimMul(std::vector<double> &func) override;

private:
    double calcIntegralTrapeze(std::vector<double> values, double step);
};


#endif //COURSEWORKCPP_PLAIN_H
