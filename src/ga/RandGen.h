#ifndef _LX_RANDGEN_H_INCLUDED_
#define _LX_RANDGEN_H_INCLUDED_

#include <time.h>

namespace ncRandGen{
	#include "LibRand.h"
};

class CRandGen
{
	//int m_seed;
public:

	CRandGen()
	{
		ncRandGen::SetRandomSeed();
	};

	inline void SetSeed(const int &a_seed)
	{
		ncRandGen::SetRandomSeed(a_seed);
	};
	// для полноты
	//inline int GetSeed()const{return m_seed;};

	//int GetRand() {return rand();};

	inline static int GetRand(const int& a_min, const int& a_max)
		{ return ncRandGen::randuint(a_min, a_max); };

	double GetRandD()
	{
		return ncRandGen::randu();
	};

	double GetRandD(const double& dMin, const double& dMax)
	{
		return ncRandGen::randu(dMin, dMax);
	};

	void SetRandBuff(void* pBuff, unsigned int size)
	{
		// записываем интами - не работает
		/*unsigned int i=0;
		while(size>=sizeof(short int))
		{
			size-=sizeof(short int);
			((int*)pBuff)[i++] = rand();
		}*/
		// записываем чарами
		while(size>=1/*sizeof(char)*/)
		{
			--size;
			((char*)pBuff)[size] = (char)rand();
		}
	};

	inline double Gaussian(const double& Median, const double& sqrDisp)
	//Returns a normally distributed random value with zero M=m and STD=d, 
	//using randu() as the source of uniform deviates.
	{
		if (sqrDisp <= 0)
			return Median;
		else
			return(sqrDisp * ncRandGen::randn() + Median);
	};


};


#endif //_LX_RANDGEN_H_INCLUDED_
