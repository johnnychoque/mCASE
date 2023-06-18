#ifndef _INC_RNG_H
#define _INC_RNG_H

//#include <math.h>
#include <cmath>
#include <sys/time.h>
#include "mtrand.h"

class rng {
  public:
	static rng *instance();
	void endInstance(void);
	
	inline double uniform()
		{ return (drand());}
	inline long int uniform(int k) 
		{ return (irand() % (unsigned long int)k); }
	inline double uniform(double r) 
		{ return (r * uniform());}
	inline double uniform(double a, double b)
		{ return (a + uniform(b - a)); }
	inline double exponential()
		{ return (-log(uniform())); }
	inline double exponential(double r)
		{ return (r * exponential());}
	
  private:
	rng();
	rng(rng const&) {};
	static rng *m_instance;
	MTRand_int32 irand;
	MTRand drand;
};
#endif //  _INC_RNG_H