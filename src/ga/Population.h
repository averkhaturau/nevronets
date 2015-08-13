#ifndef _LX_POPULATION_H_INCLUDED_
#define _LX_POPULATION_H_INCLUDED_

//#include "Genotype.h"
#include <vector>
#include "RandGen.h"
#include "../pneuro/nnTraining.h"

// ��������� �������� ����� � ����������� ������� ������
template <class CIndividual>
class CPopulation : protected std::vector<CIndividual>
{
	typedef CPopulation _MyT;
protected:
	void CalcUnfitness()
	{
		for (iterator Iter = begin(); Iter != end(); ++Iter)
		{
			Iter->CalcUnfitness();
		}
	};

public:
	CPopulation(){};
	CPopulation(const unsigned int uiCreationSize)
	{
		resize(uiCreationSize);
		//FillRand(a_RandGen);
	};
	// ������������� a_Individual ��� ��������� �����������
	CPopulation(const unsigned int uiCreationSize, CIndividual & a_Individual )
	{
		resize(uiCreationSize, a_Individual);
		unsigned int i=0;
		for (iterator Iter=begin(); Iter!=end(); ++Iter)
		{
			unsigned int uiNumgenes = Iter->GetNumGenes();
			//CNaturalHabitat<CIndividual>::Mutate< CNNStochastic >(*Iter);
			//CNNStochastic::Train(Iter,Iter->m_pTI,1e-7,1);
			double blablabla=-0.73;
			CNNStochastic::Train(&*Iter,Iter->m_pTI,1e-7, 50, blablabla);
			Iter->CalcUnfitness();
			Iter->CalcCellularPoint();
		}

	};
	~CPopulation(){};

	// ����������
	void AddIndividual(const CIndividual &a_Individual)
	{
		CIndividual Ind = a_Individual;
		push_back(Ind);
		back().CalcUnfitness();
		back().CalcCellularPoint();
	};
	// � ��������
	void KillIndividual(iterator a_Deseased)
	{
		erase(a_Deseased);
	};
	// ���������� � �������� ������������
	void SwapWith(iterator a_Deseased, const CIndividual &a_Individual)
	{
		((CGenotype*)(a_Deseased))->operator =(a_Individual);
		a_Deseased->CalcUnfitness();
	};

	void FillRand(CRandGen& a_RandGen)
	{
		a_RandGen.SetRandBuff(&*this->begin(),
			this->size()*sizeof(CIndividual));
		CalcUnfitness();
	};

	// returns end() if Population is empty
	iterator FindBest()
	{
		double dBestUnfitness = 1.7e308/*the maximum with minus*/, dCurrUnfitness;
		iterator BestIndividual = begin();
		for (iterator Iter = begin(); Iter != end(); ++Iter)
		{
			if ((dCurrUnfitness = Iter->GetUnfitness()) < dBestUnfitness)
			{
				dBestUnfitness = dCurrUnfitness;
				BestIndividual = Iter;
			}
		}

		return BestIndividual;
	};

	// returns end() if Population is empty
	iterator FindWorst()
	{
		double dWorstUnfitness = 0, dCurrUnfitness;
		iterator WorstIndividual = begin();
		for (iterator Iter = begin(); Iter != end(); ++Iter)
		{
			if ((dCurrUnfitness = Iter->GetUnfitness()) > dWorstUnfitness)
			{
				dWorstUnfitness = dCurrUnfitness;
				WorstIndividual = Iter;
			}
		}

		return WorstIndividual;

	};

	// end() ���� ��� ������
	iterator FindUnfitnessMore(const double& a_dUnfit, const iterator& a_begin)
	{
		iterator Iter = begin();
		for (; Iter != end(); ++Iter)
			if (Iter->GetUnfitness() >= a_dUnfit)
				break;

		return Iter;
	}

	double CalcMeanUnfitness()
	{
		double dMean =0;
		size_t uiCount = size();
		for (iterator Iter = begin(); Iter != end(); ++Iter)
		{
			// ����� ������ �� ������ ����������,
			// �� ��������� �������� ������������
			dMean += Iter->GetUnfitness() / uiCount;
		}
		return dMean;
	};


	void KillWithUnfitnessMoreThan(const double &dFitness, signed int uiMaxToKill)
	{
		iterator Iter = begin();
		while ( (end() != (Iter = FindUnfitnessMore(dFitness, Iter))) && (uiMaxToKill-- > 0) )
		{
			KillIndividual(Iter);
		}
	};

	// ��� ���������� ��: ����� ��������� � �������� ��� �������������
	iterator FindNearest(iterator CandidatNo1)
	{
		iterator IWeF = begin();
		if (size() >= 2)
		{
			double dCelPoint = CandidatNo1->GetCellularPoint();
			if (IWeF == CandidatNo1)
				++IWeF;
			double dCellDistance = dCelPoint - IWeF->GetCellularPoint();
			for (iterator Iter=begin()+1; Iter!=end(); ++Iter)
			{
				if (Iter == CandidatNo1)
					continue;
				double curDist = dCelPoint - Iter->GetCellularPoint();
				if (dCellDistance > curDist)
				{
					 dCellDistance = curDist;
					 IWeF = Iter;
				}
			}
		}
		return IWeF;
	};

	// ���������� ��������� �������� �������
public:
	inline size_type size() const
		{ return ((std::vector<CIndividual>*)this)->size(); };

	inline reference operator[](size_type _P)
		{ return ((std::vector<CIndividual>*)this)->operator[](_P); };

	inline const_reference operator[](difference_type _N) const
		{ return ((std::vector<CIndividual>*)this)->operator[](_N); };

	typedef iterator iterator;

	iterator begin(){return ((std::vector<CIndividual>*)this)->begin();};
};

#endif //_LX_POPULATION_H_INCLUDED_
