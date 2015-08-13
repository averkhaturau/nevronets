project(ga)


set(SRC
	1x_pga.h
	Displaying.h
	GenAlgorithm.h
	Genotype.h
	Individual.h
	LibRand.cpp
	LibRand.h
#	main.cpp
	NaturalHabitat.h
	Population.h
	RandGen.h
)

add_library(${PROJECT_NAME} ${SRC})
