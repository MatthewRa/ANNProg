#include "Header.h"


int main(int argc, char *argv[])
{
	InputParameters inputParams;
	CSVFileReader csvFileReader;
	ParameterFileReader paramFileReader;
	string parameterFileName;

	if (argc != 2)
	{
		return 0;
	}

	if (argv[1] != NULL)
	{
		parameterFileName = string(argv[1]);
	}

	inputParams = paramFileReader.ParseParameterFile(parameterFileName);

	// Read in data from csv file to a 2D vector and normalizes data
	csvFileReader.ReadDataFile(inputParams.DataFileName);

	// Randomize read in data into a new vector
	csvFileReader.RandomizeValues();

	if (csvFileReader.records.empty() == true ||
		csvFileReader.headings.empty() == true)
	{
		cout << "An error occurred while reading csv file, exiting program.";
		return -1;
	}

	return 0;
}
