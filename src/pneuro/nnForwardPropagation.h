#ifndef _LX_NN_H_INCLUDED___
#define _LX_NN_H_INCLUDED___

#include "gsettings.h"

#include "nn1x.h"
/*
	Это перцептон - простейшая нс.
*/

template<class tLayer>
class CNNForwardPropagation : public CNeuroNet<tLayer>
{
	friend class CNNBackPropagation;
	friend class CNNTraining;
	friend class CNNForwardPropagation<CNNLayer_store>;
public:

	CNNForwardPropagation()
		: CNeuroNet<tLayer>()
	{
	};
	/** Конструктор перцептрона
	@param a_NumLayers - количество слоёв
	@param a_pNumSizes - количество нейонов в каждом слое + количество входов
	(массив с размерностью на 1 больше a_NumLayers) a_pNumSizes[i] - ширина i-го слоя
	@param a_funcacti  - активационная функция
	*/
	explicit CNNForwardPropagation(
		unsigned int a_NumLayers, unsigned int *a_pNumSizes, CActi &a_acti)
	{// a_NumLayers слоёв, a_pNumSizes[i] - ширина i-го слоя

		m_MAXLAYERSIZE = a_pNumSizes[0];
		m_numlayers = a_NumLayers;
		m_layers = new CNNLayer[a_NumLayers];
		for (unsigned int i=1; i<=a_NumLayers; ++i)
		{
			m_layers[i-1].Construct(a_pNumSizes[i], a_pNumSizes[i-1], a_acti);
			// считаем размер буфера
			if (m_MAXLAYERSIZE < a_pNumSizes[i])
				m_MAXLAYERSIZE = a_pNumSizes[i];
		}
		m_pBuf = new SYGDATATYPE[m_MAXLAYERSIZE];
	};

	CNNForwardPropagation(const CNNForwardPropagation<CNNLayer> &a_NN)
	{
		unsigned int ui; SYGDATATYPE *p;
		m_numlayers = a_NN.m_numlayers;
		m_layers = new tLayer[m_numlayers];
		for (unsigned int i=0; i<m_numlayers; ++i)
		{
			m_layers[i].Construct(
				a_NN.m_layers[i].GetThisLayerSize(),
				a_NN.m_layers[i].GetPrevLayerSize(),
				a_NN.m_layers[i].GetActiFunc()    );

			p=a_NN.m_layers[i].GetRates(ui);
			m_layers[i].SetRates(p, ui);

			p=a_NN.m_layers[i].GetDispl(ui);
			m_layers[i].SetDispl(p, ui);
		}

		m_MAXLAYERSIZE = a_NN.m_MAXLAYERSIZE;
		m_pBuf = new SYGDATATYPE[m_MAXLAYERSIZE];
	};

	// чтение из файла
	CNNForwardPropagation(char *filename, CNNIfc &a_Ifc, CActi &a_acti)
	{
		a_Ifc.ReadFromFile(filename,this, a_acti);
	};

	virtual ~CNNForwardPropagation()
	{
		delete[] m_pBuf;
		delete[] m_layers;
	};


	/** Принимаем входной вектор, возвращаем выходной - основная функция.
	@param a_pInput Вектор входных данных
	@param a_pOutput Вектор выходных данных
	@param i_size Размер входных данных - только для контроля правильности
	@param o_size Размер выходных данных - только для контроля правильности
	*/
	virtual void Propagate(const SYGDATATYPE *a_pInput, const unsigned int i_size,
					SYGDATATYPE *a_pOutput, const unsigned int o_size)
	{
		// прогоняем данные по слоям

		m_layers[0].SetInput(a_pInput, i_size);
		for (unsigned int i=0; i<m_numlayers-1; ++i)
		{	unsigned int iSize;
			m_layers[i+1].GetInput(iSize);

			m_layers[i].GetAscons(m_pBuf, iSize);
			m_layers[i+1].SetInput(m_pBuf, iSize);
		}
		m_layers[m_numlayers-1].GetAscons(a_pOutput, o_size);
	};

	CNNForwardPropagation<tLayer> & operator = (const CNNForwardPropagation<tLayer> & a_NN)
	{
		delete[] m_layers;
		delete[] m_pBuf;

		unsigned int ui; SYGDATATYPE *p;
		m_numlayers = a_NN.m_numlayers;
		m_layers = new tLayer[m_numlayers];
		for (unsigned int i=0; i<m_numlayers; ++i)
		{
			m_layers[i].Construct(
				a_NN.m_layers[i].GetThisLayerSize(),
				a_NN.m_layers[i].GetPrevLayerSize(),
				a_NN.m_layers[i].GetActiFunc()    );

			p=a_NN.m_layers[i].GetRates(ui);
			m_layers[i].SetRates(p, ui);

			p=a_NN.m_layers[i].GetDispl(ui);
			m_layers[i].SetDispl(p, ui);
		}

		m_MAXLAYERSIZE = a_NN.m_MAXLAYERSIZE;
		m_pBuf = new SYGDATATYPE[m_MAXLAYERSIZE];

		return *this;
	};
};

#endif //_LX_NN_H_INCLUDED___
