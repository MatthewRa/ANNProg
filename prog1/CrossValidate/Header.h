#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <cstdlib>

using namespace std;

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

bool CheckArguments(int argc);

struct InputParameters
{
	string ANNWeightFile;
	int TrainingEpochs;
	double LearnRate;
	double Momentum;
	double ErrorThreshold;
	int AdjustableLayerWeights;
	int NumberOfInputNodes;
	int NumberOfHiddenNodes;
	int NumberOfOutputNodes;
	string DataFileName;
	int BurnedAcreageYears;
	int MonthsOfPDSIData;
	int EndMonthOfYear;
	int NumberOfClasses;
	int FireSeverityLowCutoff;
	int FireSeverityHighCutoff;
};

class ParameterFileReader
{
	public:

		InputParameters ParseParameterFile(string fileName);
};
