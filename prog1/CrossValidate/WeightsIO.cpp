#include "Header.h"

void WeightsIO::readInWeights(vector<vector<vector<double>>> &weights, InputParameters params)
{
	// vector<vector<vector<double>>> weights;
	ifstream inWeights;
	int layer = 0; // current working layer 
	int nodes = params.NumberOfInputNodes; // handle input to hidden nodes
	int hiddenlayers = params.AdjustableLayerWeights - 1; // number of hidden layers
	double nodeValue = 0;
	vector<double> valToBeInserted;
	vector<vector<double>> nodeToBeInserted;

	// empty the vector for new read
	weights.clear();
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
		for (int startNode = 0; startNode < nodes; startNode++)
		{
			for (int endNode = 0; endNode < params.NumberOfHiddenNodes + 1; endNode++)
			{
				//get weight value and push on to the vector holding the weights
				// connected to the current node
				inWeights >> nodeValue;
				valToBeInserted.push_back(nodeValue);


			}
			// once all weights have been pushed for that node push the node
			// into the node list
			nodeToBeInserted.push_back(valToBeInserted);
			//clear the value vector for next node
			valToBeInserted.clear();
		}
		// After entire layer has been processed push to weight vector and clear nodes
		weights.push_back(nodeToBeInserted);
		nodeToBeInserted.clear();
		// update nodes so the loop can handle the hidden to hidden weight layers
		nodes = params.NumberOfHiddenNodes;
		layer++;
	}

	// read in the last hidden to output weight layer 
	for (int startNode = 0; startNode < nodes + 1; startNode++)
	{
		for (int endNode = 0; endNode < params.NumberOfOutputNodes; endNode++)
		{
			inWeights >> nodeValue;
			valToBeInserted.push_back(nodeValue);
		}
		nodeToBeInserted.push_back(valToBeInserted);
		valToBeInserted.clear();
	}
	weights.push_back(nodeToBeInserted);

	inWeights.close();

}

void WeightsIO::writeOutWeights(vector<vector<vector<double>>> &weights, InputParameters params)
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
	for (int layer = 0; layer < weights.size(); layer++)
	{
		for (int leftNodes = 0; leftNodes < weights[layer].size(); leftNodes++)
		{
			for (int rightNode = 0; rightNode < weights[layer][leftNodes].size(); rightNode++)
			{
				outWeights << weights[layer][leftNodes][rightNode] << endl;
			}
		}
	}

	outWeights.close();
}