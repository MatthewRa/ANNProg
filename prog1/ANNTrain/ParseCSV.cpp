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
	vector<string> &headings, string filename)
{
	ifstream csvFile;
	string currentRow;
	int csvRows = getRows(filename);
	

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
		// clear the vector or vector maintains pervious iteration as well 
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
		// push processing vector onto final records vector
		records.push_back(toBeInserted);
	}

	// prints out the vectors for testing
	
	for (std::vector<std::vector<int>>::size_type i = 0; i < records.size(); i++)
	{
		cout << "Vector row number: " << i << endl;
		for (std::vector<std::vector<int>>::size_type j = 0; j < records[i].size(); j++)
			cout << records[i][j] << ' ';
		cout << endl << endl;
	}
	

	csvFile.close();
}