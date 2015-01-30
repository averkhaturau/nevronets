
#include "nnifc.h"
#include "nn1x.h"
#include <iostream>
#include <iostream.h> // -для совместимости
#include <assert.h>
#include "nnTraining.h"

void CNNIfc::printNN(CNeuroNet<CNNLayer> *a_NN)
{
	printf("\n------------------------------------------------------------------------------\n            =хщЁюээр  ёхЄ№ яюър эх тvтюфшЄё  эр ¤ъЁрэ\n------------------------------------------------------------------------------\n");
}

void CNNIfc::Save2file(char *filename, CNeuroNet<CNNLayer> *a_NN)
{
	FILE *fnn = fopen(filename, "w");
	if (fnn !=NULL)
	{
		unsigned int numlayers;
		fprintf(fnn, "numlayers=%d\n", numlayers=a_NN->m_numlayers);
		for (unsigned int i=0;i<numlayers; ++i)
		{
			SYGDATATYPE *_pRates;
			//SYGDATATYPE *_pInput;
			SYGDATATYPE *_pDispl;
			unsigned int _iThisLayerSize_X_PrevLayerSize;
			unsigned int _iPrevLayerSize;
			unsigned int j;

			_pRates = a_NN->m_layers[i].GetRates(_iThisLayerSize_X_PrevLayerSize);
			//_pInput = a_NN->m_layers[i].GetInput(_iPrevLayerSize);
			_pDispl = a_NN->m_layers[i].GetDispl(_iPrevLayerSize);

			fprintf(fnn, "\nrates%d:%d\n",i,_iThisLayerSize_X_PrevLayerSize);
			for (j=0;j<_iThisLayerSize_X_PrevLayerSize;++j)
				fprintf(fnn, "%le ",_pRates[j]);

			fprintf(fnn, "\ndispl%d:%d\n",i,_iPrevLayerSize);
			for (j=0;j<_iPrevLayerSize;++j)
				fprintf(fnn, "%1.0f ",_pDispl[j]);
			fprintf(fnn, "\n");
		}

		fclose(fnn);
	}
	else
	{
		fprintf(stderr, "\nerror: can't open %s for writing", filename);
		throw std::exception("can't open file for writing");
	}

}

void CNNIfc::ReadFromFile(char *filename, CNeuroNet<CNNLayer> *a_NN, CActi &a_acti)
{
	a_NN->m_MAXLAYERSIZE =0;
	FILE *fnn = fopen(filename, "r");
	if (fnn !=NULL)
	{
		int isok=0;

		unsigned int numlayers;
		char bufstr[100];
		do{
			isok=fscanf(fnn, "%c", bufstr, &numlayers);
		}while(bufstr[0] != '=');

		fscanf(fnn, "%d", &numlayers);
		// нужно создать эти слои
								if (a_NN->m_layers !=0)
									delete[] a_NN->m_layers;
								a_NN->m_layers = new CNNLayer[numlayers];
								a_NN->m_numlayers = numlayers;
		//заполнение слоёв
		for (int i=0; i<numlayers; ++i)
		{
			SYGDATATYPE *_pRates;
			//SYGDATATYPE *_pInput;
			SYGDATATYPE *_pDispl;
			unsigned int _iThisLayerSize_X_PrevLayerSize;
			unsigned int _iPrevLayerSize;
			unsigned int j;

			{// слово "rates%d:"
				char ci = itoa(i, bufstr, 10)[0];
				bufstr[0] = 0;

				for (j=0; bufstr[0] != ci; ++j)
				{
					isok=fscanf(fnn, "%c", &bufstr[0]);
				}

				isok=fscanf(fnn, "%c", &bufstr[j]);
				//assert(bufstr[j]==':');
			}
			isok=fscanf(fnn, "%d",&_iThisLayerSize_X_PrevLayerSize);

			_pRates = new SYGDATATYPE[_iThisLayerSize_X_PrevLayerSize];
			for (j=0;j<_iThisLayerSize_X_PrevLayerSize;++j)
				isok=fscanf(fnn, "%le", &_pRates[j]);

			{// слово "displ%d:"
				char ci = itoa(i, bufstr, 10)[0];
				bufstr[0] = 0;

				for (j=0; bufstr[0] != ci; ++j)
				{
					isok=fscanf(fnn, "%c", &bufstr[0]);
				}
				isok=fscanf(fnn, "%c", &bufstr[j]);
				//assert(bufstr[j]==':');
			}

			isok=fscanf(fnn, "%d",&_iPrevLayerSize);
			_pDispl = new SYGDATATYPE[_iPrevLayerSize];
			for (j=0;j<_iPrevLayerSize;++j)
				isok=fscanf(fnn, "%le",&_pDispl[j]);


			a_NN->m_layers[i].Construct(_iThisLayerSize_X_PrevLayerSize/_iPrevLayerSize,
				_iPrevLayerSize, a_acti);

			a_NN->m_layers[i].SetRates(_pRates, _iThisLayerSize_X_PrevLayerSize);
			a_NN->m_layers[i].SetDispl(_pDispl, _iPrevLayerSize);

			// позаботимся о макссайзе и буфере
			if (_iThisLayerSize_X_PrevLayerSize/_iPrevLayerSize > a_NN->m_MAXLAYERSIZE)
				a_NN->m_MAXLAYERSIZE = _iThisLayerSize_X_PrevLayerSize/_iPrevLayerSize;
			
		}
		a_NN->m_pBuf = new SYGDATATYPE[a_NN->m_MAXLAYERSIZE];
		fclose(fnn);
	}
	else
	{
		fprintf(stderr, "\nerror: can't open %s for reading", filename);
		throw std::exception("can't open file for reading");
	}
}


