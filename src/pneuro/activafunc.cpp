#include "activafunc.h"
#include <math.h>

ACTIFUNC(f_sigmoid)
{
const double lambda=0.6;//magic number
	//double ssss=lambda*(double)a_param;
	double e = exp(lambda*(double)a_param);
	SYGDATATYPE a = SYGDATATYPE( 1.0f/(1.0f+e));
	return 2*a - 0.5f;
}

ACTIFUNC(f_sin)
{
	return static_cast<SYGDATATYPE>(a_param*sin(a_param));
}

ACTIFUNC(f_lin)
{
	return a_param;
}

ACTIFUNC(f_htan)
{
	return 1-2/(1+exp(2*(a_param)));
}

ACTIFUNC(dfds_sigmoid)
{
const double lambda=0.6;//magic number
	double e = exp(lambda*(double)a_param);
    double _1_e = e + 1.0;
	if (e == _1_e) // округилось?
		return 0;
	SYGDATATYPE a = -2 * lambda * e / (_1_e * _1_e);
	return a;
}

ACTIFUNC(dfds_sin)
{
	return sin(a_param) + a_param * cos(a_param) ;
}

ACTIFUNC(dfds_lin)
{
	return 1.0;
}

ACTIFUNC(dfds_htan)
{
	return 1.0 - a_param*a_param;
}

