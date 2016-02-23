#include "Header.h"

/**************************************************************************
* Name:  main
*
* Params:  int argc - number of command line arguments
*		   char *argv[] - 2d array holding each command line argument
*
* Description:  Like the ANNTrainer and ANNTester main functions, this 
* houses the function calls for reading in the parameters as well as data.
* Then it calls the cross validate method to test the weights generated in
* ANNTrainer.
**************************************************************************/
int main(int argc, char *argv[])
{
	ANNCrossValidate crossValidate;
	InputParameters inputParams;
	CSVFileReader csvFileReader;
	ParameterFileReader paramFileReader;
	string parameterFileName;
	WeightsIO weightWriter;

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
	csvFileReader.RandomizeValues(inputParams);

	if (csvFileReader.NormalizedRecords.empty() == true ||
		csvFileReader.headings.empty() == true)
	{
		cout << "An error occurred while reading csv file, exiting program.";
		return -1;
	}

	// runs cross validation on a given csv file that matches formats
	crossValidate.CrossValidate(csvFileReader, inputParams);

	return 0;
}
