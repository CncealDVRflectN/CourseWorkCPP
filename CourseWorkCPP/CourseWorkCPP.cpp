#include <iostream>
#include "fluid/Axisymmetric.h"
#include "fluid/Plain.h"
#include "plot/Plot.h"


enum optIds {WIDTH_ID = 1, HEIGHT_ID, ANGLE_ID, BOND_ID, STEP_ID, RELAXATION_COEF_ID, 
			ACCURACY_ID, SPLITS_ID, ITERATIONS_ID, RESULTS_ID, NONDIM_ID, EQUAL_ID};


void handleOpt(int optId, char *optPtr, RelaxationParams *params, 
			   unsigned int &width, unsigned int &height, bool &equalAxis) noexcept(false)
{
	switch (optId)
	{
		case WIDTH_ID:
			width = (unsigned int)std::atoi(optPtr);
			break;
		case HEIGHT_ID:
			height = (unsigned int)std::atoi(optPtr);
			break;
		case ANGLE_ID:
			params->alpha = std::atof(optPtr) * M_PI / 180.0;
			break;
		case BOND_ID:
			params->targetBond = std::atof(optPtr);
			break;
		case STEP_ID:
			params->bondStep = std::atof(optPtr);
			break;
		case RELAXATION_COEF_ID:
			params->relaxationCoefMin = std::atof(optPtr);
			break;
		case ACCURACY_ID:
			params->epsilon = std::atof(optPtr);
			break;
		case SPLITS_ID:
			params->splitNum = (unsigned int)std::atoi(optPtr);
			break;
		case ITERATIONS_ID:
			params->iterationsNumMax = (unsigned int)std::atoi(optPtr);
			break;
		case RESULTS_ID:
			params->resultsNum = (unsigned int)std::atoi(optPtr);
			break;
		case NONDIM_ID:
			params->volumeNondim = true;
			break;
		case EQUAL_ID:
			equalAxis = true;
			break;
	}
}


void parseArgs(int argc, char *argv[], RelaxationParams *params,
	unsigned int &width, unsigned int &height, bool &equalAxis) noexcept(false)
{
	int optId = 0;
	char *optPtr = nullptr;

	const LongOpt longOpts[] = {
		{"width",      WIDTH_ID},
		{"height",     HEIGHT_ID},
		{"angle",      ANGLE_ID},
		{"bond",       BOND_ID},
		{"step",       STEP_ID},
		{"relax",      RELAXATION_COEF_ID},
		{"accuracy",   ACCURACY_ID},
		{"splits",     SPLITS_ID},
		{"iterations", ITERATIONS_ID},
		{"results",    RESULTS_ID},
		{"nondim",     NONDIM_ID},
		{"equal",      EQUAL_ID},
		{nullptr,      0}
	};

	for (int i = 0; i < argc; i++)
	{
		optPtr = getOpt(argv[i], nullptr, longOpts, optId);
		if (optPtr != nullptr)
		{
			handleOpt(optId, optPtr, params, width, height, equalAxis);
		}
		else
		{
			printf("Unknown parameter: %s", argv[i]);
		}
	}
}


int main(int argc, char *argv[])
{
	RelaxationParams params = { 0.0, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, false };
	bool equalAxis = false;
	unsigned int width = 1280;
	unsigned int height = 720;
	parseArgs(argc, argv, &params, width, height, equalAxis);

	Axisymmetric axisymmetric;
	Plain plain;
	std::vector<Vector2> axisymHeights;
	std::vector<Vector2> plainHeights;
	std::vector<Result> *resultsAxi = axisymmetric.calcRelaxation(params, &axisymHeights);
	std::vector<Result> *resultsPlain = plain.calcRelaxation(params, &plainHeights);

	Plot plot(*resultsAxi, *resultsPlain, axisymHeights, plainHeights);
	plot.plot(width, height, equalAxis);

	delete resultsAxi;
	delete resultsPlain;

	return 0;
}
