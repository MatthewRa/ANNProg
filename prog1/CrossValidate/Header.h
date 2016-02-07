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

class CSVFileReader
{
	public:
		vector<vector<string>> records;
		vector<vector<string>> randRecords;
		vector<vector<string>> normalizedRecords;
		vector<string> headings;
		string filename;
		vector<int> randRows;

		// ReadDataFile destroys the passed in vectors content
		void ReadDataFile(vector<vector<string>> &records, 
			vector<vector<string>> normalizedRecords,
			vector<string> &headings, string filename);

		void GetRandValues(vector<vector<string>> &randRecords, \
			vector<vector<string>> records, int rowCount, vector<int> randRows, \
			int trainingSize);

		void normalize(vector<string> &toBeInserted, double minBurned,
			double maxBurned);

	private:
		int getRows(string filename);
		void findminMax(double temp2, double &minBurned, double &maxBurned);

};
