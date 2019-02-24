#ifndef COURSEWORKCPP_PLOT_H
#define COURSEWORKCPP_PLOT_H

#include <iostream>
#include <cstdio>
#include "../fluid/fluid.h"

class Plot
{
public:
    Plot(std::vector<Result> &axisymData, std::vector<Result> &plainData,
            std::vector<Vector2> &axisymHeight, std::vector<Vector2> &plainHeight);

    void plot(unsigned int width, unsigned int height, bool equalAxis) noexcept(false);
private:
    FILE *axisymPipe;
    FILE *plainPipe;
    FILE *heightCoefsPipe;

    std::vector<Result> &axisymData;
    std::vector<Result> &plainData;
    std::vector<Vector2> &axisymHeights;
    std::vector<Vector2> &plainHeights;


    void openPipes() noexcept(false);

    void closePipes();

    void setupAxisymPipe(unsigned int width, unsigned int height, bool equalAxis) noexcept(false);

    void setupPlainPipe(unsigned int width, unsigned int height, bool equalAxis) noexcept(false);

    void setupHeightCoefsPipe(unsigned int width, unsigned int height) noexcept(false);

    void wait();

    void uploadFluidData(FILE *pipe, std::vector<Result> &data);

    void uploadHeightCoefsData(FILE *pipe, std::vector<Vector2> &heightsCoefsData);
};


#endif //COURSEWORKCPP_PLOT_H
