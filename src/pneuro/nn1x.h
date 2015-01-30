#ifndef _LX_NN_H_INCLUDED____
#define _LX_NN_H_INCLUDED____

#include "nnlayer.h"
/*
	��� ��������� ����. ���� ���� �� ���� �����������, ����� �����
	������ ���������, ������������ � ���������������� ����������...
*/


template<class tLayer> // ������ ��� CNNLayer (�� �������� ���������)
class CNeuroNet
{
	// ������ ������� ����� ���������
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

	/// ������������ ������ ���� �������� - ����� ��� ��������
	unsigned int m_MAXLAYERSIZE;

	SYGDATATYPE *m_pBuf; // ����� ��� ������� ������ - ��� ��������������� ������

public:
	CNeuroNet(){m_layers=0; m_numlayers=0;};

	virtual ~CNeuroNet(){};
	//friend class CNNIfc;
	/** ��������� ������� ������, ���������� �������� - �������� �������.
	@param a_pInput ������ ������� ������
	@param a_pOutput ������ �������� ������
	@param i_size ������ ������� ������ - ������ ��� �������� ������������
	@param o_size ������ �������� ������ - ������ ��� �������� ������������
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


    /** ��������� ������������ ����� ��� �������� ��� ������
    �� ����� ��������� ����� - ���������� ���, ��� ��� ����
	@param Size ���������� ������ ������
	*/
    SYGDATATYPE *GetBuf(unsigned int & Size)
	{
		Size = m_MAXLAYERSIZE;
		return m_pBuf;
	};

};

#endif //_LX_NN_H_INCLUDED____
