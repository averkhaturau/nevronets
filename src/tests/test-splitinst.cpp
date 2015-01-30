#include <iostream>
#include <windows.h>
#include "../pneuro/nnForwardPropagation.h"
#include "../pneuro/nn1x.h"
#include "../pneuro/nnifc.h"
#include "../pneuro/nnTraining.h"
#include "../pneuro/nnGATraining.h"

#include <vector>
#include <math.h>



//#define EXP1 1
#define EXP2 1
//#define EXP3 1
#ifdef EXP1
int main(int argc, char *argv[])
{
	CNNIfc MyIfc;
	
	{
		CNNPerceptron<CNNLayer> MyPerceptron("FRET_percept.nn", MyIfc, static_cast<CActi>(acti_htan));

		CTrainingInstances MyInstatnces1("FRET_in_out-all.nni", MyIfc);
		CTrainingInstances MyInstatnces2;

		CNNIndividual MyIndividual(&MyPerceptron, &MyInstatnces1);

		printf("\nLipsic const is %le", MyInstatnces1.CalcLipshic());

		for (unsigned int i = MyInstatnces1.size()/20; i>0; --i)
		{// лишнее - детям
			unsigned int MaxELoc =0;
			{ // поиск максимальной ошибки
				SYGDATATYPE MaxError=MyIndividual.GetError(0);
				for (unsigned int ii = 1; ii<MyInstatnces1.size(); ++ii)
					if (MyIndividual.GetError(ii) > MaxError)
					{
						MaxError = MyIndividual.GetError(ii);
						MaxELoc = ii;
					}
			}
			// лишнее - детям
			MyInstatnces2.AddInstance(MyInstatnces1[MaxELoc]);
			MyInstatnces1.RemoveInstance(MaxELoc);
			printf("\nLipsic const is %le", MyInstatnces1.CalcLipshic());
			printf(" and %le", MyInstatnces2.CalcLipshic());
			getch();
		}

		//MyIfc.SaveInstanses("FRET_in_out-part1of20.nni", &MyInstatnces1);
		MyIfc.SaveInstanses("FRET_in_out-part2of20.nni", &MyInstatnces2);
	}


	return 0;
}
#endif

#ifdef EXP2
// разделение с учетом максимума константы Липшица
int main(int argc, char *argv[])
{
	CNNIfc MyIfc;

	{
		SYGDATATYPE **matrLip;
		
	
		int i,j;
		

		CNNForwardPropagation<CNNLayer> MyPerceptron("FRET_percept.nn", MyIfc, static_cast<CActi>(acti_htan));

		CTrainingInstances MyInstatnces1("FRET_in_out-all.nni", MyIfc);
		CTrainingInstances MyInstatnces2;

		CNNIndividual MyIndividual(&MyPerceptron, &MyInstatnces1);

		printf("\nLipsic const is %le", MyInstatnces1.CalcLipshic());
		unsigned int InitSize=MyInstatnces1.size();
	
		matrLip=new SYGDATATYPE*[InitSize];
			for (i=0;i<InitSize;i++)
				matrLip[i]=new double[i+1];
			for (i = MyInstatnces1.size()-1; i>=0; --i)
			{			
					for (j = i-1; j>=0;--j)
					{	
						//	printf("\n%d", j);
						matrLip[i][j] = MyInstatnces1.CalcLipshicFor(i,j);
					}
			}
		
		while(MyInstatnces1.size()>InitSize/2)
		{
				int MaxLocI =1;
				int MaxLocJ =1;
				SYGDATATYPE MaxLipshic=0;

			
			for (i = MyInstatnces1.size()-1; i>=0; --i)
			{// лишнее - взрослым
			 // поиск максимальной константы Липшица
			     if (matrLip[i][1]=-1) continue;
					for (j = i-1; j>=0;--j)
						if (matrLip[i][j]>MaxLipshic)
						{
							MaxLipshic = matrLip[i][j];
							MaxLocI = i+1;
							MaxLocJ = j+1;
						}
			}
			printf("\n%d", MyInstatnces1.size());
			// лишнее - взрослым
			MyInstatnces2.AddInstance(MyInstatnces1[MaxLocJ]);
		    MyInstatnces1.RemoveInstance(MaxLocJ);

			for (i = MaxLocJ-1; i<=MyInstatnces1.size()-1; ++i)
				for (j = 0; j<= MaxLocJ-2; ++j)
					matrLip[i][j]=matrLip[i+1][j];
			for (i = MaxLocJ; i<=MyInstatnces1.size()-1; ++i)
				for (j = MaxLocJ-1; j<= i-1; ++j)
					matrLip[i][j]=matrLip[i+1][j+1];




		}
			printf("\nLipsic const is %le", MyInstatnces1.CalcLipshic());
			printf(" and %le", MyInstatnces2.CalcLipshic());
			getch();
	
		MyIfc.SaveInstanses("FRET_in_out-part1.nni", &MyInstatnces1);
		MyIfc.SaveInstanses("FRET_in_out-part2.nni", &MyInstatnces2);
	}


	return 0;
}
#endif

#ifdef EXP3
//разделение по моему методу
//пока не сделано...

int main(int argc, char *argv[])
{
	CNNIfc MyIfc;

	{
		CNNPerceptron<CNNLayer> MyPerceptron("FRET_percept.nn", MyIfc, static_cast<CActi>(acti_htan));

		CTrainingInstances MyInstatnces1("FRET_in_out-all.nni", MyIfc);
		CTrainingInstances MyInstatnces2;

		CNNIndividual MyIndividual(&MyPerceptron, &MyInstatnces1);

		printf("\nLipsic const is %le", MyInstatnces1.CalcLipshic());
		unsigned int InitSize=MyInstatnces1.size();
		while(MyInstatnces1.size()>InitSize/2)
		{
				unsigned int MaxLocI =1;
				unsigned int MaxLocJ =1;
				SYGDATATYPE MaxLipshic=0;
			for (unsigned int i = MyInstatnces1.size()-1; i>0; --i)
			{// лишнее - взрослым
			 // поиск максимальной константы Липшица
					for (unsigned int j = i-1; j>0;--j)
						if (MyInstatnces1.CalcLipshicFor(i,j)>MaxLipshic)
						{
							MaxLipshic = MyInstatnces1.CalcLipshicFor(i,j);
							MaxLocI = i;
							MaxLocJ = j;
						}
			}
			// лишнее - взрослым
			MyInstatnces2.AddInstance(MyInstatnces1[MaxLocJ]);
			MyInstatnces1.RemoveInstance(MaxLocJ);
		}
			printf("\nLipsic const is %le", MyInstatnces1.CalcLipshic());
			printf(" and %le", MyInstatnces2.CalcLipshic());
			getch();
	
		MyIfc.SaveInstanses("FRET_in_out-part1.nni", &MyInstatnces1);
		MyIfc.SaveInstanses("FRET_in_out-part2.nni", &MyInstatnces2);
	}


	return 0;
}
#endif