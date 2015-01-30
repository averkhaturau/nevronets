#include <iostream> 
#include <windows.h>
#include "../pneuro/nnForwardPropagation.h"
#include "../pneuro/nnifc.h"
#include "../pneuro/nnTraining.h"
#include "../pneuro/nnGATraining.h"


int main(int argc, char *argv[])
{
	unsigned int i;
	CNNIfc MyIfc;

	{
		//CNNForwardPropagation<CNNLayer> MyPerceptron("FRET_percept.nn", MyIfc, static_cast<CActi>(acti_htan));

		//CTrainingInstances MyInstatnces1("FRET_in_out-all.nni", MyIfc);
		//CTrainingInstances MyInstatnces2("test-inst2.nni", MyIfc);

		CNNTGA MyTrener1;

		//CNNIndividual MyIndividual(&MyPerceptron, &MyInstatnces1);


		for (i=3; i<105; i+=5)
		{
			CNNForwardPropagation<CNNLayer> MyPerceptron("FRET_percept.nn", MyIfc, static_cast<CActi>(acti_htan));
			CTrainingInstances MyInstatnces2("test-inst2.nni", MyIfc);
			MyTrener1.Train(&MyPerceptron, &MyInstatnces2, 1e-7, i);
		}
/*

		//printf("\nLipsic const is %le", MyInstatnces2.CalcLipshic());
		//MyIfc.SaveInstanses("test-inst2.nni", &MyInstatnces2);
		unsigned int i;
		for (i = 2; i<20; ++i)
		{
			CNNForwardPropagation<CNNLayer> locNN=MyPerceptron;
			MyTrener1.Train(&locNN, &MyInstatnces2, 7.2e-1, i);
			printf("\npopsize=%3d;\n", i);
			getch();
		}

		//MyIfc.SaveInstanses("FRET_in_out-part1of20.nni", &MyInstatnces1);
	*/	
	}


	return 0;
}
