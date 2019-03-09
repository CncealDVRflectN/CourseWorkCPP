#ifndef COURSEWORKCPP_UTIL_H
#define COURSEWORKCPP_UTIL_H

#define _USE_MATH_DEFINES

#include <vector>
#include <math.h>
#include <limits>


#ifdef WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
#include <unistd.h>
    #define GetCurrentDir getcwd
#endif


// Vector2

typedef struct vector2_t
{
    double x;
    double y;
} Vector2;


static inline double norm(std::vector<double> &next, std::vector<double> &prev)
{
    double max = -std::numeric_limits<double>::infinity();
    double tmp;
    auto nextSize = next.size();

    for (int i = 0; i < nextSize; i++)
    {
        tmp = fabs(next[i] - prev[i]);
        if (tmp > max)
        {
            max = tmp;
        }
    }

    return max;
}

static inline void relaxation(std::vector<double> &nextApprox, const std::vector<double> &curApprox, double relaxationCoef)
{
	auto size = nextApprox.size();
	for (auto i = 0U; i < size; i++)
	{
		nextApprox[i] = curApprox[i] + relaxationCoef * (nextApprox[i] - curApprox[i]);
	}
}


// Args parsing

typedef struct short_opt_t
{
	char arg;
	int id;
} ShortOpt;

typedef struct long_opt_t
{
	char *arg;
	int id;
} LongOpt;


char* compareShortOpt(char *opt, const ShortOpt *shortOpt);

char* compareLongOpt(char *opt, const LongOpt *longOpt);

char* getShortOpt(char *opt, const ShortOpt *shortOpts, int &id);

char* getLongOpt(char *opt, const LongOpt *longOpts, int &id);

char* getOpt(char *arg, const ShortOpt *shortOpts, const LongOpt *longOpts, int &id);

#endif //COURSEWORKCPP_UTIL_H
