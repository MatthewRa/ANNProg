#include "Header.h"

/*
	RandomizeValues takes the records read in from the csv file and put them into
	a vector in a random order.
*/
void CSVFileReader::RandomizeValues(InputParameters params)
{
	RandRecords = NormalizedRecords; // set equal to normalized vector

	// take the ceiling of the months needed for the ANN
	int PDSIMonthsNeeded =  ceil(params.MonthsOfPDSIData / 12.0);
	// get burned years needed
	int BAYearsNeeded = params.BurnedAcreageYears;


	// drop the data that doesn't have enough data to be processed in the csv file
	if (PDSIMonthsNeeded > BAYearsNeeded)
	{
		RandRecords.erase(RandRecords.begin(), RandRecords.begin() + PDSIMonthsNeeded);
	}
	else
	{
		RandRecords.erase(RandRecords.begin(), RandRecords.begin() + BAYearsNeeded);
	}

	srand(time(0));
	// shuffle the records in randRecords to randomize data
	random_shuffle(begin(RandRecords), end(RandRecords));
}

/*
	GetRows gets the number of rows from the csv file
*/
int CSVFileReader::GetRows(string filename)
{
	ifstream csvFile;
	int rowCount = 0;
	string row = "";

	// test file open
	csvFile.open("../" + filename);
	if (!csvFile)
	{
		cout << "error opening csv file " << filename << endl;
		return -1;
	}

	while (getline(csvFile,row))
	{
		// for each row increment a row counter
		rowCount++;
	}

	return rowCount - 2; // drop title and headings rows
}

/*
	 reads in the csv values to the records array. Not used in testing 
*/
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
	csvFile.open("../" + filename);
	if (!csvFile)
	{
		cout << "error opening csv file " << filename << endl;
		return; // on return must check if vector is empty
	}

	//empty the passed in vectors
	NormalizedRecords.clear();
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

	NormalizedRecords = Records;
	// normalize record vectors and store in new vector
	for (int row = 0; row < NormalizedRecords.size(); row++)
	{
		for (int column = 1; column < toBeInserted.size(); column++)
		{
			if (column == 1)
			{
				double normalizedValue = (NormalizedRecords[row][column] - minBurned) / (maxBurned - minBurned);

				NormalizedRecords[row][column] = normalizedValue;
			}
			else
			{
				double normalizedValue = (NormalizedRecords[row][column] - minPDSI) / (maxPDSI - minPDSI);

				NormalizedRecords[row][column] = normalizedValue;
			}
		}
	}

	csvFile.close();
}

/*
	FindMinMax finds the min and max burned acres in records
*/
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