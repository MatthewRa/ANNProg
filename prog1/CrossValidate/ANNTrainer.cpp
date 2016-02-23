#include "Header.h"

void ANNTrainer::TrainNetwork(CSVFileReader data, InputParameters params)
{
	int leftNodes = params.NumberOfInputNodes + 1;
	double calculatedValue = 0;
	double calculatedDeltaError = 0;
	double desired = 0;
	double previousWeight = 0;
	double ksum = 0;
	double psum = 0;
	vector<int> encodedDesired;
	srand(time(0));

	double squaredError = 0;
	double RMSE = 0;

	//GenerateHiddenLayers(params);
	//GenerateOutputLayer(params);
	InitializeWeights(params);


	for (int epochIndex = 1; epochIndex < params.TrainingEpochs+1; epochIndex++)
	{
		squaredError = 0;

		random_shuffle(begin(data.RandRecords), end(data.RandRecords));

		psum = 0;

		for (int recordIndex = 0; recordIndex < data.RandRecords.size(); recordIndex++)
		{
			inputLayer.clear();
			outputLayer.clear();
			hiddenLayers.clear();
			encodedDesired.clear();

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

			// Calculate values for hidden neuorns and output Neurons
			ForwardPropagation(data, params); 

			// Calculate a delta error for hidden neruons, then hidden neurons
			CalculateDeltaError(data, params, psum, encodedDesired);

			// Update weights based on values and delta errors
			UpdateWeights(data, params);

		}

		RMSE = sqrt(psum / (params.NumberOfOutputNodes * data.RandRecords.size()));
		if (RMSE < params.ErrorThreshold)
		{
			cout << "Epoch: " << epochIndex << ", RootMeanSquaredError: " << RMSE << endl;
			break;
		}

		if (epochIndex % 10 == 0)
		{
			cout << "Epoch: " << epochIndex <<  ", RootMeanSquaredError: " << RMSE << endl;
		}
	}
}

void ANNTrainer::UpdateWeights(CSVFileReader data, InputParameters params)
{
	double newWeight = 0;

	// Updating Weights
	for (int layer = params.AdjustableLayerWeights - 1; layer >= 0; layer--)
	{
		for (int leftNodes = 0; leftNodes < weights[layer].size(); leftNodes++)
		{
			for (int rightNode = 0; rightNode < weights[layer][leftNodes].size(); rightNode++)
			{
				if (layer == 0)
				{
					newWeight = weights[layer][leftNodes][rightNode] - params.LearnRate * hiddenLayers[0][rightNode].deltaError * inputLayer[leftNodes].value;
					weights[layer][leftNodes][rightNode] = newWeight;
				}
				else if (layer != params.AdjustableLayerWeights - 1)
				{
					newWeight = weights[layer][leftNodes][rightNode] - params.LearnRate * hiddenLayers[layer + 1][rightNode].deltaError * hiddenLayers[layer - 1][leftNodes].value;
					weights[layer][leftNodes][rightNode] = newWeight;
				}
				else
				{
					newWeight = weights[layer][leftNodes][rightNode] - params.LearnRate * outputLayer[rightNode].deltaError * hiddenLayers[layer - 1][leftNodes].value;
					weights[layer][leftNodes][rightNode] = newWeight;
				}
			}
		}
	}
}

void ANNTrainer::ForwardPropagation(CSVFileReader data, InputParameters params)
{
	double calculatedValue = 0;
	int leftNodes = params.NumberOfInputNodes + 1;

	// Calculated Values
	for (int hiddenCol = 0; hiddenCol < hiddenLayers.size(); hiddenCol++)
	{
		for (int hiddenNodeIndex = 0; hiddenNodeIndex < params.NumberOfHiddenNodes; hiddenNodeIndex++)
		{
			for (int LeftNodeIndex = 0; LeftNodeIndex < leftNodes; LeftNodeIndex++)
			{
				calculatedValue = calculatedValue + (inputLayer[LeftNodeIndex].value * weights[hiddenCol][LeftNodeIndex][hiddenNodeIndex]);

			}
			hiddenLayers[hiddenCol][hiddenNodeIndex].value = hiddenLayers[hiddenCol][hiddenNodeIndex].Sigmoid(calculatedValue);
			calculatedValue = 0;
		}
		leftNodes = params.NumberOfHiddenNodes;
	}

	int weightIndex = params.AdjustableLayerWeights - 1;
	calculatedValue = 0;
	leftNodes = params.NumberOfHiddenNodes + 1;
	for (int outputNodeIndex = 0; outputNodeIndex < params.NumberOfOutputNodes; outputNodeIndex++)
	{
		for (int hiddenNodeIndex = 0; hiddenNodeIndex < leftNodes; hiddenNodeIndex++)
		{
			calculatedValue = calculatedValue + (hiddenLayers[hiddenLayers.size() - 1][hiddenNodeIndex].value * weights[weightIndex][hiddenNodeIndex][outputNodeIndex]);

		}
		outputLayer[outputNodeIndex].value = outputLayer[outputNodeIndex].Sigmoid(calculatedValue);
		calculatedValue = 0;
	}
}

