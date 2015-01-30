#ifndef _LX_GENOTYPE_H_INCLUDED_
#define _LX_GENOTYPE_H_INCLUDED_

#define NUMGENES 4

#include <math.h>
#include <memory.h>
//#include <stdexcept>

// ������ ���� (��������� �����������).
// � ��� ������ ����� ������ CFitnessFunction
class CGenotype
{
	friend class CFitnessFunction;
protected:
	double m_Genes[NUMGENES];

public:

	double GetGene(const int i)const {return m_Genes[i];};
	void SetGene(const int i, const double a_NewGene){m_Genes[i]=a_NewGene;};

	CGenotype& operator =(const CGenotype & gNew)
	{
		memcpy(m_Genes, &gNew, sizeof(CGenotype));
		return *this;
	};

};

#ifndef _USE_CFitnessFunction
// ������ �������, ����������� ����������������
class CFitnessFunction
{
public:
	/*double GetFitness(const CGenotype *a_pGenotype)
	{
		double dFit = 0.5;
		// write real function
		for (int i=1; i<NUMGENES; ++i)
		{
			dFit += -pow(sin(a_pGenotype->GetGene(i) - i) , 2);
		}
		return dFit; 
	};*/

	// ������� �������
	double GetGoal(const CGenotype *a_pGenotype)
	{
		double dGoal = 0;

		// for test let's set ideal to 1, 2, 3, 4, 5...
		for (int i=0; i<NUMGENES; ++i)
		{
			dGoal += fabs(a_pGenotype->GetGene(i)-i-1);
		}

		return dGoal;
	};

	// �� ������ �� ������ �������������� �������, �����
	// �� ������ ������ �������������� � ���������, �.�.
	// ���� ���� �����������, ��� � �������,
	// �� ��� ����� ���� ���� � �� �� �����...

};
#endif

/*
It is clear that the individual who persecutes a man, his brother, because
he is not of the same opinion, is a monster.
- Voltaire
*/

// ����� = ������� + ������ ����������������
class CIndividual : public CGenotype
{
	typedef CIndividual _MyT;

protected:

	double m_dUnfitness;

	CFitnessFunction *m_pFitnessFunction;

public:
	
	void CalcUnfitness()
	{
		//if (!m_pFitnessFunction)
		//	throw std::exception("FitnessFunction not setted");

		m_dUnfitness = m_pFitnessFunction->GetGoal(this);
	};

	CIndividual(const CGenotype &a_Genotype)
	{
		((CGenotype*)this)->operator =(a_Genotype);
		m_pFitnessFunction = 0;
	};

	CIndividual(){};

	inline void SetFitnessFunction(CFitnessFunction *a_pFitnessFunction)
	{
		m_pFitnessFunction = a_pFitnessFunction;
	};

	/*inline */double GetUnfitness() const
	{
		return m_dUnfitness;
	};
	//��� �������
	inline void SetUnfitness(const double &a_dUnfitness)
	{
		m_dUnfitness = a_dUnfitness;
	};

	CIndividual& operator =(const CIndividual& gNew)
	{
		CGenotype::operator =(gNew);
		m_dUnfitness = gNew.GetUnfitness();
		return *this;
	};
};

#endif //_LX_GENOTYPE_H_INCLUDED_
