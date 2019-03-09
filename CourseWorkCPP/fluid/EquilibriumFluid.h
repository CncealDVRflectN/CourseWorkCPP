#ifndef COURSEWORKCPP_EQUILIBRIUMFLUID_H
#define COURSEWORKCPP_EQUILIBRIUMFLUID_H

#include "fluid.h"
#include "../util/RightSweep.h"
#include "exceptions/IterationsLimitException.h"
#include "exceptions/InvalidResultException.h"
#include "exceptions/TargetBondException.h"

class EquilibriumFluid
{
public:
    virtual std::vector<Result>* calcRelaxation(RelaxationParams &params) noexcept(false) = 0;

    virtual std::vector<Result>* calcRelaxation(RelaxationParams &params, std::vector<Vector2> *heightCoefs) noexcept(false) = 0;
};


#endif //COURSEWORKCPP_EQUILIBRIUMFLUID_H
