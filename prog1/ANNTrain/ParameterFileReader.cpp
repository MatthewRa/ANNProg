#include "Header.h"

void ParameterFileReader::ParseParameterFile(string fileName)
{
	ifstream ifstream;
	int parameterIndex = 0;
	string row = "";

	ifstream.open(fileName);
	if (!ifstream)
	{
		cout << "error opening parameter file: " << fileName << endl;
		return;
	}

	while (getline(ifstream, row))
	{
		if (row[0] == '#' || row.size() == 0)
		{
			continue;
		}
		else 
		{
			switch (parameterIndex)
			{
				case 0:
					break;
				case 1:
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					break;
				case 5:
					break;
				case 6:
					break;
				case 7:
					break;
				case 8:
					break;
				case 9:
					break;
				case 10:
					break;
				case 11:
					break;
				case 12:
					break;
				case 13:
					break;
				case 14:
					break;
			}
			parameterIndex++;
		}

	}

}