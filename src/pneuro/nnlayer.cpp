#include "nnlayer.h"
#include <assert.h>
#include <memory.h>

// конструктор по умолчанию - требует последующего вызова CNNLayer::Construct
CNNLayer::CNNLayer()
{
}

CNNLayer::CNNLayer(const unsigned int iThisLayerSize, const unsigned int iPrevLayerSize, CActi &a_funcacti)
{
	Construct(iThisLayerSize, iPrevLayerSize, a_funcacti);
}

void CNNLayer::Construct(const unsigned int iThisLayerSize, const unsigned int iPrevLayerSize, CActi &a_funcacti)
{
	m_iThisLayerSize = iThisLayerSize;
	m_iPrevLayerSize = iPrevLayerSize;

	m_pRates = new SYGDATATYPE[m_iPrevLayerSize*m_iThisLayerSize];
	m_pInput = 0;
		// new SYGDATATYPE[m_iPrevLayerSize];
	m_pDispl = new SYGDATATYPE[m_iPrevLayerSize];

	m_funcacti = a_funcacti;
}


CNNLayer::~CNNLayer()
{
	Destruct();
}

void CNNLayer::Destruct()
{
	delete[] m_pRates;
	delete[] m_pDispl;
}



void CNNLayer::SetRates(const SYGDATATYPE *a_pRates, const unsigned int size)
{
	assert(size == m_iThisLayerSize * m_iPrevLayerSize);
	memcpy(m_pRates, a_pRates, size*sizeof(SYGDATATYPE));
}

void CNNLayer::SetDispl(const SYGDATATYPE *a_pDispl, const unsigned int size)
{
	assert(size == m_iPrevLayerSize);
	memcpy(m_pDispl, a_pDispl, size*sizeof(SYGDATATYPE));
}


void CNNLayer::SetInput(const SYGDATATYPE *a_pInput, const unsigned int size)
{
	assert(size == m_iPrevLayerSize);
	//memcpy(m_pInput, a_pInput, size*sizeof(SYGDATATYPE));
	m_pInput = const_cast<SYGDATATYPE*>(a_pInput);
}

SYGDATATYPE *CNNLayer::GetRates(unsigned int &size)
{
	size = m_iThisLayerSize * m_iPrevLayerSize;
	return m_pRates;
}

SYGDATATYPE *CNNLayer::GetInput(unsigned int &size)
{
	size = m_iPrevLayerSize;
	return m_pInput;
}

SYGDATATYPE *CNNLayer::GetDispl(unsigned int &size)
{
	size = m_iPrevLayerSize;
	return m_pDispl;
}


void CNNLayer::GetAscons(SYGDATATYPE *a_pOutput, const unsigned int size)
{
	assert(size == m_iThisLayerSize);
	assert(m_pInput != 0);

	unsigned int i,ii;
	for (i=ii=0; i<m_iThisLayerSize; ++i,ii+=m_iPrevLayerSize)
	{
		SYGDATATYPE tempVal=0;
		for (unsigned int j=0;j<m_iPrevLayerSize;++j)
			tempVal+=m_pInput[j]*m_pRates[j+ii];
		a_pOutput[i] = (*m_funcacti.f)(tempVal);
	}
}
