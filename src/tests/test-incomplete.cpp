#include <iostream>
#include "../pneuro/nnIncompleteConnected.h"
#include "../pneuro/nnlayer.h"


int main(int argc, char *argv[])
{
	unsigned int NumLayers[]={1,2,2,2};// трёхслойная НС
	unsigned int NumSizes []={1,2,3,2,3}; // . :
                                          // : : . 
	                                      // . :
	unsigned int _idispls []={0,1};
	unsigned int _isizes  []={2,2};

	SYGDATATYPE _pInput[]={2e2, 1e3, 15};
	SYGDATATYPE _pOutput[1];

	typedef CNNForwardPropagation<CNNLayer> FullConectedNN;
	CNNIncompleteConnected<FullConectedNN>
		MyICNN(2, NumLayers, NumSizes, static_cast<CActi>(acti_htan));

	MyICNN.Propagate(
		_pInput, _idispls, _isizes, 2,
		_pOutput, 1);

	return 0;
}
