#include "Header.h"


/*************************************************************************
* Name:  ParseParameterFile
* Params: string fileName - The name of the parameter file to read in.
*
* Description:  This file steps through each row in the parameter file.
* It checks the value of the first character and determines wether to read
* the value in or skip the line. This method returns a struct storing all
* the parameters.
*
* To-do:
*		- implement the momentum value in updating weights
*************************************************************************/

InputParameters ParameterFileReader::ParseParameterFile(string fileName)
{
	ifstream ifstream;
	InputParameters inputParams;
	int parameterIndex = 0;
	string row = "";
	char* token;

	// Open value
	ifstream.open(fileName);
	if (!ifstream)
	{
		cout << "error opening parameter file: " << fileName << endl;
		return inputParams;
	}

	// Loop through every line in the parameter file
	while (getline(ifstream, row))
	{
		// If the line starts with # or is empty skip that line
		if (row[0] == '#' || row.size() == 0)
		{
			continue;
		}
		else
		{
			// Prep the row for tokenizing
			char *tempRow = new char[row.size() + 1]; // Character array that will store copy of row to be tokenized
			const char *rowPtr = row.c_str(); // Pointer to current row

			switch (parameterIndex)
			{
				// Store the weight file name
			case 0:
				strcpy(tempRow, rowPtr);
				token = strtok(tempRow, " ");

				inputParams.ANNWeightFile = string(token);
				break;
				// Store the number of epochs to run
			case 1:
				strcpy(tempRow, rowPtr);
				token = strtok(tempRow, " ");

				inputParams.TrainingEpochs = atoi(token);
				break;
				// Store the learn rate
			case 2:
				strcpy(tempRow, rowPtr);
				token = strtok(tempRow, " ");

				inputParams.LearnRate = atof(token);
				break;
				// Store the momentum variable
			case 3:
				strcpy(tempRow, rowPtr);
				token = strtok(tempRow, " ");

				inputParams.Momentum = atof(token);
				break;
				// Store the error threshold
			case 4:
				strcpy(tempRow, rowPtr);
				token = strtok(tempRow, " ");

				inputParams.ErrorThreshold = atof(token);
				break;
				// Store the layer of adjustable weights
			case 5:
				strcpy(tempRow, rowPtr);
				token = strtok(tempRow, " ");

				inputParams.AdjustableLayerWeights = atoi(token);
				break;
				// Store how many nodes are in each layer
			case 6:
				strcpy(tempRow, rowPtr);
				token = strtok(tempRow, " ");

				inputParams.NumberOfInputNodes = atoi(token);

				strcpy(tempRow, rowPtr);
				token = strtok(NULL, " ");

				inputParams.NumberOfHiddenNodes = atoi(token);

				strcpy(tempRow, rowPtr);
				token = strtok(NULL, " ");

				inputParams.NumberOfOutputNodes = atoi(token);
				break;
				// Store the data file name
			case 7:
				strcpy(tempRow, rowPtr);
				token = strtok(tempRow, " ");

				inputParams.DataFileName = string(token);
				break;
				// Store burned acreage
			case 8:
				strcpy(tempRow, rowPtr);
				token = strtok(tempRow, " ");

				inputParams.BurnedAcreageYears = atoi(token);
				break;
				// Store the amount of PDSI months
			case 9:
				strcpy(tempRow, rowPtr);
				token = strtok(tempRow, " ");

				inputParams.MonthsOfPDSIData = atoi(token);
				break;
				// Store ending month of year
			case 10:
				strcpy(tempRow, rowPtr);
				token = strtok(tempRow, " ");

				inputParams.EndMonthOfYear = atoi(token);
				break;
				// Store the number of classes
			case 11:
				strcpy(tempRow, rowPtr);
				token = strtok(tempRow, " ");

				inputParams.NumberOfClasses = atoi(token);
				break;
				// Store low cutoff value
			case 12:
				strcpy(tempRow, rowPtr);
				token = strtok(tempRow, " ");

				inputParams.FireSeverityLowCutoff = atoi(token);
				break;
				// Store high cutoff value
			case 13:
				strcpy(tempRow, rowPtr);
				token = strtok(tempRow, " ");

				inputParams.FireSeverityHighCutoff = atoi(token);
				break;
			}
			parameterIndex++;
		}

	}

	return inputParams;
}