#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include <cstdlib>

using namespace std;

bool CheckArguments(int argc);

class ParameterFileReader
{
	public:

		void ParseParameterFile(string fileName);
};

class CSVFileReader
{
	public:
		vector<vector<string>> records;
		vector<vector<string>> randRecords;
		vector<string> headings;
		string filename;
		vector<int> randRows;

		// ReadDataFile destroys the passed in vectors content
		void ReadDataFile(vector<vector<string>> &records, 
			vector<string> &headings, string filename);
		void GetRandValues(vector<vector<string>> &randRecords, \
			vector<vector<string>> records, int rowCount, vector<int> randRows, \
			int trainingSize);

	private:
		int getRows(string filename);

};
