
#include <stdio.h>
#include <conio.h>
//#include <curses.h>
#include <iostream>

#include <mpi.h>

#include "Genotype.h"
#include "Population.h"
//#include "Displaying.h"
#include "NaturalHabitat.h"
#include "GenAlgorithm.h"
#include "Displaying.h"


int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);
	
	int myrank, gsize;
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &gsize);

	CRandGen RandGen;
	RandGen.SetSeed(MPI_Wtime()*1e7);
	CPopulation<CNNIndividual> Population(15);

	//Population.SetFitnessFunction(&g_NHabitat);
	Population.FillRand(RandGen);
	//Population.FillRand(RandGen, Min, Max);

    CDisplaying D;


	if (myrank==0)
	{
	    printf("\n\n\nLet's test algorithm, when done press ""Esc""\n");
	}

	MPI_Barrier(MPI_COMM_WORLD);
	CGenAlgorithm<CNNIndividual, CNNBackPropagation> MyGenAlg;
	
	char lastchar = ' ';
	int iNumIter=0;
	while (lastchar != 27/*Esc*/)
	{

		MyGenAlg.GenerationNext(Population);
#if 0
		// посмотрим
		if ( myrank==0)
		{
			printf("\n\nIteration %u:", ++iNumIter);
		    D.Show(Population);
			printf("\nThe worst is:");
			D.Show(*Population.FindWorst());
			printf("\nThe best is:");
			D.Show(*Population.FindBest());
			printf("\nBest goals in processes:");

		}
#endif
		MPI_Barrier(MPI_COMM_WORLD);
		printf(" %11.3e;", Population.FindBest()->GetUnfitness());

		
		if (myrank==0)
		{
		    lastchar = getch();
		}
		
		MPI_Bcast(&lastchar, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}
