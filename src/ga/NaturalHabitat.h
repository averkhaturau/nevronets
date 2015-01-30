#ifndef _LX_NATURALHABITAT_H_
#define _LX_NATURALHABITAT_H_

#include "RandGen.h"
#include "Population.h"
#include "Individual.h"

/*
	Естественная среда обитания,
	которая и определяет поведение, жизнеспособность, и т.д.
*/

template <class tIndividual, class tTrainer>
	class CNaturalHabitat
{
public:
	CRandGen m_RandGen;
protected:
/*	static inline tIndividual CloneWithMinDiff(const tIndividual &a_Creature_1, const tIndividual &a_Creature_2)
	{
		tIndividual Clone = a_Creature_2;
		// исходя из конкретной функции
		// пока опустим
		return Clone;
	};
*/
public:
	static tIndividual Copulate(const tIndividual &a_Creature_1, const tIndividual &a_Creature_2)
	{
		// Воспользуемся симметричностью генов, если она есть
		// Для этого создадим "клон" с переставленными генами
		//	tIndividual *Ancestry[2];
		//Ancestry[0] = &static_cast<tIndividual>(a_Creature_1);
		//Ancestry[1] = &static_cast<tIndividual>(a_Creature_2);//CloneWithMinDiff(a_Creature_1, a_Creature_2);
		//Ancestry[1].SetUnfitness(a_Creature_2.GetUnfitness());

		// Соберём новую особь из родительских генов
		// вероятность одного из родителей оказать большее влияние
		// должна прямо зависеть от его фитнеса
		tIndividual Offspring(a_Creature_1);
		for (int i=0; i<Offspring.GetNumGenes(); ++i)
		{
			if (ncRandGen::randuint(0, 1))
				Offspring.SetGene(i, a_Creature_2.GetGene(i));
		}
		Offspring.CalcUnfitness();
		Offspring.CalcCellularPoint();

		return Offspring;
	};

	// может изменить особь до неузнаваемости, может не менять её
	static void Mutate(tIndividual& a_Individual)
	{ // Здесь реализуется одна из итераций классических методов

	//	unsigned int _NumGenes = a_Individual.GetNumGenes();

		{
			static double skorostshodimosti=0.1;
			tTrainer::Train( &a_Individual, a_Individual.m_pTI, 1e-7, 100, skorostshodimosti);
			a_Individual.CalcUnfitness();
			a_Individual.CalcCellularPoint();
		}
	};

	static void Migrate(CPopulation<tIndividual>& a_Population, const unsigned int a_MigrSize)
	{
/*		MPI_Status rcvStatus;
		int myrank, gsize;
		MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
		MPI_Comm_size(MPI_COMM_WORLD, &gsize );
		// Просто послать конец популяции, принять конец популяции
		MPI_Sendrecv_replace((void*)a_Population.begin(),
				sizeof(tIndividual)*a_MigrSize, MPI_CHAR,
			(myrank+1)%gsize, 7, myrank!=0?(myrank-1):(gsize-1), 7,
			MPI_COMM_WORLD, &rcvStatus
		);
*/			
	};

};

template<>
class CNaturalHabitat<CNNIndividual, CNNBackPropagation>
: public CNaturalHabitat<CNNIndividual, CNNStochastic>
{
public:
	void Mutate(tIndividual& a_Individual)
	{
		//  выберем пример, наиболее расходящийся с выходом сети
		unsigned int MaxI=0;
		a_Individual.CalcUnfitness();
		{
			double MaxE;
			MaxE = a_Individual.GetError(MaxI);
			for (unsigned int ii = 1; ii < a_Individual.m_pTI->size(); ++ii)
			{
				if (MaxE < a_Individual.GetError(ii))
				{
					MaxI = ii;
					MaxE = a_Individual.GetError(MaxI);
				}
			}
		}
		CNNBackPropagation::Train( &a_Individual, a_Individual.m_pTI, 1e-7, 1, MaxI);
		a_Individual.CalcUnfitness();
		a_Individual.CalcCellularPoint();
	};
};


#endif //_LX_NATURALHABITAT_H_
