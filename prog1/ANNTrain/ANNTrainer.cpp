#include "Header.h"

void ANNTrainer::TrainNetwork(CSVFileReader data, InputParameters params)
{
	int leftNodes = params.NumberOfInputNodes;
	double calculatedValue = 0;

	GenerateHiddenLayers(params);
	GenerateOutputLayer(params);
	InitializeWeights(params);



	for (int recordIndex = 0; recordIndex < data.RandRecords.size(); recordIndex++)
	{
		GenerateInputLayer(data, params);

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
			for (int hiddenNodeIndex = 0; hiddenNodeIndex < leftNodes; hiddenNodeIndex++)
			{
				calculatedValue = calculatedValue + (inputLayer[hiddenNodeIndex].value * weights[weightIndex][hiddenNodeIndex][outputNodeIndex]);

			}
			outputLayer[outputNodeIndex].value = outputLayer[outputNodeIndex].Sigmoid(calculatedValue);
			calculatedValue = 0;
		}


	}
	//double newLow = 1 * (params.FireSeverityLowCutoff / params.FireSeverityHighCutoff);
	//double newHigh = 1;
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
	int numLeftNodes = params.NumberOfInputNodes;
	int numRightNodes = params.NumberOfHiddenNodes + 1;
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
				innerVector.push_back(rand());
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

		Neuron hiddenNeuron;

		hiddenNeuron.value = 1;
		hiddenNeuron.deltaError = 0;

		hiddenLayerHolder.push_back(hiddenNeuron);

		hiddenLayers.push_back(hiddenLayerHolder);
	}
}

void ANNTrainer::GenerateInputLayer(CSVFileReader data, InputParameters params)
{
	static int randRecordIndex = 0;
	int recordIndex = 0;

	int year = data.RandRecords[randRecordIndex][0];
	int PDSIYears = ceil(params.MonthsOfPDSIData / 12.0);
	int BAYears = params.BurnedAcreageYears;

	for (recordIndex = 0; recordIndex < data.Records.size(); recordIndex++)
	{
		if (data.Records[recordIndex][0] == year)
		{
			break;
		}
	}

	int numPDSIMonths = params.MonthsOfPDSIData;
	int j = 4;
	for (int i = recordIndex; numPDSIMonths != 0;)
	{
		Neuron inputNeuron;

		inputNeuron.value = data.Records[i][j];
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
	for (int i = recordIndex - 1; numBAYears != 0 ; i--)
	{
		Neuron inputNeuron;

		inputNeuron.value = data.Records[i][1];
		inputNeuron.deltaError = 0;

		inputLayer.push_back(inputNeuron);

		numBAYears--;
	}
}

void ANNTrainer::readInWeights(InputParameters params)
{
	// vector<vector<vector<double>>> weights;
	ifstream inWeights;
	int layer = 0; // current working layer 
	int nodes = params.NumberOfInputNodes; // handle input to hidden nodes
	int hiddenlayers = params.AdjustableLayerWeights - 1; // number of hidden layers

	// open and error check file
	inWeights.open(params.ANNWeightFile);
	if (!inWeights)
	{
		cerr << "Error opening weights file" << endl;
		exit(-1);
	}

	// handle the input to first hidden layer and the hidden to hidden layers
	while (layer != params.AdjustableLayerWeights - 1)
	{
		for (int startNode = 0; startNode < nodes - 1; startNode++)
		{
			for (int endNode = 0; endNode < params.NumberOfHiddenNodes -1; endNode++)
			{
				inWeights >> weights[layer][startNode][endNode];
			}
		}
		// update nodes so the loop can handle the hidden to hidden weight layers
		nodes = params.NumberOfHiddenNodes;
		layer++;
	}

	// read in the last hidden to output weight layer 
	for (int startNode = 0; startNode < nodes - 1; startNode++)
	{
		for (int endNode = 0; endNode < params.NumberOfOutputNodes - 1; endNode++)
		{
			inWeights >> weights[layer][startNode][endNode];
		}
	}

	inWeights.close();

}

void ANNTrainer::writeOutWeights(InputParameters params)
{
	// vector<vector<vector<double>>> weights;
	ofstream outWeights;
	int layer = 0; // current working layer

	// open file and error check
	outWeights.open(params.ANNWeightFile);
	if (!outWeights)
	{
		cerr << "Error opening weights file" << endl;
		exit(-1);

	}

	// write out the weight values to the file based on the size of each layer 
	while (layer != params.AdjustableLayerWeights)
	{
		for (int startNode = 0; startNode < sizeof(weights); startNode++)
		{
			for (int endNode = 0; endNode < sizeof(weights[startNode]); endNode++)
			{
				//outWeights << layer << ' ' << startNode << ' ' << endNode << ' '
					//<< weights[layer][startNode][endNode] << endl;
				outWeights << weights[layer][startNode][endNode] << endl;
			}
		}
		layer++;
	}

	outWeights.close();
}