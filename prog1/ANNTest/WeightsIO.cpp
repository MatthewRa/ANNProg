#include "Header.h"

/*     
   Reads in the weights from the weights file to connect the nodes for testing 
*/
void WeightsIO::readInWeights(vector<vector<vector<double>>> &weights, InputParameters params)
{
	ifstream inWeights;
	int layer = 0; // current working layer 
	int nodes = params.NumberOfInputNodes + 1; // handle input to hidden nodes
	int hiddenlayers = params.AdjustableLayerWeights - 1; // number of hidden layers
	double nodeValue = 0;
	vector<double> valToBeInserted; // stores weight values for a node to another node  
	vector<vector<double>> nodeToBeInserted; // stores node in layer

	// empty the vector for new read
	weights.clear();
	// open and error check file
	inWeights.open("../" + params.ANNWeightFile);
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
			for (int endNode = 0; endNode < params.NumberOfHiddenNodes; endNode++)
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

/*
Write out the  weights to the weights file to connect the nodes for testing
*/
void WeightsIO::writeOutWeights(vector<vector<vector<double>>> &weights, InputParameters params)
{
	ofstream outWeights;
	int layer = 0; // current working layer

				   // open file and error check
	outWeights.open("../" + params.ANNWeightFile);
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