#ifndef _LX_CNNTGA_INCLUDED___
#define _LX_CNNTGA_INCLUDED___

#include "nnTraining.h"
#include "../ga/1x_pga.h"

#include <conio.h>



// ���������������� ������������ ��������
class CNNTGA : public CNNTraining
{

public:
	void Train(CNNForwardPropagation<CNNLayer> *a_pNN,
		CTrainingInstances *a_pTI, double a_Error, unsigned int popsize=8)
	{
		// ��������� ����������, ������������� ������� �������� ���������� � ����������
		const unsigned int nI = a_pNN->GetINum();
		const unsigned int nO = a_pNN->GetONum();
        const unsigned int _NI = a_pTI->size();
		const unsigned int uiTIsize = a_pTI->size();
		assert(nI && nO && _NI && uiTIsize);
//        SYGDATATYPE Error;

		// ������ ����� ����
		unsigned int iBufSize;
		SYGDATATYPE *pNetBuf = a_pNN->GetBuf(iBufSize);

		// � ������ ���������� ��������� ���������
		CPopulation<CNNIndividual> MyPopulation(popsize, CNNIndividual(a_pNN, a_pTI));

		// debug output
		/*{
			for (unsigned int I = 0; I < MyPopulation.size(); ++I)
			{	char buf[16]="rrr---";
				itoa(I, buf+4, 10);				
				CNNIfc::Save2file(buf, &MyPopulation[I]);
			}
		}*/

		// ��������� ��������� ���� - ���� ��������� ��������.

		CGenAlgorithm<CNNIndividual, CNNStochastic> MyGenAlg;
	
		char lastchar = ' ';
		int iNumIter=0;
		
		CPopulation<CNNIndividual>::iterator Result = MyPopulation.FindBest();

		while (lastchar != 27/*Esc*/ && Result->GetUnfitness() > a_Error)
		{

			MyGenAlg.GenerationNext(MyPopulation);

			{ // �������� ������
				++iNumIter;
				double SumError=0;
				FILE *faaai=fopen("error-GA-inst.log","a");
				FILE *faaa=fopen("error-GA.log","a");
				Result = MyPopulation.FindBest();
	
				for (int ih=0; ih< a_pTI->size()-1; ++ih)
				{	SumError += Result->GetError(ih);
					fprintf(faaai, "%4.1e\t", Result->GetError(ih));
				}
				SumError += Result->GetError(a_pTI->size()-1);
				fprintf(faaai, "%4.1e\n", Result->GetError(a_pTI->size()-1));
				fprintf(faaa, "%le\t", SumError);


				//fprintf(faaa, "%u\t", ++iNumIter);
				//fprintf(faaa,"%d\n", MyPopulation.size());
			//	printf("\nThe worst is:");
			//	CNNIfc::printNN(MyPopulation.FindWorst());
				//printf("\tThe best is: %e", SumError);
			//	CNNIfc::printNN(MyPopulation.FindBest());
			//	printf("\nBest goals in processes:");
				if (iNumIter == 30)
					fprintf(faaa, "\n");

				fprintf(faaai, "\n");
				fclose(faaai);
				fclose(faaa);
			}
			lastchar = 
				iNumIter == 30? 27 : 0;
				//getch();

		}
		
		// ��������� ��������
		a_pNN->operator =(*MyPopulation.FindBest());
	}
};


#endif //_LX_CNNTGA_INCLUDED___
