//*****************************************************************************
//* Library of Stochastic generators and utilities. Routines file.
//*    Initial routines from "Numerical Recipes" by Press, Teukolsky, Vetterling, Flanery
//*    Realized by P.V.Nazarov, 2002
//*****************************************************************************

#include <time.h>
#include <math.h>

#pragma hdrstop

namespace ncRandGen
{

#include "LibRand.h"


//------------------------------------------------------------------------
long SetRandomSeed(void)
{
	time_t curr_time;
	time( &curr_time );
	RANDOM_SEED=(((long)curr_time)^((long)(&curr_time)))<<(((char)curr_time)&(0xF));
	RANDOM_SEED=(RANDOM_SEED&0x7FFFFFFF);
	RANDOM_SEED=-RANDOM_SEED;
	randu1();//randu1 initialization
	RANDOM_SEED=(((long)curr_time)^((long)(&curr_time)))<<(((char)curr_time)&(0xF));
	RANDOM_SEED=(RANDOM_SEED&0x7FFFFFFF);
	randu0();//randu0 initialization
	return RANDOM_SEED; 
}
long SetRandomSeed(long seed)
{
	RANDOM_SEED=seed^_MASK;//XORing with MASK allows use of zero and other simple bit patterns for idum. 
	return RANDOM_SEED; 
}
long GetRandomSeed(void)
{
	return RANDOM_SEED;
}

//-------------------------------------------------------------------------
float randu(void)
//Generate random value in [0,1] range by method, determined
//by RANDU_METHOD
{
	switch (RANDU_METHOD) {
  	case 1: return randu1();
    default:return randu0();
  }
}

double randu(double a,double b)
//Generate random value in [a,b] range by method, determined
//by RANDU_METHOD
{
  float r;
	switch (RANDU_METHOD) {
  	case 1: r=randu1();break;
    default:r=randu0();
  }
  return (float)(a+(b-a)*r);
}



//------------------------------------------------------------------
float randu0(void)
//"Minimal" random number generator of Park and Miller. Returns a 
//uniform random deviate between 0.0 and 1.0. Set or reset idum to
//any integer value (except the unlikely value MASK) to initialize
//the sequence; idum must not be altered between calls for successive
//deviates in a sequence.
{
	long k;
	float ans;
	k=(RANDOM_SEED)/_I_Q;
	RANDOM_SEED=_I_A*(RANDOM_SEED-k*_I_Q)-_I_R*k; //Compute idum=(IA*RANDOM_SEED) % IM without overflows by Schrage's method. 
	if (RANDOM_SEED < 0) RANDOM_SEED += _I_M;
	ans=(float)_A_M*(RANDOM_SEED); //Convert idum to a floating result.
	RANDOM_SEED ^= _MASK; //Unmask before return.
	return ans;
}

//---------------------------------------------------------------------
float randu1(void)
//"Minimal" random number generator of Park and Miller with Bays-
//Durham shuffle and added safeguards. Returns a uniform random 
//deviate between 0.0 and 1.0 (exclusive of the endpoint values).
//Call with RANDOM_SEED a negative integer to initialize; thereafter,
//do not alter RANDOM_SEED between successive deviates in a sequence.
//RNMX should approximate the largest floating value that is less than 1.
{
	int j;
	long k;
	static long iy=0;
	static long iv[_NTAB];
	double temp;
	
	if (RANDOM_SEED <= 0 || !iy) { //Initialize.
		if (-(RANDOM_SEED) < 1) RANDOM_SEED=1; //Be sure to prevent idum = 0.
		else RANDOM_SEED = -(RANDOM_SEED);
		for (j=_NTAB+7;j>=0;j--) { //Load the shuffle table (after 8 warm-ups).
			k=(RANDOM_SEED)/_I_Q;
			RANDOM_SEED=_I_A*(RANDOM_SEED-k*_I_Q)-_I_R*k;
			if (RANDOM_SEED < 0) RANDOM_SEED += _I_M;
			if (j < _NTAB) iv[j] = RANDOM_SEED;
		}
		iy=iv[0];
	}
	k=(RANDOM_SEED)/_I_Q; //Start here when not initializing.
	RANDOM_SEED=_I_A*(RANDOM_SEED-k*_I_Q)-_I_R*k; //Compute idum=(IA*RANDOM_SEED) % IM without overflows by Schrage's method. 
	if (RANDOM_SEED < 0) RANDOM_SEED += _I_M;
	j=iy/_NDIV; //Will be in the range 0..NTAB-1.
	iy=iv[j]; //Output previously stored value and refill the shuffle table. 
	iv[j] = RANDOM_SEED;
	if ((temp=_A_M*iy) > _RNMX) return (float)_RNMX; //Because users don't expect endpoint values.
	else return (float)temp;
}

//-----------------------------------------------------------------------------------
float randn(void)
//Returns a normally distributed random value with zero M=0 and STD=1, 
//using randu() as the source of uniform deviates.
{
	static int iset=0;
	static double gset;
	float fac,rsq,v1,v2;
	if (iset == 0) { //We don't have an extra deviate handy, so
		do {
			v1=(float)(2.0*randu()-1.0); //pick two uniform numbers in the square extending from -1 to +1 in each direction
			v2=(float)(2.0*randu()-1.0);
			rsq=(float)(v1*v1+v2*v2);	//see if they are in the unit circle,
		} while (rsq >= 1.0 || rsq == 0.0); //and if they are not, try again.
		fac=(float)sqrt(-2.0*log(rsq)/rsq);
		//Now make the Box-Muller transformation to get two normal deviates. Return one and save the other for next time.
		gset=v1*fac;
		iset=1; //Set flag.
		return (float)v2*fac;
	} else { //We have an extra deviate handy,
		iset=0; //so unset the flag,
		return (float)gset; //and return it.
	}
}

double randn(double m,double d)
//Returns a normally distributed random value with zero M=m and STD=d, 
//using randu() as the source of uniform deviates.
{
	if (d<0) return m;
	else return (d*randn()+m);
}

//-------------------------------------------------------------------------------
int randuint(int a, int b)
//Returns a uniformly distributed integer random value from range [a,b], 
//using randu() as the source of uniform random.
{
	return (int)randu((double)a,(double)b+_RNMX);
}

};

