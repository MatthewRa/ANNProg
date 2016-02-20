#include "Header.h"

double Neuron::Sigmoid(double x)
{
	double retVal = 0;

	double temp = exp(-x);

	retVal = 1.0 / (1.0 + exp(-x));

	return retVal;
}

double Neuron::SigmoidPrime(double x)
{
	double retVal = 0;

	retVal = x * (1.0 - x);

	return retVal;
}