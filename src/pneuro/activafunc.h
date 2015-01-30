#ifndef _LX_ACTIVAFUNC_H_INCLUDED_
#define _LX_ACTIVAFUNC_H_INCLUDED_

#include "gsettings.h"

#define ACTIFUNC(f_name) \
SYGDATATYPE f_name(const SYGDATATYPE& a_param)

typedef ACTIFUNC(t_actifunc);

t_actifunc f_sigmoid; //=1/(1+exp(lambda*x)
t_actifunc f_sin;     //=x*sin(x)
t_actifunc f_lin;     //=x
t_actifunc f_htan;    //=1-2/(1+exp(2*(omea-theta)))

// поизводные
t_actifunc dfds_sigmoid;
t_actifunc dfds_sin;
t_actifunc dfds_lin;
t_actifunc dfds_htan;


// А сама функция есть агрегация себя со своей производной, обратной, может чо ещё...
struct CActi
{
	t_actifunc *f;
	t_actifunc *dfds;
	CActi(t_actifunc f_=f_lin, t_actifunc dfds_=dfds_lin)
	{f=f_; dfds=dfds_;};
};

const CActi acti_sigmoid(f_sigmoid, dfds_sigmoid);
const CActi acti_sin(f_sin, dfds_sin);
const CActi acti_lin(f_lin, dfds_lin);
const CActi acti_htan(f_htan, dfds_htan);

#endif _LX_ACTIVAFUNC_H_INCLUDED_
