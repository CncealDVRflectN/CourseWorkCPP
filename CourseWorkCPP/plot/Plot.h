#ifndef COURSEWORKCPP_PLOT_H
#define COURSEWORKCPP_PLOT_H

#include <iostream>
#include <cstdio>
#include "../fluid/fluid.h"


typedef struct plot_line_t
{
	std::vector<Vector2> points;
	std::string title;
} PlotLine;

typedef struct plot_params_t
{
	std::vector<PlotLine> lines;
	std::string title;
	std::string labelX;
	std::string labelY;
	unsigned int width;
	unsigned int height;
	bool equalAxis;
} PlotParams;


class Plot
{
public:
	void addPlot(PlotParams &plotParam);

	void clear();

    void plot() noexcept(false);

private:
	std::vector<PlotParams> plots;
	std::vector<FILE *> pipes;

    void openPipes() noexcept(false);

    void closePipes();

	void writePipes();

	void writePipe(FILE *pipe, PlotParams &plotParams);

    void wait();
};


#endif //COURSEWORKCPP_PLOT_H
