#include <iostream>
#include <windows.h>
#include "../pneuro/nnlayer.h"
#include "../pneuro/nnifc.h"
#include "../pneuro/nnTraining.h"
#include "../pneuro/nnGATraining.h"


int main(int argc, char *argv[])
{
	CNNIfc MyIfc;

	{
		//CNNForwardPropagation<CNNLayer> MyPerceptron("FRET_percept.nn", MyIfc, static_cast<CActi>(acti_htan));

		//CNNStochastic MyTrener1;
		//CNNBackPropagation MyTrener2;

		//CTrainingInstances MyInstatnces("FRET_in_out-all.nni", MyIfc);

		//MyTrener2.Train(&MyPerceptron, &MyInstatnces, 1e-9);
		//MyTrener1.Train(&MyPerceptron, &MyInstatnces, 1e-7);

		//double dstime=MPI_Wtime();
		unsigned int ls[2];
		for (int n=1; n< 10000; n+=100)
		{
			ls[0]=n;ls[1]=n;
			CNNForwardPropagation<CNNLayer> MyNN(1, ls, static_cast<CActi>(acti_lin));
			SYGDATATYPE *pInput =new SYGDATATYPE[n];
			SYGDATATYPE *pOutput=new SYGDATATYPE[n];
		  __int64 i64time = GetTickCount();
			MyNN.Propagate(pInput, n, pOutput, n);
			i64time = GetTickCount() - i64time;
			printf("Time for %d neurons is %i\n", n, i64time);
			delete[] pInput;
			delete[] pOutput;
		}
		//dstime=MPI_Wtime()-dstime;
//		printf("Time for %d neurons is %i\n", n, i64time);

		//MyIfc.Save2file("p-obucheny.nn", &MyPerceptron);
	}


	return 0;
}
