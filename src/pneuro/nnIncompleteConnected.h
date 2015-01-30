#ifndef _LX_NNINCOMPLETECONNECTED_H_
#define _LX_NNINCOMPLETECONNECTED_H_

#include "nnForwardPropagation.h"

/*
	����� ����������� ����� ���������������� ��, ��������� ��
	������ �������������� ��, ����������� �� �����:
	
		��1 \
		��2 -  ��f 
		...
		��n /

	����� ������� ����� ����� �����������, ���������� �� �����
	������, ���� ������������� � �������� ������� ����������������
	�� �� ����� �� ���������� ���� �� ����.
*/

//CNNFullyConnected must be one of CNeuroNet succesors
template <class CNNFullyConnected>
class CNNIncompleteConnected
{
protected:
	CNNFullyConnected **m_pNN1metalayer; // ��������� �� ������� ���������� (������������)
	typedef CNNFullyConnected* CpNNFullyConnected;
	unsigned int m_nSubNN;
public:
	CNNIncompleteConnected()
	{
	};

	/**
	@param unsigned int a_NumSubNN -- ���������� ����������� ��������
	@param unsigned int *a_NumLayers -- ������ ������������ a_NumSubNN+1,
									���������� ���������� ���� �������� 
	@param unsigned int *a_pNumSizes -- ������ ������������ {����� a_NumLayers},
									���������� ���������� �������� � ������ ����
	@param CActi &a_acti -- ������������� ������� _�������_ �������
	*/
	explicit CNNIncompleteConnected(
			unsigned int a_NumSubNN,
			unsigned int *a_NumLayers,
			unsigned int *a_pNumSizes,
			CActi &a_acti)
	{
		m_nSubNN = a_NumSubNN;
		m_pNN1metalayer = new CpNNFullyConnected[a_NumSubNN+1];
		unsigned int i,ILayerDisp;
		for (i=0,ILayerDisp=0; i<=a_NumSubNN;
				++i, ILayerDisp+=a_NumLayers[i])
		{
			m_pNN1metalayer[i] = new CNNFullyConnected(
				a_NumLayers[i], &a_pNumSizes[ILayerDisp], a_acti);
		}

		m_pNN1metalayer[i] = new CNNFullyConnected(
			a_NumLayers[a_NumSubNN-1],
			&a_pNumSizes[ILayerDisp],a_acti);

	};

	explicit CNNIncompleteConnected(
		const CNNIncompleteConnected& a_Another1)
	: CNNFullyConnected(a_NumLayers[0],a_pNumSizes,a_acti)
	{
		m_nSubNN = a_NumSubNN;
		m_pNN1metalayer = new (CNNFullyConnected*)[a_NumSubNN];
		unsigned int i,ILayerSize;
		for (i=1,ILayerSize=a_NumLayers[0]; i<=a_NumSubNN;
				++i, ILayerSize+=a_NumLayers[i])
		{
			m_pNN1metalayer[i] = new CNNFullyConnected(
				a_NumLayers[i],&a_pNumSizes[ILayerSize],a_acti);
		}
	};

	~CNNIncompleteConnected()
	{
		Destruct();
	};

	virtual void Propagate(
		const SYGDATATYPE *a_pInput, const unsigned int *i_displs, const unsigned int *i_sizes, const unsigned int i_numsizes,
		SYGDATATYPE *a_pOutput, const unsigned int o_size)
	{
		assert(i_numsizes==m_nSubNN);
		unsigned int uiN;
		for (unsigned int i=0; i<m_nSubNN; ++i)
		{
			m_pNN1metalayer[i]->Propagate(
				&a_pInput[i_displs[i]], i_sizes[i],
				m_pNN1metalayer[m_nSubNN]->GetBuf(uiN), m_pNN1metalayer[m_nSubNN]->GetINum());
		}

		m_pNN1metalayer[m_nSubNN]->Propagate(
			m_pNN1metalayer[m_nSubNN]->GetBuf(uiN), m_pNN1metalayer[m_nSubNN]->GetINum(),
			a_pOutput, o_size);
	};

private:
	inline void Construct(const unsigned int NumML)
	{
		m_nSubNN = NumML;
	};
	inline void Destruct()
	{

		if (m_nSubNN != 0)
		{
			while(--m_nSubNN != 0)
				delete m_pNN1metalayer[m_nSubNN];
			delete[] m_pNN1metalayer;
		}
	};

};

#endif //_LX_NNINCOMPLETECONNECTED_H_
