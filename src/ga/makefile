# Generated automatically from Makefile.in by configure.
#ALL: GenAlgMPI
##### User configurable options #####

SHELL       = /bin/sh
ARCH        = LINUX
MPIR_HOME   = /usr/local/mpich/mpich-1.2.1
CC          = /usr/local/mpich/mpich-1.2.1/bin/mpicc
#CC          = gcc
CLINKER     = $(CC)
CCC         = /usr/local/mpich/mpich-1.2.1/bin/mpiCC
CCLINKER    = $(CCC)
AR          = ar crl
RANLIB      = ranlib
LOG_LIB     = -mpilog -lm
PROF_LIB    = -lmpe -lm
OPTFLAGS    = -O3
MPE_DIR     = /usr/local/mpich/mpich-1.2.1/mpe
MAKE        = make --no-print-directory
#
srcdir=/usr/local/creations/GenAlgMPI

VPATH=.:$(srcdir)
### End User configurable options ###
#.SUFFIXES:	.cc .C .f90

CFLAGS	  = $(OPTFLAGS) -Wall
CFLAGSMPE = $(CFLAGS) -I$(MPE_DIR)/include 
#CCFLAGS	  = $(CFLAGS)
#FFLAGS	  = '-qdpc=e' 
#FFLAGS	  = $(OPTFLAGS)
#EXECS	  = cpi systest srtest 
#ALL_EXECS = ${EXECS} fpi cpilog cpi_autolog hello++ iotest pi3f90 

all: GenAlgMPI clean


GenAlgMPI: main.cpp LibRand.cpp Displaying.h GenAlgorithm.h Genotype.h LibRand.h NaturalHabitat.h Population.h RandGen.h
	$(CCC) $(OPTFLAGS) -o Debug/GenAlgMPI main.cpp LibRand.cpp -lm


clean:
	/bin/rm -f *.o upshot rdb.* startup.* core


#main.o: main.cpp

#LibRand.o: LibRand.cpp

#.c.o:
#	$(CC) $(CFLAGS) -c $<
#.cc.o:
#	$(CCC) -c $<
#.C.o:
#	$(CCC) -c $<
