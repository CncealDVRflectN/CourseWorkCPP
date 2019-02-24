#ifndef COURSEWORKCPP_EQUILIBRIUMFLUID_H
#define COURSEWORKCPP_EQUILIBRIUMFLUID_H

#include "fluid.h"
#include "../util/RightSweep.h"
#include "exceptions/IterationsLimitException.h"
#include "exceptions/IncorrectResultException.h"
#include "exceptions/TargetBondException.h"

class EquilibriumFluid
{
public:
    Result calcResult(Params &params) noexcept(false);

    std::vector<Result>* calcRelaxation(RelaxationParams &params) noexcept(false);

    std::vector<Result>* calcRelaxation(RelaxationParams &params, std::vector<Vector2> *heightCoefs) noexcept(false);

protected:
    RightSweep rightSweep;

    Params lastParams;
    std::vector<double> lastCorrectResult;

    std::vector<double> nodes;
    std::vector<double> nextApprox;
    std::vector<double> curApprox;
    double step;

    std::vector<double> coefsLowerDiagonal;
    std::vector<double> coefsMainDiagonal;
    std::vector<double> coefsUpperDiagonal;
    std::vector<double> rightVect;

    unsigned int iterationsCounter;


//    EquilibriumFluid();


    virtual void calcNextApproximation(Params &params) = 0;

    virtual void calcInitialApproximation(Params &params) = 0;

    virtual double calcVolumeNondimMul(std::vector<double> &func) = 0;

    std::vector<Vector2> getPoints(std::vector<double> &func);

private:
    bool isCorrect(std::vector<double> &result);

    void calcFunctionValues(Params &params) noexcept(false);
};


#endif //COURSEWORKCPP_EQUILIBRIUMFLUID_H
