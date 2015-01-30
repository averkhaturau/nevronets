#ifndef _LX_DISPLAYING_H_
#define _LX_DISPLAYING_H_

#include "Genotype.h"
//#include "string.h"

/* Содержит функции для отображения наших классов */
class CDisplaying
{
public:
	void Show(const CGenotype &a_Genotype)
	{
		printf("\nGenes: [");

		for (int i=0; i<NUMGENES; ++i)
		{
			printf(" %11.3e", a_Genotype.GetGene(i));
		}
		printf(" ]");
	};

	void Show(const CIndividual &a_Individual)
	{
		Show((CGenotype)a_Individual);
		printf(" Goal=%11.3e", a_Individual.GetUnfitness());
	};

	void Show(const CPopulation<CIndividual> &a_Population)
	{
		const unsigned int pop_size = a_Population.size();
		printf("\nPopulation of %u individuals:", pop_size);
		for (int i=0; i<pop_size; ++i)
		{
			Show(a_Population.operator [](i));
		}
	};
};

#endif //_LX_DISPLAYING_H_
