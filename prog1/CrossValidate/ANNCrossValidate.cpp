#include "Header.h"

/*************************************************************************
* Name:  CrossValidate
* Params: CSVFileReader data - input data from the PDSI
*		  InputParameters params - List of parameters from the parameter file
*
* Description:  The purpose of CrossValidate is to take one PDSI data point
* out of the testing data and run ANNtrain on the remaining years.  After
* the weights are trained, then ANNtest is run on the removed year to check
* accuracy of the network.  
*
*************************************************************************/
void ANNCrossValidate::CrossValidate(CSVFileReader data, InputParameters params)
{
	ANNTester tester;
	ANNTrainer trainer;
	WeightsIO weightsIO;

	int PDSIMonthsNeeded = ceil(params.MonthsOfPDSIData / 12.0);
	int BAYearsNeeded = params.BurnedAcreageYears;
	int firstYearIndex = 0;
	int correct = 0;

	// Determine how many years I should avoid removing (has no affect)
	if (PDSIMonthsNeeded > BAYearsNeeded)
	{
		firstYearIndex = PDSIMonthsNeeded;
	}
	else
	{
		firstYearIndex = BAYearsNeeded;
	}

	for (int index = firstYearIndex; index < data.NormalizedRecords.size(); index++)
	{
		// Determine year to be removed from rand list
		int removedYear = data.NormalizedRecords[index][0];

		data.RandomizeValues(params);
		// Removes the selected year
		for (int randIndexer = 0; randIndexer < data.RandRecords.size(); randIndexer++)
		{
			if (data.RandRecords[randIndexer][0] == removedYear)
			{
				data.RandRecords.erase(data.RandRecords.begin() + randIndexer, data.RandRecords.begin() + randIndexer + 1);
				cout << "Crossvalidating... Removed  year " << removedYear << "." << endl;
				break;
			}
		}

		// Calls ANN trainer
		trainer.TrainNetwork(data, params);

		// Writes out weights file
		weightsIO.writeOutWeights(trainer.weights, params);

		// Keeps track of how many correct tests the network has done
		correct += tester.TestNetwork(data, params, removedYear);
	}

	cout << "Crossvalidate Accuracy: " << (double(correct / data.RandRecords.size())*100.0) << "%" << endl;
}