#include "Header.h"

void ANNTester::TestNetwork(CSVFileReader data, InputParameters params)
{
	int leftNodes = params.NumberOfInputNodes + 1;
	WeightsIO weightsReader;
	vector<int> encodedDesired;
	vector<int> actualVector;
	double desired;
	double calculatedValue = 0;
	bool correct = true;
	double numberCorrect = 0.0;

	GenerateHiddenLayers(params);
	GenerateOutputLayer(params);

	weightsReader.readInWeights(weights, params);

	for (int recordIndex = 0; recordIndex < data.RandRecords.size(); recordIndex++)
	{
		encodedDesired.clear();
		actualVector.clear();
		correct = true;
		inputLayer.clear();
		outputLayer.clear();
		hiddenLayers.clear();

		desired = GenerateInputLayer(data, params);
		GenerateHiddenLayers(params);
		GenerateOutputLayer(params);

		// Encode Desired
		if (desired < params.FireSeverityLowCutoff)
		{
			encodedDesired.push_back(1);
			encodedDesired.push_back(0);
			encodedDesired.push_back(0);
		}
		else if (desired > params.FireSeverityHighCutoff)
		{
			encodedDesired.push_back(0);
			encodedDesired.push_back(0);
			encodedDesired.push_back(1);
		}
		else
		{
			encodedDesired.push_back(0);
			encodedDesired.push_back(1);
			encodedDesired.push_back(0);
		}

		// Calculated Values
		for (int hiddenCol = 0; hiddenCol < hiddenLayers.size(); hiddenCol++)
		{
			for (int hiddenNodeIndex = 0; hiddenNodeIndex < params.NumberOfHiddenNodes; hiddenNodeIndex++)
			{
				for (int numLeftNodes = 0; numLeftNodes < leftNodes; numLeftNodes++)
				{
					calculatedValue = calculatedValue + (inputLayer[numLeftNodes].value * weights[hiddenCol][numLeftNodes][hiddenNodeIndex]);

				}
				hiddenLayers[hiddenCol][hiddenNodeIndex].value = hiddenLayers[hiddenCol][hiddenNodeIndex].Sigmoid(calculatedValue);
				calculatedValue = 0;
			}
			leftNodes = params.NumberOfHiddenNodes;
		}

		int weightIndex = params.AdjustableLayerWeights - 1;
		calculatedValue = 0;
		for (int outputNodeIndex = 0; outputNodeIndex < params.NumberOfOutputNodes; outputNodeIndex++)
		{
			for (int hiddenNodeIndex = 0; hiddenNodeIndex < leftNodes + 1; hiddenNodeIndex++)
			{
				calculatedValue = calculatedValue + (hiddenLayers[hiddenLayers.size() - 1][hiddenNodeIndex].value * weights[weightIndex][hiddenNodeIndex][outputNodeIndex]);

			}
			outputLayer[outputNodeIndex].value = outputLayer[outputNodeIndex].Sigmoid(calculatedValue);
			calculatedValue = 0;
		}

		// Check if we are correct
		for (int outputNodeIndex = 0; outputNodeIndex < params.NumberOfOutputNodes; outputNodeIndex++)
		{
			double actual = outputLayer[outputNodeIndex].value;

			if (actual < .5)
			{
				actualVector.push_back(0);
			}
			else
			{
				actualVector.push_back(1);
			}
		}

		cout << "Desired: " << encodedDesired[0] << encodedDesired[1] << encodedDesired[2]
			 << " Actual: " << actualVector[0] << actualVector[1] << actualVector[2];

		for (int index = 0; index < actualVector.size(); index++)
		{
			if (encodedDesired[index] != actualVector[index])
			{
				correct = false;
			}
		}

		if (!correct)
		{
			cout << " *" << endl;
		}
		else
		{
			numberCorrect++;
			cout << endl;
		}

	}

	cout << "Percentage Correct: " << (numberCorrect / data.RandRecords.size())*100.0 << "%" << endl;


}

void ANNTester::GenerateOutputLayer(InputParameters params)
{
	for (int i = 0; i < params.NumberOfOutputNodes; i++)
	{
		Neuron outNeuron;

		outNeuron.value = 0;
		outNeuron.deltaError = 0;

		outputLayer.push_back(outNeuron);
	}
}

void ANNTester::GenerateHiddenLayers(InputParameters params)
{
	for (int i = 0; i < (params.AdjustableLayerWeights - 1); i++)
	{
		vector<Neuron> hiddenLayerHolder;

		for (int j = 0; j < params.NumberOfHiddenNodes; j++)
		{
			Neuron hiddenNeuron;

			hiddenNeuron.value = 0;
			hiddenNeuron.deltaError = 0;

			hiddenLayerHolder.push_back(hiddenNeuron);
		}

		Neuron hiddenNeuron;

		hiddenNeuron.value = 1;
		hiddenNeuron.deltaError = 0;

		hiddenLayerHolder.push_back(hiddenNeuron);

		hiddenLayers.push_back(hiddenLayerHolder);
	}
}

double ANNTester::GenerateInputLayer(CSVFileReader data, InputParameters params)
{
	static int randRecordIndex = 0;
	int recordIndex = 0;
	double retVal = 0;

	if (randRecordIndex == data.RandRecords.size())
	{
		randRecordIndex = 0;
	}

	int year = data.RandRecords[randRecordIndex][0];
	int PDSIYears = ceil(params.MonthsOfPDSIData / 12.0);
	int BAYears = params.BurnedAcreageYears;

	for (recordIndex = 0; recordIndex < data.Records.size(); recordIndex++)
	{
		if (data.Records[recordIndex][0] == year)
		{
			retVal = data.Records[recordIndex][1];
			break;
		}
	}

	int numPDSIMonths = params.MonthsOfPDSIData;
	int j = 4;
	for (int i = recordIndex; numPDSIMonths != 0;)
	{
		Neuron inputNeuron;

		inputNeuron.value = data.NormalizedRecords[i][j];
		inputNeuron.deltaError = 0;

		inputLayer.push_back(inputNeuron);

		if (j == 2)
		{
			i--;
			j = 14;
		}

		numPDSIMonths--;
		j--;
	}

	int numBAYears = params.BurnedAcreageYears;
	for (int i = recordIndex - 1; numBAYears != 0; i--)
	{
		Neuron inputNeuron;

		inputNeuron.value = data.NormalizedRecords[i][1];
		inputNeuron.deltaError = 0;

		inputLayer.push_back(inputNeuron);

		numBAYears--;
	}

	Neuron biasNode;

	biasNode.value = 1;
	biasNode.deltaError = 0;

	inputLayer.push_back(biasNode);

	randRecordIndex++;

	return retVal;
}