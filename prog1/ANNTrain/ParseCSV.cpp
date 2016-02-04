#include "Header.h"

void CSVFileReader::GetRandValues(vector<vector<string>> &randRecords, \
	vector<vector<string>> records, int rowCount, vector<int> randRows, \
	int trainingSize)
{
	int value = 0;

	for (int i = 0; i < trainingSize; i++)
	{
		value = rand() % rowCount - 1;
		randRows.push_back(value);
		randRecords.push_back(records[value]);
	}


}

int CSVFileReader::getRows(string filename)
{
	ifstream csvFile;
	int rowCount = 0;
	string row = "";

	csvFile.open(filename);
	if (!csvFile)
	{
		cout << "error opening csv file " << filename << endl;
		return -1;
	}

	while (getline(csvFile,row))
	{
		rowCount++;
	}

	return rowCount - 2; // drop title and headings rows
}

void CSVFileReader::ReadDataFile(vector<vector<string>> &records,
	vector<vector<string>> normalizedRecords,
	vector<string> &headings, string filename)
{
	ifstream csvFile;
	string currentRow;
	int csvRows = getRows(filename);
	double minBurned = 0;
	double maxBurned = 0;
	

	// open and error check file
	csvFile.open(filename);
	if (!csvFile)
	{
		cout << "error opening csv file " << filename << endl;
		return; // on return must check if vector is empty
	}

	//empty the passed in vectors
	records.clear();
	headings.clear();

	getline(csvFile, currentRow); // ignore file heading line


	getline(csvFile, currentRow); // get heading for data

	// stringstream allows for easy tokenizing of rows in csv file
	stringstream streamcurrentRow(currentRow);
	string insert; // storage for the results as string is tokenized

	// As each heading is found push onto heading vector
	while (getline(streamcurrentRow, insert, ','))
	{
		headings.push_back(insert);
	}

	// vector for processing before being pushed onto records vector
	vector<string> toBeInserted;

	for (int row = 0; row < csvRows; row++)
	{
		// clear the vector or vector maintains previous iteration as well 
		toBeInserted.clear(); 
		getline(csvFile, currentRow);

		// set stringstream to new row
		streamcurrentRow.clear();
		streamcurrentRow.str(currentRow);

		// push each value in the row onto the processing vector
		while (getline(streamcurrentRow, insert, ','))
		{
			toBeInserted.push_back(insert);
		}

		// check for min and max burned anchorage
		const char* temp = toBeInserted[1].c_str();
		double temp2 = atoi(temp);
		findminMax(temp2, minBurned, maxBurned);

		// push processing vector onto final records vector
		records.push_back(toBeInserted);

	}

	// normalize record vectors and store in new vector
	for (int rows = 0; rows < records.size(); rows++)
	{

		//normalize the processing vector
		normalize(toBeInserted, minBurned, maxBurned);
		normalizedRecords.push_back(toBeInserted);
	}

	// prints out the vectors for testing
	/*
	for (std::vector<std::vector<int>>::size_type i = 0; i < records.size(); i++)
	{
		cout << "Vector row number: " << i << endl;
		for (std::vector<std::vector<int>>::size_type j = 0; j < records[i].size(); j++)
			cout << records[i][j] << ' ';
		cout << endl << endl;
	}

	for (std::vector<std::vector<int>>::size_type i = 0; i < normalizedRecords.size(); i++)
	{
		cout << "Normalized vector row number: " << i << endl;
		for (std::vector<std::vector<int>>::size_type j = 0; j < normalizedRecords[i].size(); j++)
			cout << normalizedRecords[i][j] << ' ';
		cout << endl << endl;
	}
	*/

	csvFile.close();
}

void CSVFileReader::normalize(vector<string> &toBeInserted,double minBurned,
	double maxBurned)
{
	double minPredicted = -10;
	double maxPredicted = 10;

	const char* temp = toBeInserted[1].c_str();
	double temp2 = atoi(temp);

	double normalBurn = (temp2 - minBurned) / (maxBurned - minBurned);
	// next line taken from http://www.cplusplus.com/articles/D9j2Nwbp/
	string normalizedString = static_cast<ostringstream*>(&(ostringstream() << normalBurn))->str();
	toBeInserted[1] = normalizedString;

	double temp2 = 0;
	double normalPredicted = 0;

	for (int values = 2; values < 14; values++)
	{
		const char* temp = toBeInserted[values].c_str();
		temp2 = atoi(temp);
		normalPredicted = (temp2 - minBurned) / (maxBurned - minBurned);
		// next line taken from http://www.cplusplus.com/articles/D9j2Nwbp/
		normalizedString = static_cast<ostringstream*>(&(ostringstream() << normalPredicted))->str();
		toBeInserted[values] = normalizedString;

	}
}

void CSVFileReader::findminMax(double temp2, double &minBurned, double &maxBurned)
{
	if (temp2 < minBurned)
	{
		minBurned = temp2;
	}
	if (temp2 > maxBurned)
	{
		maxBurned = temp2;
	}


}