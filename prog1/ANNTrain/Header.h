#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

bool CheckArguments(int argc);

class ParameterFileReader
{
	public:
		void ParseParameterFile();
};

class CSVFileReader
{
	public:
		vector<vector<string>> records;
		vector<string> headings;
		string filename;

		// ReadDataFile destroys the passed in vectors content
		void ReadDataFile(vector<vector<string>> &records, 
			vector<string> &headings, string filename);

	private:
		int getRows(string filename);

};
