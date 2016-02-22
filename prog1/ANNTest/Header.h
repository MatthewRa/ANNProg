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

// structure containing all the various input parameters
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


// class to read in the InputParameters structure from a given file
class ParameterFileReader
{
	public:

		InputParameters ParseParameterFile(string fileName);
};

/* class to read in the csv file and store the data into various vectors
   for training and testing */
class CSVFileReader
{
	public:
		vector<vector<double>> Records; // the records read from the file
		vector<vector<double>> NormalizedRecords;
		vector<vector<double>> RandRecords; // records in a random order
		vector<string> headings; // vector that holds csv file headings

		// ReadDataFile destroys the passed in vectors content
		void ReadDataFile(string filename);

		// randomize the values in records and stores in rand records for testing
		void RandomizeValues(InputParameters params);

	private:
		int GetRows(string filename); // gets number of rows in csv file 
		// finds the min and max burned acres
		void FindMinMax(double temp2, double &minBurned, double &maxBurned);

};

/*   WeightsIO handles the reading and writing of the weights file for the ANN */
class WeightsIO
{
	public:
		void readInWeights(vector<vector<vector<double>>> &weights, InputParameters params);
		void writeOutWeights(vector<vector<vector<double>>> &weights, InputParameters params);
};

/*  Neuron creates the nodes in the ANN. Each node contains the value and delta 
    error for that node.
	
*/
class Neuron
{
	public:
		double value; // value in node
		double deltaError; // nodes delta error

		double Sigmoid(double x);  // takes nodes value and does 1/(1-e^-x)
		double SigmoidPrime(double x); // takes sigmoid value and runs x(1-x)
};


/*  ANNTester contains all the functions needed to test the network using the 
    trained  weights */
class ANNTester
{
	public:
		vector<Neuron> inputLayer; 
		vector<vector<Neuron>> hiddenLayers;
		vector<Neuron> outputLayer;
		vector<vector<vector<double>>> weights;

		// runs testing on network given the data and input parameters
		void TestNetwork(CSVFileReader data, InputParameters params);
		// steps through the network - called in TestNetwork
		void ForwardPropagation(CSVFileReader data, InputParameters params);

	private:
		// generates the input layer for currently testing layer
		double GenerateInputLayer(CSVFileReader data, InputParameters params);
		// generates hidden for current test
		void GenerateHiddenLayers(InputParameters params);
		// generates outputs for current test run
		void GenerateOutputLayer(InputParameters params);

};


