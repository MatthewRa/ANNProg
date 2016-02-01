#include "Header.h"
#include <regex>

int main(int argc, char *argv[])
{

	CSVFileReader csvFileReader;
	ParameterFileReader paramFileReader;

	// replace with correct commandline argument (Marcus)------------------------
	csvFileReader.filename = "PDSI_BH_1978-2015.csv";
	// -------------------------------------------------------------------------
	
	/* commented out for testing purposes (Marcus)
	if (!CheckArguments(argc,argv[]))
	{
		return 0;
	}
	*/

	paramFileReader.ParseParameterFile();
	csvFileReader.ReadDataFile(csvFileReader.records, csvFileReader.headings,
		csvFileReader.filename);
	if (csvFileReader.records.empty() == true ||
		csvFileReader.headings.empty() == true)
	{
		cout << "An error occurred while reading csv file, exiting program.";
		return -1;
	}

	return 0;
}

bool CheckArguments(int argc, char *argv[])
{
	bool retVal = true;

	if (argc != 2)
	{
		retVal = false;
	}
	if (regex_match(argv[1], regex("(.*csv)")) == false)
		retVal = false;

	return retVal;
}