#include "ANNTest.h"
#include "Header.h"

int main(int argc, char *argv[])
{
	ParameterFileReader paramFileReader;
	InputParameters inputParams;
	string parameterFileName;

	if (argc != 2)
	{
		return 0;
	}

	if (argv[1] != NULL)
	{
		parameterFileName = string(argv[1]);
	}

	inputParams = paramFileReader.ParseParameterFile(parameterFileName);

	int N = 100;
	int i, j = 0;
	double weights[3][2];
	double weights2[2][3];

	Neuron inputLayer[3];
	Neuron hiddenLayer[2];
	Neuron outputLayer[3];

	// Generate test weights
	for (j = 0; j < 3; j++)
	{ 
		for (i = 0; i < 2; i++)
		{
			weights[0][i] = double(rand());
			//cout << "Weight[0][" << i << "]: " << weights[0][i] << endl;
			weights[1][i] = double(rand());
			//cout << "Weight[1][" << i << "]: " << weights[1][i] << endl;
			weights[2][i] = double(rand());
			//cout << "Weight[2][" << i << "]: " << weights[2][i] << endl;
		}
	}

	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 3; j++)
		{
			weights2[i][j] = double(rand());
			cout << "Weight2 element [" << i << "][" << j << "]: " << weights2[i][j] << endl;
		}
	}


	cout << endl;
	// Generate random inputs for inputlayer
	for (i = 0; i < 3; i++)
	{
		inputLayer[i].output = double(rand());
		cout << "Neuron " << i << " has input: " << inputLayer[i].output << endl;
	}

	// This loop nest translates the data from the first layer "j" into the next layer "i"
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 3; j++)
		{
			
			// Weights the output from neuron j and pushes it into the input vector of neuron i
			hiddenLayer[i].input.push_back(  weights[j][i] * inputLayer[j].output);

			cout << "Hidden neuron: " << i << " has from weight [" << j << "][" << i << "]"
				 << " which results in weighted input: " << hiddenLayer[i].input[0] << endl;
		}

		// Sum up input vector values
		hiddenLayer[i].output = hiddenLayer[i].sum();
		cout << "Hidden neuron sum: " << hiddenLayer[i].output << endl;
	}


	// Hidden layer to output layer
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 2; j++)
		{
			outputLayer[i].input.push_back( weights2[j][i] * hiddenLayer[j].output);
			cout << "Output neuron: " << i << " has from weight [" << j << "][" << i << "]"
				 << " which results in weighted input: " << outputLayer[i].input[0] << endl;
		}
	}

	
	// For testing each neuron constructor
	//cout << "Here is test1: " << test1.input << ", sum: " << test1.sum << endl;
	//cout << "Here is test2: " << test2.input << ", sum: " << test2.sum << endl;
	//cout << "Here is test3: " << test3.input << ", sum: " << test3.sum << endl;
	
	return 0;
}