#include "Plot.h"
#include "exceptions/PipeException.h"
#include "exceptions/EmptyDataException.h"

#define GNUPLOT             "gnuplot"

Plot::Plot(std::vector<Result> &axisymData, std::vector<Result> &plainData,
        std::vector<Vector2> &axisymHeights, std::vector<Vector2> &plainHeights) :
        axisymData(axisymData), plainData(plainData), axisymHeights(axisymHeights), plainHeights(plainHeights)
{

}

void Plot::plot(unsigned int width, unsigned int height, bool equalAxis) noexcept(false)
{
    openPipes();
    setupAxisymPipe(width, height, equalAxis);
    setupPlainPipe(width, height, equalAxis);
    setupHeightCoefsPipe(width, height);
    wait();
    closePipes();
}



void Plot::openPipes() noexcept(false)
{
#ifdef WIN32
    axisymPipe = _popen(GNUPLOT, "w");
    plainPipe = _popen(GNUPLOT, "w");
    heightCoefsPipe = _popen(GNUPLOT, "w");
#else
    axisymPipe = popen(GNUPLOT, "w");
    plainPipe = popen(GNUPLOT, "w");
    heightCoefsPipe = popen(GNUPLOT, "w");
#endif

    if (axisymPipe == nullptr || plainPipe == nullptr || heightCoefsPipe == nullptr)
    {
        throw PipeException();
    }
}

void Plot::closePipes()
{
#ifdef WIN32
    _pclose(axisymPipe);
    _pclose(plainPipe);
    _pclose(heightCoefsPipe);
#else
    pclose(axysimPipe);
    pclose(plainPipe);
    pclose(heightCoefsPipe);
#endif
}

void Plot::setupAxisymPipe(unsigned int width, unsigned int height, bool equalAxis) noexcept(false)
{
    auto size = axisymData.size();

    if (size == 0)
    {
        throw EmptyDataException();
    }

    fprintf(axisymPipe, "set term wxt size %d, %d\n", width, height);
    fprintf(axisymPipe, "set title \"Осесимметричный случай (a = %lf)\"\n", axisymData.front().alpha);
    fprintf(axisymPipe, "set xlabel \"r\"\n");
    fprintf(axisymPipe, "set ylabel \"z\"\n");
    if (equalAxis)
    {
        fprintf(axisymPipe, "set size ratio -1\n");
    }

    fprintf(axisymPipe, "plot '-' with lines title 'Bo = %lf'", axisymData.front().bond);
    for (auto i = 1; i < size; i++)
    {
        fprintf(axisymPipe, ", '-' with lines title 'Bo = %lf'", axisymData[i].bond);
    }
    fprintf(axisymPipe, "\n");

    uploadFluidData(axisymPipe, axisymData);
}

void Plot::setupPlainPipe(unsigned int width, unsigned int height, bool equalAxis) noexcept(false)
{
    auto size = plainData.size();

    if (size == 0)
    {
        throw EmptyDataException();
    }

    fprintf(plainPipe, "set term wxt size %d, %d\n", width, height);
    fprintf(plainPipe, "set title \"Плоский случай (a = %lf)\"\n", plainData.front().alpha);
    fprintf(plainPipe, "set xlabel \"x\"\n");
    fprintf(plainPipe, "set ylabel \"y\"\n");
    if (equalAxis)
    {
        fprintf(plainPipe, "set size ratio -1\n");
    }

    fprintf(plainPipe, "plot '-' with lines title 'Bo = %lf'", plainData.front().bond);
    for (auto i = 1; i < size; i++)
    {
        fprintf(plainPipe, ", '-' with lines title 'Bo = %lf'", plainData[i].bond);
    }
    fprintf(plainPipe, "\n");

    uploadFluidData(plainPipe, plainData);
}

void Plot::setupHeightCoefsPipe(unsigned int width, unsigned int height) noexcept(false)
{
    fprintf(heightCoefsPipe, "set term wxt size %d, %d\n", width, height);
    fprintf(heightCoefsPipe, "set title \"Коэффициенты сжатия (a = %lf)\"\n", axisymData.front().alpha);
    fprintf(heightCoefsPipe, "set xlabel \"Bo\"\n");
    fprintf(heightCoefsPipe, "set ylabel \"k\"\n");

    fprintf(heightCoefsPipe, "plot '-' with lines title 'Осесимметричный случай', "
                             "'-' with lines title 'Плоский случай'\n");

    uploadHeightCoefsData(heightCoefsPipe, axisymHeights);
    uploadHeightCoefsData(heightCoefsPipe, plainHeights);
}

void Plot::wait()
{
    system("pause");
}

void Plot::uploadFluidData(FILE *pipe, std::vector<Result> &data)
{
    for (const Result &result : data)
    {
        for (const Vector2 &point : result.points)
        {
            fprintf(pipe, "%lf %lf\n", point.x, point.y);
        }
        fprintf(pipe, "e\n");
        fflush(pipe);
    }
}

void Plot::uploadHeightCoefsData(FILE *pipe, std::vector<Vector2> &heightsCoefsData)
{
    for (const Vector2 &result : heightsCoefsData)
    {
        fprintf(pipe, "%lf %lf\n", result.x, result.y);
    }
    fprintf(pipe, "e\n");
    fflush(pipe);
}
