#pragma once

#include <iostream>
#include <vector>

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


using namespace std;

class Neuron
{
	public:

		// Vector of inputs to the neuron
		vector<double> input;
		// Vector of outputs to the next layer
		double output;


		// Function to determine if neuron fires or not
		bool activation();

		// Sums up input from previous layer of neurons
		double sum();
};


