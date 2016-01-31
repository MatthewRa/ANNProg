#include "Header.h"

int main(int argc, char *argv[])
{
	ParameterFileReader paramFileReader;

	if (!CheckArguments(argc))
	{
		return 0;
	}

	paramFileReader.ParseParameterFile();

	return 0;
}

bool CheckArguments(int argc)
{
	bool retVal = true;

	if (argc != 2)
	{
		retVal = false;
	}

	return retVal;
}