#include "Header.h"


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
