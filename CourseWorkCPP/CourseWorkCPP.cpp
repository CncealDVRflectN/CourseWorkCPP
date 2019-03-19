#include <iostream>
#include "fluid/nonparametric/AxisymmetricNonParam.h"
#include "fluid/nonparametric/PlainNonParam.h"
#include "fluid/parametric/AxisymmetricParam.h"
#include "plot/Plot.h"
#include "fluid/parametric/PlainParam.h"


enum optIds { WIDTH_OPT_ID = 1, HEIGHT_OPT_ID, ANGLE_OPT_ID, BOND_OPT_ID, STEP_OPT_ID, RELAXATION_COEF_OPT_ID, 
			  ACCURACY_OPT_ID, SPLITS_OPT_ID, ITERATIONS_OPT_ID, RESULTS_OPT_ID, NONDIM_OPT_ID, EQUAL_OPT_ID, FORCE_OPT_ID, 
			  REVERSE_PARAM_OPT_ID, AXISYM_OPT_ID, PLAIN_OPT_ID, AXISYM_PARAM_OPT_ID, PLAIN_PARAM_OPT_ID, HEIGHT_COEFS_OPT_ID };


typedef struct program_params_t
{
	unsigned int width;
	unsigned int height;
	bool equalAxis;
	bool force;
	bool axisym;
	bool plain;
	bool axisymParam;
	bool plainParam;
	bool reverseParam;
	bool heightCoefs;
} ProgramParams;


void handleOpt(int optId, char *optPtr, RelaxationParams &params, ProgramParams &programParams) noexcept(false)
{
	switch (optId)
	{
		case WIDTH_OPT_ID:
			programParams.width = (unsigned int)std::atoi(optPtr);
			break;
		case HEIGHT_OPT_ID:
			programParams.height = (unsigned int)std::atoi(optPtr);
			break;
		case ANGLE_OPT_ID:
			params.alpha = std::atof(optPtr) * M_PI / 180.0;
			break;
		case BOND_OPT_ID:
			params.targetBond = std::atof(optPtr);
			break;
		case STEP_OPT_ID:
			params.bondStep = std::atof(optPtr);
			break;
		case RELAXATION_COEF_OPT_ID:
			params.relaxationCoefMin = std::atof(optPtr);
			break;
		case ACCURACY_OPT_ID:
			params.epsilon = std::atof(optPtr);
			break;
		case SPLITS_OPT_ID:
			params.splitNum = (unsigned int)std::atoi(optPtr);
			break;
		case ITERATIONS_OPT_ID:
			params.iterationsNumMax = (unsigned int)std::atoi(optPtr);
			break;
		case RESULTS_OPT_ID:
			params.resultsNum = (unsigned int)std::atoi(optPtr);
			break;
		case NONDIM_OPT_ID:
			params.volumeNondim = true;
			break;
		case EQUAL_OPT_ID:
			programParams.equalAxis = true;
			break;
		case FORCE_OPT_ID:
			programParams.force = true;
			break;
		case REVERSE_PARAM_OPT_ID:
			programParams.reverseParam = true;
			break;
		case AXISYM_OPT_ID:
			programParams.axisym = true;
			break;
		case AXISYM_PARAM_OPT_ID:
			programParams.axisymParam = true;
			break;
		case PLAIN_OPT_ID:
			programParams.plain = true;
			break;
		case PLAIN_PARAM_OPT_ID:
			programParams.plainParam = true;
			break;
		case HEIGHT_COEFS_OPT_ID:
			programParams.heightCoefs = true;
			break;
	}
}


