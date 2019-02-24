#ifndef COURSEWORKCPP_FLUID_H
#define COURSEWORKCPP_FLUID_H

#include <vector>
#include "../util/util.h"

typedef struct params_t
{
    double alpha;
    double bond;
    double relaxationCoef;
    double epsilon;
    unsigned int splitNum;
    unsigned int iterationsNumMax;
    bool volumeNondim;
} Params;

typedef struct relaxation_params_t
{
    double alpha;
    double epsilon;
    double targetBond;
    double bondStep;
    double relaxationCoefMin;
    unsigned int splitNum;
    unsigned int resultsNum;
    unsigned int iterationsNumMax;
    bool volumeNondim;
} RelaxationParams;

typedef struct result_t
{
    std::vector<Vector2> points;
    double alpha;
    double bond;
    double relaxationCoef;
    unsigned int iterationsNum;
} Result;

#endif //COURSEWORKCPP_FLUID_H
