#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <cstdlib>
#include <algorithm>
#include <random>

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

class CSVFileReader
{
	public:
		vector<vector<double>> Records;
		vector<vector<double>> RandRecords;
		vector<string> headings;

		// ReadDataFile destroys the passed in vectors content
		void ReadDataFile(string filename);

		void RandomizeValues();

	private:
		int GetRows(string filename);
		void FindMinMax(double temp2, double &minBurned, double &maxBurned);

};

class ANNTrainer
{
	public:
		vector<vector<vector<double>>> weights;
		vector<double> inputLayer;
		vector<double> outputLayer;

		void TrainNetwork(InputParameters params);
		void readInWeights(InputParameters params);
		void writeOutWeights(InputParameters params);
};
