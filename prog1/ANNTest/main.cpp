#include "Header.h"


int main(int argc, char *argv[])
{
	InputParameters inputParams;  // list of input parameters 
	CSVFileReader csvFileReader; // class to parse csv file
	ParameterFileReader paramFileReader; // class for reading inputs
	string parameterFileName; // csv file to open
	WeightsIO weightWriter; // class for reading and writing files
	ANNTester tester; // ANN test class

	// check args 
	if (argc != 2)
	{
		return 0;
	}

	// check for parameter file name
	if (argv[1] != NULL)
	{
		parameterFileName = string(argv[1]);
	}

	// read in the data from the parameter file
	inputParams = paramFileReader.ParseParameterFile(parameterFileName);

	// Read in data from csv file to a 2D vector and normalizes data
	csvFileReader.ReadDataFile(inputParams.DataFileName);

	// Randomize read in data into a new vector
	csvFileReader.RandomizeValues(inputParams);

	if (csvFileReader.NormalizedRecords.empty() == true ||
		csvFileReader.headings.empty() == true)
	{
		cout << "An error occurred while reading csv file, exiting program.";
		return -1;
	}

	tester.TestNetwork(csvFileReader, inputParams);

	return 0;
}
