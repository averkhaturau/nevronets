#ifndef _LX_NNSHOW_H_INCLUDED__
#define _LX_NNSHOW_H_INCLUDED__

// Здесь отладочный вывод нейронной сети
#include "activafunc.h"

class CNNLayer;

template<class _T>
class CNeuroNet;

class CTrainingInstances;

class CNNIfc
{

public:
	static void printNN(CNeuroNet<CNNLayer> *a_NN);
	static void Save2file(char *filename, CNeuroNet<CNNLayer> *a_NN);
	static void ReadFromFile(char *filename, CNeuroNet<CNNLayer> *a_NN, CActi &a_acti);

	static void ReadInstanses(char *filename, CTrainingInstances *a_Instncs);
	static void SaveInstanses(char *filename, CTrainingInstances *a_Instncs);
};

#endif //_LX_NNSHOW_H_INCLUDED__
