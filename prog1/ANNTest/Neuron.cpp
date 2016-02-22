#include "Header.h"

/*
	takes the value of a node and runs it though the sigmoid function.
	f(x) = 1/(1+e^x)
*/
double Neuron::Sigmoid(double x)
{
	double retVal = 0;

	double temp = exp(-x);

	retVal = 1.0 / (1.0 + exp(-x));

	return retVal;
}

/*
	Takes the sigmoided value of a function and runs it through the sigmoid prime
	function. f(x) = x(1-x)
*/
double Neuron::SigmoidPrime(double x)
{
	double retVal = 0;

	retVal = x * (1 - x);

	return retVal;
}