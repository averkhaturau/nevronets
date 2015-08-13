#ifndef _LX_NNLAYER_H_
#define _LX_NNLAYER_H_

#include "gsettings.h"
#include "activafunc.h"
#include <assert.h>
#include <memory.h>

/*
	���� - ����������� ������ � ����� ��������, ������� ����� �������������� ������������,
	�.�. � ������ ���� � �������� ������ ������, ������ �� ���������� ���� ������ ����������
	�� ���� ����� ����.
	������ ��� ������� ��������� ���������� ���������� ������������
	������� ����� ����� � ������� ���� ������ ���� � ������������� �������.
			acson = f_act(dotproduct(rates, inputs)

	��� �������� ��� ������� �������� ��������� �������� ��� �����:
			acson1 = f_act(dotproduct(rates1, inputs)
			...
			acsonN = f_act(dotproduct(ratesN, inputs)

	���
			acson_i = f_act(inputs_j^T * rates_ji)

	����, � ��� ���� ������� ����� rates_ji (�� ������� - �������, �� �������� - ��� ��� �����. �����)
	� ���� ������ ������, ���� ��� ����.
*/

/**
	���� ���� ������� ����� (������� � ����) � ������ ������� ������.
	����� ����� ������� = ����������� ������� ��.������ = ����� �������� �� ���������� ����.
	����� �������� ������� = ����� �������� �� ������� ����.

	� ������ ���������� ������ �������� �� ���� � ����, ���� ��� ���� � feedback,
	�� ��������� �� ������� ����� ����, ��� ��� �� ��������� ����, ��� � ����������
	�� ������� ������ (��������) ����� ����. ����� �������, ������ "����"
	��� ���� ��������� �� ��������.

	� ������ ������������ ���������� � ������ �������� ����� ���� ����,
	�� � ���� ����� ������ ����� �������� => ������ ����� ����� ������� �����,
	� ����������� ����� ������ ����� ������� �������, � �� ����� ��� �����
	����� ��������� �� ���� ���������. ���� ����� ���� ������ ����� ���������
	� ������� MPI_Allgatherv. ����� �������, ������ "����" �������� ���
	������������ � ���������������� ����������.
*/
class CNNLayer
{
	friend class CNNBackPropagation;
protected:
	/// �������
	SYGDATATYPE *m_pRates;
	/// ������ ������
	SYGDATATYPE *m_pInput;
	/// �������� ������
	SYGDATATYPE *m_pDispl;

	unsigned int m_iThisLayerSize;
	unsigned int m_iPrevLayerSize;

	CActi m_funcacti;

public:
	// ����������� �� ��������� - ������� ������������ ������ CNNLayer::Construct
	CNNLayer();
	/** �������� ���� ��������
	@param iThisLayerSize ����� �������� �� ���� ���� (����� ����� ������� (�������))
	@param iPrevLayerSize ����� �������� �� ���������� ���� (����� ����� ������ (���������))
	*/
	explicit CNNLayer(const unsigned int iThisLayerSize, const unsigned int iPrevLayerSize, const CActi &a_funcacti);
	/// ����������
	~CNNLayer();

	/** ������������� �������� ��� ������� �����
	@param a_pRates �������� ��������� ������� �����
	@param size ���������� ��������� (��� ��������)
	*/
	void SetRates(const SYGDATATYPE *a_pRates, const unsigned int size);

	/** ������������� �������� ��� ������� ������. ������������ ��� ���������� ������� ������ �� ��.
	@param a_pInput �������� ��������� ������� ������
	@param size ���������� ��������� (��� ��������)
	*/
	void SetInput(const SYGDATATYPE *a_pInput, const unsigned int size);

	/** ������������� �������� ��� ������� ��������.
	@param a_pDispl �������� ��������� ������� ��������
	@param size ���������� ��������� (��� ��������)
	*/
	void SetDispl(const SYGDATATYPE *a_pDispl, const unsigned int size);

	/** ���������� �������� ��� ������� �����
	@param size ���������� ���������� ��������� (��� ��������)
	@return pointer to matrix
	*/
	SYGDATATYPE *GetRates(unsigned int &size);

	/** ���������� �������� ��� ������� ��������
	@param size ���������� ���������� ��������� (��� ��������)
	@return pointer to matrix
	*/
	SYGDATATYPE *GetDispl(unsigned int &size);


	/** ���������� �������� ��� ������� ������. ������������ ��� ���������� ������� ������ �� ��.
	@param size ���������� ���������� ��������� (��� ��������)
	@return pointer to vector
	*/
	SYGDATATYPE *GetInput(unsigned int &size);


	/** ������������� �������� ��� ������� ������. ������������ ��� ���������� ������� ������ �� ��.
	@param a_pOutput ���������� �������� ��������� ������� ������� (�������)
	@param size ���������� ��������� (��� ��������)
	*/
	void GetAscons(SYGDATATYPE *a_pOutput, const unsigned int size);

	inline CActi GetActiFunc()const{return m_funcacti;};

	inline unsigned int GetThisLayerSize()const{return m_iThisLayerSize;};
	inline unsigned int GetPrevLayerSize()const{return m_iPrevLayerSize;};

protected:
	void Destruct();
public:
	void Construct(const unsigned int iThisLayerSize, const unsigned int iPrevLayerSize, const CActi &a_funcacti);
};

/**
	������������� ������ ���� ������, ������� ������ ����� � ����� ��������,
	 ��� ��������� ������������� �������� ��.
*/
class CNNLayer_store : public CNNLayer
{
protected:
	SYGDATATYPE *m_pSS; // ��������� ����������� �������

public:
	CNNLayer_store(){m_pSS=0;};
	void Construct(const unsigned int iThisLayerSize, const unsigned int iPrevLayerSize, CActi &a_funcacti)
	{
		CNNLayer::Construct(iThisLayerSize, iPrevLayerSize, a_funcacti);
		
		m_pInput = new SYGDATATYPE[m_iPrevLayerSize];
		m_pSS    = new SYGDATATYPE[m_iThisLayerSize];
	};

	~CNNLayer_store()
	{
		if (m_pSS)
			delete[] m_pSS;
		if (m_pInput)
			delete[] m_pInput;
	};

	void SetInput(const SYGDATATYPE *a_pInput, const unsigned int size)
	{
		assert(size == m_iPrevLayerSize);
		memcpy(m_pInput, a_pInput, size*sizeof(SYGDATATYPE));
		//m_pInput = const_cast<SYGDATATYPE*>(a_pInput);
	};

	void GetAscons(SYGDATATYPE *a_pOutput, const unsigned int size)
	{
		assert(size == m_iThisLayerSize);
		assert(m_pInput != 0);

		unsigned int i,ii;
		for (i=ii=0; i<m_iThisLayerSize; ++i,ii+=m_iPrevLayerSize)
		{
			SYGDATATYPE tempVal=0;
			for (unsigned int j=0;j<m_iPrevLayerSize;++j)
				tempVal+=m_pInput[j]*m_pRates[j+ii];
			m_pSS[i] = tempVal; // ��� ��� ����������
			a_pOutput[i] = (*m_funcacti.f)(tempVal);
		}
	};

	SYGDATATYPE *GetS()const{return m_pSS;};
};


#endif //_LX_NNLAYER_H_