void CNNIfc::ReadInstanses(char *filename, CTrainingInstances *a_Instncs)
{
	FILE *fnn = fopen(filename, "r");
	if (fnn !=NULL)
	{
		char bufstr[100]; int isok;
		SYGDATATYPE *_pInput;
		SYGDATATYPE *_pOutput;
		unsigned int _iSize;
		unsigned int _oSize;
		unsigned int _NumInstncs;
		unsigned int j, jj;

			{// слово "Instances:"
				char ci = ':';
				bufstr[0] = 0;

				for (j=1; bufstr[j-1] != ci; ++j)
				{
					isok=fscanf(fnn, "%c", &bufstr[j]);
				}
			}
		isok=fscanf(fnn, "%d",&_NumInstncs);

		for (jj=0; jj<_NumInstncs; ++jj)
		{

			{// слово "Inputs:"
				char ci = ':';
				bufstr[0] = 0;

				for (j=1; bufstr[j-1] != ci; ++j)
				{
					isok=fscanf(fnn, "%c", &bufstr[j]);
				}

			}
			isok=fscanf(fnn, "%d",&_iSize);
			_pInput = new SYGDATATYPE[_iSize];
			
			for (j=0;j<_iSize;++j)
				isok=fscanf(fnn, "%le ",&_pInput[j]);


			{// слово "Outputs:"
				char ci = ':';
				bufstr[0] = 0;

				for (j=1; bufstr[j-1] != ci; ++j)
				{
					isok=fscanf(fnn, "%c", &bufstr[j]);
				}

			}
			isok=fscanf(fnn, "%d",&_oSize);
			_pOutput = new SYGDATATYPE[_oSize];
			
			for (j=0;j<_oSize;++j)
				isok=fscanf(fnn, "%le",&_pOutput[j]);

			// ключевая строчка
			a_Instncs->AddInstance(CTrainingInstance(_pInput, _iSize, _pOutput, _oSize));

			delete[] _pInput;
			delete[] _pOutput;
		}
		fclose(fnn);
	}
	else
	{
		fprintf(stderr, "\nerror: can't open %s for reading", filename);
		std::exception("can't open file for reading");
	}
}

void CNNIfc::SaveInstanses(char *filename, CTrainingInstances *a_Instncs)
{
	FILE *fnn = fopen(filename, "w");
	if (fnn !=NULL)
	{
		unsigned int _NumInstncs = a_Instncs->size();
		if (_NumInstncs == 0)
		{
			fprintf(fnn, "Instances:0\n");
			fclose(fnn);
			return;
		}

		SYGDATATYPE *_pInput;
		SYGDATATYPE *_pOutput;
		unsigned int _iSize=a_Instncs->begin()->GetISize();
		unsigned int _oSize=a_Instncs->begin()->GetOSize();
		unsigned int j, jj;

		fprintf(fnn, "Instances:%d\n", _NumInstncs);

		for (jj=0; jj<_NumInstncs; ++jj)
		{
			_pInput = a_Instncs->operator [](jj).GetInput();
			_pOutput = a_Instncs->operator [](jj).GetOutput();

			fprintf(fnn, "\nInputs:%d\n", _iSize);
			for (j=0;j<_iSize;++j)
				fprintf(fnn, "%le \t",_pInput[j]);


			fprintf(fnn, "\nOutputs:%d\n", _oSize);
			for (j=0;j<_oSize;++j)
				fprintf(fnn, "%le \t",_pOutput[j]);

		}
		fclose(fnn);
	}
	else
	{
		fprintf(stderr, "\nerror: can't open %s for reading", filename);
		std::exception("can't open file for reading");
	}

}
