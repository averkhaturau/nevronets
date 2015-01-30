#include <iostream>
#include "../pneuro/nnlayer.h"


int main(int argc, char *argv[])
{
	const SYGDATATYPE rates[] = {-0.8, 0.6, 0.9, 3};
	SYGDATATYPE inputs[2];
//	unsigned int n,m;
	//std::cout << "Enter this and prevous layes size:";
	//std::cin >> n >> m;
 	CNNLayer MyLayer(2, 2, f_sigmoid);
	MyLayer.SetRates(rates, 4);
	std::cout << "Enter inputs: ";
	std::cin >> inputs[0] >> inputs[1];
	MyLayer.SetInput(inputs, 2);
	MyLayer.GetAscons(inputs, 2);
	std::cout << inputs[0] << inputs[1];
	return 0;
}
