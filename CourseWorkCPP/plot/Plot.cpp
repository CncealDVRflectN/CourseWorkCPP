#include "Plot.h"
#include "exceptions/PipeException.h"
#include "exceptions/EmptyDataException.h"

#define GNUPLOT             "gnuplot"


void Plot::addPlot(PlotParams &plotParam)
{
	plots.insert(plots.end(), plotParam);
}

void Plot::clear()
{
	plots.clear();
}

void Plot::plot() noexcept(false)
{
    openPipes();
	writePipes();
    wait();
    closePipes();
}

// Private

void Plot::openPipes() noexcept(false)
{
	auto plotsNum = plots.size();
	FILE *pipe;

	for (auto i = 0U; i < plotsNum; i++)
	{
#ifdef WIN32
		pipe = _popen(GNUPLOT, "w");
#else
		pipe = popen(GNUPLOT, "w");
#endif
		if (pipe == nullptr)
		{
			for (auto j = 0U; j < i; j++)
			{
#ifdef WIN32
				_pclose(pipe);
#else
				pclose(pipe);
#endif
			}
			throw PipeException();
		}

		pipes.insert(pipes.end(), pipe);
	}
}

void Plot::closePipes()
{
	for (FILE *pipe : pipes)
	{
#ifdef WIN32
		_pclose(pipe);
#else
		pclose(pipe);
#endif
	}
}

void Plot::writePipes()
{
	auto pipesNum = pipes.size();
	for (auto i = 0U; i < pipesNum; i++)
	{
		writePipe(pipes[i], plots[i]);
	}
}

void Plot::writePipe(FILE *pipe, PlotParams &plotParams)
{
	auto linesNum = plotParams.lines.size();

	if (linesNum == 0)
	{
		return;
	}

	fprintf(pipe, "set term wxt size %d, %d\n", plotParams.width, plotParams.height);
	fprintf(pipe, "set title \"%s\"\n", plotParams.title.c_str());
	fprintf(pipe, "set xlabel \"%s\"\n", plotParams.labelX.c_str());
	fprintf(pipe, "set ylabel \"%s\"\n", plotParams.labelY.c_str());
	if (plotParams.equalAxis)
	{
		fprintf(pipe, "set size ratio -1\n");
	}

	fprintf(pipe, "plot '-' with lines title '%s'", plotParams.lines[0].title.c_str());
	for (auto i = 1; i < linesNum; i++)
	{
		fprintf(pipe, ", '-' with lines title '%s'", plotParams.lines[i].title.c_str());
	}
	fprintf(pipe, "\n");

	for (const PlotLine &line : plotParams.lines)
	{
		for (const Vector2 &point : line.points)
		{
			fprintf(pipe, "%lf %lf\n", point.x, point.y);
		}
		fprintf(pipe, "e\n");
		fflush(pipe);
	}
}

void Plot::wait()
{
    system("pause");
}
