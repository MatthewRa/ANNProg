#include "ANNTest.h"
using namespace std;

int main(int argc, char *argv[])
{
	//cout << "Hello ANNTest! \n ";

	double testNum = 5.21;
	double testNums[2] = { 5.223, 213.244 };

	neuron test1;
	neuron test2(testNum);
	neuron test3(testNums);

	cout << "Here is test1: " << test1.input << ", sum: " << test1.sum << endl;
	cout << "Here is test2: " << test2.input << ", sum: " << test2.sum << endl;
	cout << "Here is test3: " << test3.input << ", sum: " << test3.sum << endl;
	
	return 0;
}