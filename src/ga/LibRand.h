//*****************************************************************************
//* Library of Stochastic generators and utilities. Headers file.
//*    Initial routines from "Numerical Recipes" by Press, Teukolsky, Vetterling, Flanery
//*    Realized by P.V.Nazarov, 2002
//*****************************************************************************

#ifndef LIBRAND_H_20020801
#define LIBRAND_H_20020801

#define _I_A 16807
#define _I_M 2147483647
#define _A_M (1.0/_I_M)
#define _I_Q 127773
#define _I_R 2836
#define _MASK 123459876
#define _NTAB 32
#define _NDIV (1+(_I_M-1)/_NTAB)
#define _EPS 1.2e-15
#define _RNMX (1.0-_EPS)

static
long RANDOM_SEED=0x321;

static
char RANDU_METHOD=1;

long SetRandomSeed(void);
long SetRandomSeed(long seed);
long GetRandomSeed(void);
void SetRanduMethod(char method);

float randu0(void);
//"Minimal" random number generator of Park and Miller. Returns a
//uniform random deviate between 0.0 and 1.0. Set or reset RANDOM_SEED
//to any integer value (except the unlikely value MASK) to initialize
//the sequence; RANDOM_SEED must not be altered between calls for successive
//deviates in a sequence.


float randu1(void);
//"Minimal" random number generator of Park and Miller with Bays-
//Durham shuffle and added safeguards. Returns a uniform random
//deviate between 0.0 and 1.0 (exclusive of the endpoint values).
//Call with RANDOM_SEED a negative integer to initialize; thereafter,
//do not alter RANDOM_SEED between successive deviates in a sequence.
//RNMX should approximate the largest floating value that is less than 1.

float randu(void);
//Generate random value in [0,1] range by method, determined by RANDU_METHOD
double randu(double a,double b);
//Generate random value in [a,b] range by method, determined by RANDU_METHOD

float randn(void);
//Returns a normally distributed random value with zero M=0 and STD=1, 
//using randu() as the source of uniform deviates.

double randn(double m,double d);
//Returns a normally distributed random value with zero M=m and STD=d, 
//using randu() as the source of uniform deviates.

int randuint(int a, int b);
//Returns a uniformly distributed integer random value from range [a,b], 
//using randu() as the source of uniform random.


#endif