void parseArgs(int argc, char *argv[], RelaxationParams &params, ProgramParams &programParams) noexcept(false)
{
	int optId = 0;
	char *optPtr = nullptr;

	const LongOpt longOpts[] = {
		{"width",			WIDTH_OPT_ID},
		{"height",			HEIGHT_OPT_ID},
		{"angle",			ANGLE_OPT_ID},
		{"bond",			BOND_OPT_ID},
		{"step",			STEP_OPT_ID},
		{"relax",			RELAXATION_COEF_OPT_ID},
		{"accuracy",		ACCURACY_OPT_ID},
		{"splits",			SPLITS_OPT_ID},
		{"iterations",		ITERATIONS_OPT_ID},
		{"results",			RESULTS_OPT_ID},
		{"nondim",			NONDIM_OPT_ID},
		{"equal",			EQUAL_OPT_ID},
		{"force",			FORCE_OPT_ID},
		{"reverse-param",   REVERSE_PARAM_OPT_ID},
		{"axisym",			AXISYM_OPT_ID},
		{"plain",			PLAIN_OPT_ID},
		{"axisym-param",	AXISYM_PARAM_OPT_ID},
		{"plain-param",		PLAIN_PARAM_OPT_ID},
		{"height-coefs",	HEIGHT_COEFS_OPT_ID},
		{nullptr,			0}
	};

	for (int i = 0; i < argc; i++)
	{
		optPtr = getOpt(argv[i], nullptr, longOpts, optId);
		if (optPtr != nullptr)
		{
			handleOpt(optId, optPtr, params, programParams);
		}
		else
		{
			printf("Unknown parameter: %s \n", argv[i]);
		}
	}
}


void logAction(char *action)
{
	printf("\n---------- Done %s ----------\n\n", action);
}


void setupResultsPlotParams(const std::vector<Result> &results, const std::string &title ,PlotParams &plotParams)
{
	std::vector<Result>::const_iterator iter;
	char buffer[256];

	sprintf(buffer, "%s (a = %.1lf)", title.c_str(), 180.0 * results[0].alpha / M_PI);
	plotParams.title = buffer;
	for (iter = results.begin(); iter != results.end(); ++iter)
	{
		sprintf(buffer, "Bo = %.2lf", iter->bond);
		plotParams.lines.insert(plotParams.lines.end(), {iter->points, buffer});
	}
}


PlotLine createHeightsPlotLine(const std::vector<Vector2> &heights, char *title)
{
	return { heights, title };
}


