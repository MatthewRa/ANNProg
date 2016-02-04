#include "Header.h"



InputParameters ParameterFileReader::ParseParameterFile(string fileName)
{
	ifstream ifstream;
	InputParameters inputParams;
	int parameterIndex = 0;
	string row = "";
	char* token;

	ifstream.open(fileName);
	if (!ifstream)
	{
		cout << "error opening parameter file: " << fileName << endl;
		return inputParams;
	}

	while (getline(ifstream, row))
	{
		if (row[0] == '#' || row.size() == 0)
		{
			continue;
		}
		else 
		{
			char *tempRow = new char[row.size() + 1]; // Character array that will store copy of row to be tokenized
			const char *rowPtr = row.c_str(); // Pointer to current row

			switch (parameterIndex)
			{
				case 0:
					strcpy(tempRow, rowPtr);
					token = strtok(tempRow, " ");

					inputParams.ANNWeightFile = string(token);
					break;

				case 1:
					strcpy(tempRow, rowPtr);
					token = strtok(tempRow, " ");

					inputParams.TrainingEpochs = atoi(token);
					break;

				case 2:
					strcpy(tempRow, rowPtr);
					token = strtok(tempRow, " ");

					inputParams.LearnRate = atof(token);
					break;

				case 3:
					strcpy(tempRow, rowPtr);
					token = strtok(tempRow, " ");

					inputParams.Momentum = atof(token);
					break;

				case 4:
					strcpy(tempRow, rowPtr);
					token = strtok(tempRow, " ");

					inputParams.ErrorThreshold = atof(token);
					break;

				case 5:
					strcpy(tempRow, rowPtr);
					token = strtok(tempRow, " ");

					inputParams.AdjustableLayerWeights = atoi(token);
					break;

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

				case 7:
					strcpy(tempRow, rowPtr);
					token = strtok(tempRow, " ");

					inputParams.DataFileName = string(token);
					break;

				case 8:
					strcpy(tempRow, rowPtr);
					token = strtok(tempRow, " ");

					inputParams.BurnedAcreageYears = atoi(token);
					break;

				case 9:
					strcpy(tempRow, rowPtr);
					token = strtok(tempRow, " ");

					inputParams.MonthsOfPDSIData = atoi(token);
					break;

				case 10:
					strcpy(tempRow, rowPtr);
					token = strtok(tempRow, " ");

					inputParams.EndMonthOfYear = atoi(token);
					break;
				case 11:
					strcpy(tempRow, rowPtr);
					token = strtok(tempRow, " ");

					inputParams.NumberOfClasses = atoi(token);
					break;
				case 12:
					strcpy(tempRow, rowPtr);
					token = strtok(tempRow, " ");

					inputParams.FireSeverityLowCutoff = atoi(token);
					break;
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