void ANNTrainer::CalculateDeltaError(CSVFileReader data, InputParameters params, double &psum, vector<int> encodedDesired)
{
	double ksum = 0;
	double newWeight = 0;
	double calculatedDeltaError = 0;

	// DeltaError Calculatons
	for (int outputNodeIndex = 0; outputNodeIndex < params.NumberOfOutputNodes; outputNodeIndex++)
	{
		double actual = outputLayer[outputNodeIndex].value;
		// delta for output = - (desired - actual) * f'(actual)
		outputLayer[outputNodeIndex].deltaError = -1 * ((double)encodedDesired[outputNodeIndex] - actual) * outputLayer[outputNodeIndex].SigmoidPrime(actual);

		ksum = ksum + pow((double)encodedDesired[outputNodeIndex] - actual, 2.0);

	}
	psum += ksum;

	if (hiddenLayers.size() > 1)
	{
		for (int hiddenNodeIndex = 0; hiddenNodeIndex < params.NumberOfHiddenNodes + 1; hiddenNodeIndex++)
		{
			for (int rightHiddenIndex = 0; rightHiddenIndex < params.NumberOfHiddenNodes; rightHiddenIndex++)
			{
				calculatedDeltaError = calculatedDeltaError + (hiddenLayers[rightHiddenIndex][hiddenNodeIndex].deltaError * weights[params.AdjustableLayerWeights - 1][hiddenNodeIndex][rightHiddenIndex]);
			}
			double hiddenNodeValue = hiddenLayers[hiddenLayers.size() - 1][hiddenNodeIndex].value;
			hiddenLayers[hiddenLayers.size() - 1][hiddenNodeIndex].deltaError = calculatedDeltaError *
				hiddenLayers[hiddenLayers.size() - 1][hiddenNodeIndex].SigmoidPrime(hiddenNodeValue);
		}
	}

	for (int hiddenNodeIndex = 0; hiddenNodeIndex < params.NumberOfHiddenNodes; hiddenNodeIndex++)
	{
		for (int outputNodeIndex = 0; outputNodeIndex < params.NumberOfOutputNodes; outputNodeIndex++)
		{
			calculatedDeltaError = calculatedDeltaError + (outputLayer[outputNodeIndex].deltaError * weights[params.AdjustableLayerWeights - 1][hiddenNodeIndex][outputNodeIndex]);

		}
		double hiddenNodeValue = hiddenLayers[hiddenLayers.size() - 1][hiddenNodeIndex].value;
		hiddenLayers[hiddenLayers.size() - 1][hiddenNodeIndex].deltaError = calculatedDeltaError *
			hiddenLayers[hiddenLayers.size() - 1][hiddenNodeIndex].SigmoidPrime(hiddenNodeValue);
	}
}

void ANNTrainer::GenerateOutputLayer(InputParameters params)
{
	for (int i = 0; i < params.NumberOfOutputNodes; i++)
	{
		Neuron outNeuron;

		outNeuron.value = 0;
		outNeuron.deltaError = 0;

		outputLayer.push_back(outNeuron);
	}
}

void ANNTrainer::InitializeWeights(InputParameters params)
{
	vector<vector<double>> middleVector;
	vector<double> innerVector;
	int numLeftNodes = params.NumberOfInputNodes + 1;
	int numRightNodes = params.NumberOfHiddenNodes;
	srand(time(0));

	for (int i = 0; i < params.AdjustableLayerWeights; i++)
	{
		if (i == params.AdjustableLayerWeights - 1)
		{
			numRightNodes = params.NumberOfOutputNodes;
		}

		middleVector.clear();
		for (int j = 0; j < numLeftNodes; j++)
		{
			innerVector.clear();
			for (int k = 0; k < numRightNodes; k++)
			{
				innerVector.push_back( ((rand()%2000) / 1000.0) - 1.0);
			}
			middleVector.push_back(innerVector);
		}
		weights.push_back(middleVector);
		numLeftNodes = params.NumberOfHiddenNodes + 1;
	}

}

void ANNTrainer::GenerateHiddenLayers(InputParameters params)
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

		Neuron biasNode;

		biasNode.value = 1;
		biasNode.deltaError = 0;

		hiddenLayerHolder.push_back(biasNode);

		hiddenLayers.push_back(hiddenLayerHolder);
	}
}

double ANNTrainer::GenerateInputLayer(CSVFileReader data, InputParameters params)
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

	int numBAYears = params.BurnedAcreageYears;
	for (int i = recordIndex - 1; numBAYears != 0 ; i--)
	{
		Neuron inputNeuron;

		inputNeuron.value = data.NormalizedRecords[i][1];
		inputNeuron.deltaError = 0;

		inputLayer.push_back(inputNeuron);

		numBAYears--;
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

	Neuron biasNode;

	biasNode.value = 1;
	biasNode.deltaError = 0;

	inputLayer.push_back(biasNode);

	randRecordIndex++;

	return retVal;
}