int main(int argc, char *argv[])
{
	ProgramParams programParams = { 1280U, 720U, false, false, false, false, false, false, false, false };
	RelaxationParams params = { 0.0, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, false };
	Plot plot;
	PlotParams heightsPlotParams;

	parseArgs(argc, argv, params, programParams);
	logAction("parsing arguments");

	if (programParams.plainParam)
	{
		PlainParam axisymmetricParam;
		std::vector<Vector2> heights;
		std::vector<Result> *results;
		std::string title;
		PlotParams plotParams;
		plotParams.width = programParams.width;
		plotParams.height = programParams.height;
		plotParams.equalAxis = programParams.equalAxis;
		plotParams.labelX = "x";
		plotParams.labelY = "y";

		if (programParams.reverseParam)
		{
			title = "Плоский параметрический XY случай";

			if (programParams.heightCoefs)
			{
				results = axisymmetricParam.calcRelaxationYX(params, &heights);
				heightsPlotParams.lines.insert(heightsPlotParams.lines.end(), { heights, title });
			}
			else
			{
				results = axisymmetricParam.calcRelaxationYX(params, nullptr);
			}

			setupResultsPlotParams(*results, title, plotParams);
		}
		else
		{
			title = "Плоский параметрический XY случай";

			if (programParams.heightCoefs)
			{
				results = axisymmetricParam.calcRelaxationXY(params, &heights);
				heightsPlotParams.lines.insert(heightsPlotParams.lines.end(), { heights, title });
			}
			else
			{
				results = axisymmetricParam.calcRelaxationXY(params, nullptr);
			}

			setupResultsPlotParams(*results, title, plotParams);
		}



		plot.addPlot(plotParams);

		delete results;

		logAction("calculating plain parametric problem");
	}

	/*if (programParams.axisym && ((params.alpha > 0.0 && (180.0 * params.alpha / M_PI < 77.0)) || programParams.force))
	{
		AxisymmetricNonParam axisymmetric;
		std::vector<Vector2> heights;
		std::vector<Result> *results;
		std::string title = "Осесимметричный случай";
		PlotParams plotParams;
		plotParams.width = programParams.width;
		plotParams.height = programParams.height;
		plotParams.equalAxis = programParams.equalAxis;
		plotParams.labelX = "r";
		plotParams.labelY = "z";

		if (programParams.heightCoefs)
		{
			results = axisymmetric.calcRelaxation(params, &heights);
			heightsPlotParams.lines.insert(heightsPlotParams.lines.end(), {heights, title});
		}
		else
		{
			results = axisymmetric.calcRelaxation(params, nullptr);
		}

		setupResultsPlotParams(*results, title, plotParams);
		plot.addPlot(plotParams);

		delete results;

		logAction("calculating axisymmetric problem");
	}*/

	if (programParams.plain && ((params.alpha > 0.0 && (180.0 * params.alpha / M_PI < 77.0)) || programParams.force))
	{
		PlainNonParam plain;
		std::vector<Vector2> heights;
		std::vector<Result> *results;
		std::string title = "Плоский случай";
		PlotParams plotParams;
		plotParams.width = programParams.width;
		plotParams.height = programParams.height;
		plotParams.equalAxis = programParams.equalAxis;
		plotParams.labelX = "x";
		plotParams.labelY = "y";

		if (programParams.heightCoefs)
		{
			results = plain.calcRelaxation(params, &heights);
			heightsPlotParams.lines.insert(heightsPlotParams.lines.end(), { heights, title });
		}
		else
		{
			results = plain.calcRelaxation(params, nullptr);
		}

		setupResultsPlotParams(*results, title, plotParams);
		plot.addPlot(plotParams);

		delete results;

		logAction("calculating plain problem");
	}

	/*if (programParams.axisymParam)
	{
		AxisymmetricParam axisymmetricParam;
		std::vector<Vector2> heights;
		std::vector<Result> *results;
		std::string title;
		PlotParams plotParams;
		plotParams.width = programParams.width;
		plotParams.height = programParams.height;
		plotParams.equalAxis = programParams.equalAxis;
		plotParams.labelX = "r";
		plotParams.labelY = "z";

		if (programParams.reverseParam)
		{
			title = "Осесимметричный параметрический ZR случай";

			if (programParams.heightCoefs)
			{
				results = axisymmetricParam.calcRelaxationYX(params, &heights);
				heightsPlotParams.lines.insert(heightsPlotParams.lines.end(), { heights, title });
			}
			else
			{
				results = axisymmetricParam.calcRelaxationYX(params, nullptr);
			}

			setupResultsPlotParams(*results, title, plotParams);
		}
		else
		{
			title = "Осесимметричный параметрический RZ случай";

			if (programParams.heightCoefs)
			{
				results = axisymmetricParam.calcRelaxationXY(params, &heights);
				heightsPlotParams.lines.insert(heightsPlotParams.lines.end(), { heights, title });
			}
			else
			{
				results = axisymmetricParam.calcRelaxationXY(params, nullptr);
			}

			setupResultsPlotParams(*results, title, plotParams);
		}



		plot.addPlot(plotParams);

		delete results;

		logAction("calculating axisymmetric parametric problem");
	}	*/	

	if (programParams.heightCoefs)
	{
		heightsPlotParams.width = programParams.width;
		heightsPlotParams.height = programParams.height;
		heightsPlotParams.equalAxis = false;
		heightsPlotParams.labelX = "Bo";
		heightsPlotParams.labelY = "k";
		heightsPlotParams.title = "Коэффициент сжатия";
		plot.addPlot(heightsPlotParams);
	}

	plot.plot();
	plot.clear();

	logAction("plotting");

	return 0;
}
