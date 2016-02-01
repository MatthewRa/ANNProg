#include "Header.h"


int main(int argc, char *argv[])
{
	CSVFileReader csvFileReader;
	ParameterFileReader paramFileReader;
	string parameterFileName;

	if (argc != 2)
	{
		return 0;
	}

	if (argv[1] != NULL)
	{
		parameterFileName = string(argv[1]);
	}

	paramFileReader.ParseParameterFile(parameterFileName);

	// replace with correct commandline argument (Marcus)------------------------
	csvFileReader.filename = "PDSI_BH_1978-2015.csv";
	// -------------------------------------------------------------------------

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
