#include "ANNTest.h"

// Function to determine if neuron fires or not
#include "ANNTest.h"

bool Neuron::activation()
{
	//if (sum > threshold)
	//{
	//	return true;
	//}
	return true;
}

// Sums up input from previous layer of neurons
double Neuron::sum()
{
	int i = 0;
	double sum = 0;

	for (i = 0; i < input.size(); i++)
	{
		sum += input[i];
	}
	
	return sum;
};