#ifndef _LX_INDIVIDUAL_H__
#define _LX_INDIVIDUAL_H__

#include "../pneuro/nnTraining.h"
#include <math.h>

template <class tIndividual, class tTrainer>
	class CNaturalHabitat;

class CNNIndividual : public CNNForwardPropagation<CNNLayer>
{
	friend class CNaturalHabitat<CNNIndividual, CNNStochastic>;
	friend class CNaturalHabitat<CNNIndividual, CNNBackPropagation>;
	friend class CPopulation<CNNIndividual>;
protected:
	double m_dUnfitness;

	CTrainingInstances *m_pTI;

	double *d_Errors;

	double m_dCellularPoint[2];

public:
	double GetCellularPoint()const{return m_dCellularPoint[0];};
	void  CalcCellularPoint()
	{
		unsigned int uiSize;
		SYGDATATYPE *Vyborka = m_layers[0].GetRates(uiSize);
		SYGDATATYPE dMean=0, dDisp=0;
		unsigned int i;

		for (i = 0; i < uiSize; ++i)
		{
			dMean += Vyborka[i];
		}
		m_dCellularPoint[0] = dMean / uiSize;
		
		/*for (i = 0; i < uiSize; ++i)
		{
			dDisp += pow( dMean - Vyborka[i], 2 );
		}
		m_dCellularPoint[1] = dDisp;
		*/
	};

	void CalcUnfitness()
	{
		assert(m_pTI != 0);
		const unsigned int nI = this->GetINum();
		const unsigned int nO = this->GetONum();
        const unsigned int _NI = m_pTI->size();
		assert(nI && nO && _NI);

		unsigned int iBufSize;
		SYGDATATYPE *pNetBuf = this->GetBuf(iBufSize);
		
		{
			m_dUnfitness=0;
			for (unsigned int j_=0; j_<_NI; ++j_)
			{
				Propagate(
					(*m_pTI)[j_].GetInput(),  (*m_pTI)[j_].GetISize(),
					pNetBuf,                 (*m_pTI)[j_].GetOSize() );

				m_dUnfitness +=
					(d_Errors[j_] =
						CNNTraining::GetDiffNorm(pNetBuf,
						(*m_pTI)[j_].GetOutput(), (*m_pTI)[j_].GetOSize() )
					);
			}
		}
	};

	CNNIndividual(const CNNForwardPropagation<CNNLayer> *a_pNN, CTrainingInstances * a_pTI)
		: CNNForwardPropagation<CNNLayer>(*a_pNN)
	{
		//((CNNForwardPropagation<CNNLayer>*)this)->operator =(*a_pNN);
		m_pTI = a_pTI;
		d_Errors = new double[m_pTI->size()];
		CalcUnfitness();
		CalcCellularPoint();
	};

	// копирующий конструктор
	CNNIndividual(const CNNIndividual& gNew)
	: CNNForwardPropagation<CNNLayer>(gNew)
	{
		m_pTI = gNew.m_pTI;
		d_Errors = new double[m_pTI->size()];
		for (int ii=0; ii<m_pTI->size(); ++ii)
			d_Errors[ii] = gNew.GetError(ii); 
		m_dUnfitness = gNew.GetUnfitness();
		m_dCellularPoint[0] = gNew.GetCellularPoint();
	};

	~CNNIndividual()
	{
		delete d_Errors;
	};


	inline double GetUnfitness() const
	{
		return m_dUnfitness;
	};

	inline void SetUnfitness(const double &a_dUnfitness)
	{
		m_dUnfitness = a_dUnfitness;
	};

	CNNIndividual & operator =(const CNNIndividual& gNew)
	{
		((CNNForwardPropagation<CNNLayer>*)this)->operator =(gNew);
		m_pTI = gNew.m_pTI;

		m_dUnfitness = gNew.GetUnfitness();
		return *this;
	};

	unsigned int GetNumGenes()const 
	{
		unsigned int Result=m_layers[0].GetPrevLayerSize() * m_layers[0].GetThisLayerSize();
		/*for (unsigned int ii=0; ii< m_numlayers; ++ii)
		{
			Result += m_layers[ii].GetPrevLayerSize() * m_layers[ii].GetThisLayerSize();
		}*/

		return Result;
	};
	// пока только для первого слоя
	SYGDATATYPE GetGene(const unsigned int iGene) const
	{// не оптимально, переделать с массивом, где храняться номера начала генов на слое нейронов
		unsigned int i1=0; unsigned int i2=m_layers[0].GetPrevLayerSize() * m_layers[0].GetThisLayerSize();
		/*for (unsigned int ii=0; ii< m_numlayers; ++ii, i2=i1)
		{
			i1 += m_layers[ii].GetPrevLayerSize() * m_layers[ii].GetThisLayerSize();
			// текущий слой в диапазоне от i2 до i1
			if (iGene<i1)
				return m_layers[ii].GetRates(i1)[iGene - i2];
		}
		assert(true);// здесь значит ошибка - не правильно задан индекс гена.*/
		return m_layers[0].GetRates(i1)[iGene%i2];
	};
	// пока только для первого слоя
	void SetGene(const unsigned int iGene, const SYGDATATYPE & Vuala)
	{// не оптимально, переделать с массивом, где храняться номера начала генов на слое нейронов
		unsigned int i1=0; unsigned int i2=m_layers[0].GetPrevLayerSize() * m_layers[0].GetThisLayerSize();
		/*for (unsigned int ii=0; ii< m_numlayers; ++ii, i2=i1)
		{
			i1 += m_layers[ii].GetPrevLayerSize() * m_layers[ii].GetThisLayerSize();
			// текущий слой в диапазоне от i2 до i1
			if (iGene<i1)
				m_layers[ii].GetRates(i1)[iGene - i2] = Vuala;
		}*/
		m_layers[0].GetRates(i1)[iGene % i2] = Vuala;
		//assert(true);// здесь значит ошибка - не правильно задан индекс гена.
	};

	double GetError(unsigned int ui) const
	{
		assert(ui < m_pTI->size());

		return d_Errors[ui];
	};
};

#endif // _LX_INDIVIDUAL_H__