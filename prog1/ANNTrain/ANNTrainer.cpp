#include "Header.h"

void ANNTrainer::TrainNetwork(InputParameters params)
{
	cout << params.NumberOfInputNodes << endl;
	cout << params.NumberOfHiddenNodes << endl;
	cout << params.NumberOfOutputNodes << endl;

	for (int weights = 0; weights < params.AdjustableLayerWeights; weights++)
	{

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