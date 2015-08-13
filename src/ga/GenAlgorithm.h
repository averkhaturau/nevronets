#ifndef _LX_GENALGORITHM_H_
#define _LX_GENALGORITHM_H_

#include "Population.h"
#include "RandGen.h"
#include "NaturalHabitat.h"

// В этом классе инкапсулированы особенности алгоритма
template<class tIndividual, class tMutateProc>
class CGenAlgorithm
{

protected:
	// Настройки работы алгоритма
	unsigned int m_HalfMaxNumCreatures;
	unsigned int m_MaxMigrating;

public:

	CGenAlgorithm(const unsigned int& a_MaxNumCreatures = 9)
	{
		m_HalfMaxNumCreatures = (a_MaxNumCreatures>>1) + 1;
		m_MaxMigrating = (m_HalfMaxNumCreatures>>1) + 1;
	};

	void GenerationNext(CPopulation<tIndividual> &a_Population)
	{
		unsigned int popsize = a_Population.size();
		int i; // только для совместимости с M$ Visual C++
		// кроссинговер
		CNaturalHabitat<tIndividual, tMutateProc> MyNH;
		for (i=0; i < popsize ; ++i)
		{
			CPopulation<tIndividual>::iterator Candidat1
				= a_Population.begin() + ncRandGen::randuint(0, popsize-1);

			CPopulation<tIndividual>::iterator Candi2=a_Population.FindNearest(Candidat1);
			a_Population.AddIndividual(
				MyNH.Copulate( *Candidat1, *a_Population.FindNearest(Candidat1)
			));
		}

		// mutation
		for (i=0; i<2*popsize; ++i)
			MyNH.Mutate(a_Population[(unsigned int)ncRandGen::randuint(0, 2*popsize-1 )]);

		// миграция
		MyNH.Migrate(a_Population, m_MaxMigrating);

		// selections

		// быстрая селекция
	//	a_Population.KillWithUnfitnessMoreThan(
	//			a_Population.CalcMeanUnfitness(),
	//			2*popsize-2);

		// чтобы сильно не разрастаться, выкинем наихудших
		while (popsize < a_Population.size())
		{
			a_Population.KillIndividual(a_Population.FindWorst());
		}
	};

};

#endif //_LX_GENALGORITHM_H_
