#include <iostream>
#include <windows.h>
#include "../pneuro/nnperceptron.h"
#include "../pneuro/nnifc.h"
#include "../pneuro/nnTraining.h"
#include "../pneuro/nnGATraining.h"


int main(int argc, char *argv[])
{
	CNNIfc MyIfc;

	{
		CNNForwardPropagation<CNNLayer> MyPerceptron("perceptron.nn", MyIfc, static_cast<CActi>(acti_lin));

		CNNTGA MyTrener;

		CTrainingInstances MyInstatnces("Instances.nni", MyIfc);

		MyTrener.Train(&MyPerceptron, &MyInstatnces, 0.000390624);

		//double dstime=MPI_Wtime();
      __int64 i64time = GetTickCount();
//		MyPerceptron.Propagate(pInput, n, pOutput, n);
		i64time = GetTickCount() - i64time;
		//dstime=MPI_Wtime()-dstime;
//		printf("Time for %d neurons is %i\n", n, i64time);

		MyIfc.Save2file("perceptron-obucheny.nn", &MyPerceptron);
	}


	return 0;
}
