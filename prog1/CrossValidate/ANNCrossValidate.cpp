#include "Header.h"

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
		for (int randIndexer = 0; randIndexer < data.RandRecords.size(); randIndexer++)
		{
			if (data.RandRecords[randIndexer][0] == removedYear)
			{
				data.RandRecords.erase(data.RandRecords.begin() + randIndexer, data.RandRecords.begin() + randIndexer + 1);
				cout << "Crossvalidating... Removed  year " << removedYear << "." << endl;
				break;
			}
		}

		trainer.TrainNetwork(data, params);

		weightsIO.writeOutWeights(trainer.weights, params);

		correct += tester.TestNetwork(data, params, removedYear);
	}

	cout << "Crossvalidate Acuracy: " << (double(correct / data.RandRecords.size())*100.0) << "%" << endl;
}