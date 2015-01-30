#ifndef _LX_NN_H_INCLUDED____
#define _LX_NN_H_INCLUDED____

#include "nnlayer.h"
/*
	Это нейронная сеть. Вещь сама по себе абстрактная, будет иметь
	разные топологии, параллельную и последовательную реализацию...
*/


template<class tLayer> // только для CNNLayer (не работает правильно)
class CNeuroNet
{
	// Лишних френдов потом поубирать
	friend class CNNIfc;
	friend class CNNTraining;
	friend class CNNBackPropagation;
	friend class CNeuroNet<CNNLayer_store>;
	friend class CNNStochastic;
	friend class CNNTGA;
	friend int main(int argc, char *argv[]);
protected:

	unsigned int m_numlayers;
	tLayer *m_layers;

	/// максимальная ширина слоя нейронки - нужно для прогонки
	unsigned int m_MAXLAYERSIZE;

	SYGDATATYPE *m_pBuf; // буфер для прогона данных - под ответственность класса

public:
	CNeuroNet(){m_layers=0; m_numlayers=0;};

	virtual ~CNeuroNet(){};
	//friend class CNNIfc;
	/** Принимаем входной вектор, возвращаем выходной - основная функция.
	@param a_pInput Вектор входных данных
	@param a_pOutput Вектор выходных данных
	@param i_size Размер входных данных - только для контроля правильности
	@param o_size Размер выходных данных - только для контроля правильности
	*/
	virtual void Propagate(const SYGDATATYPE *a_pInput, const unsigned int i_size,
		SYGDATATYPE *a_pOutput, const unsigned int o_size)=0;

	unsigned int GetINum()
	{
		unsigned int Result = 0;
		if (m_layers>0)
			m_layers[0].GetInput(Result);
		return Result;
	};
	unsigned int GetONum()
	{
		unsigned int Result = 0;
		if (m_layers>0)
			m_layers[m_numlayers-1].GetInput(Result);
		return Result;
	};


    /** Позволяет использовать буфер для прогонки вне класса
    Не нужно создавать буфер - используйе тот, что уже есть
	@param Size Возвращает размер буфера
	*/
    SYGDATATYPE *GetBuf(unsigned int & Size)
	{
		Size = m_MAXLAYERSIZE;
		return m_pBuf;
	};

};

#endif //_LX_NN_H_INCLUDED____
