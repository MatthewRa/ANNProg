#include "Header.h"

/**************************************************************************
* Name:  main
*
* Params:  int argc - number of command line arguments
*		   char *argv[] - 2d array holding each command line argument
*
* Description:  Main function where the trainer class is instantiated and 
* the training method called.  The parameter file is also read in and stored 
* into a class to be passed into the ANN trainer.  As well as the parameter
* file being read in, the PDSI data file is also read in and stored.  After 
* the trainer is run main also writes out the weights to the weights file. 
**************************************************************************/
int main(int argc, char *argv[])
{
	ANNTrainer trainer;
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
	// Read in parameters from parameter file
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

	// trains the network and builds final weights list for testing
	trainer.TrainNetwork(csvFileReader, inputParams);

	// writes out final weights list to weight.wts
	weightWriter.writeOutWeights(trainer.weights ,inputParams);

	return 0;
}
