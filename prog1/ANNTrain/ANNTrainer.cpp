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