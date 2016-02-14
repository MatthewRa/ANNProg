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

		void RandomizeValues(InputParameters params);

	private:
		int GetRows(string filename);
		void FindMinMax(double temp2, double &minBurned, double &maxBurned);

};

class Neuron
{
	public:
		double value;
		double deltaError;

		double Sigmoid(double x);
		double SigmoidPrime(double x);
};
class ANNTrainer
{
	public:
		vector<Neuron> inputLayer;
		vector<vector<Neuron>> hiddenLayers;
		vector<Neuron> outputLayer;
		vector<vector<vector<double>>> weights;

		void TrainNetwork(CSVFileReader data, InputParameters params);
		void readInWeights(InputParameters params);
		void writeOutWeights(InputParameters params);

	private:
		void GenerateInputLayer(CSVFileReader data, InputParameters params);
		void GenerateHiddenLayers(InputParameters params);
		void GenerateOutputLayer(InputParameters params);
		void InitializeWeights(InputParameters params);

};

