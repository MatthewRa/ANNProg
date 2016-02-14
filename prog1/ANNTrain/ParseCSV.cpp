#include "Header.h"

void CSVFileReader::RandomizeValues(InputParameters params)
{
	RandRecords = Records;

	int PDSIMonthsNeeded =  ceil(params.MonthsOfPDSIData / 12.0);
	int BAYearsNeeded = params.BurnedAcreageYears;

	if (PDSIMonthsNeeded > BAYearsNeeded)
	{
		RandRecords.erase(RandRecords.begin(), RandRecords.begin() + PDSIMonthsNeeded);
	}
	else
	{
		RandRecords.erase(RandRecords.begin(), RandRecords.begin() + BAYearsNeeded);
	}

	srand(time(0));

	random_shuffle(begin(RandRecords), end(RandRecords));
}

int CSVFileReader::GetRows(string filename)
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

void CSVFileReader::ReadDataFile(string filename)
{
	ifstream csvFile;
	string currentRow;
	int csvRows = GetRows(filename);
	double minBurned = 0;
	double maxBurned = 0;
	double minPDSI = 0;
	double maxPDSI = 0;
	bool firstValue = true;
	

	// open and error check file
	csvFile.open(filename);
	if (!csvFile)
	{
		cout << "error opening csv file " << filename << endl;
		return; // on return must check if vector is empty
	}

	//empty the passed in vectors
	Records.clear();
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
	vector<double> toBeInserted;

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
			const char* temp = insert.c_str();

			toBeInserted.push_back(atof(temp));
		}

		// check for min and max burned anchorage
		FindMinMax(toBeInserted[1], minBurned, maxBurned);

		// find min and max PDSI
		for (int index = 2; index < toBeInserted.size(); index++)
		{
			if (firstValue)
			{
				minPDSI = toBeInserted[index];
				maxPDSI = toBeInserted[index];
				firstValue = false;
			}
			FindMinMax(toBeInserted[index], minPDSI, maxPDSI);
		}

		// push processing vector onto final records vector
		Records.push_back(toBeInserted);

	}

	// normalize record vectors and store in new vector
	for (int row = 0; row < Records.size(); row++)
	{
		for (int column = 1; column < toBeInserted.size(); column++)
		{
			if (column == 1)
			{
				double normalizedValue = (Records[row][column] - minBurned) / (maxBurned - minBurned);

				Records[row][column] = normalizedValue;
			}
			else
			{
				double normalizedValue = (Records[row][column] - minPDSI) / (maxPDSI - minPDSI);

				Records[row][column] = normalizedValue;
			}
		}
	}

	csvFile.close();
}

void CSVFileReader::FindMinMax(double temp2, double &min, double &max)
{
	if (temp2 <= min)
	{
		min = temp2;
	}
	if (temp2 >= max)
	{
		max = temp2;
	}
}