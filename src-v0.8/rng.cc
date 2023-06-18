#include "rng.h"
#include <iostream>

rng *rng::m_instance = 0;

rng::rng()
{
	unsigned long init[4];
	unsigned long length = 4;
	struct timeval tv;
	
	/* i: heuristic sequence, always make sure we're different than last time.*/
	for (int i=0; i<(int)length; i++) {
		gettimeofday(&tv, 0);
		init[i] = (tv.tv_sec ^ tv.tv_usec ^ (i << 8)) & 0x7fffffff;
	}
	irand.seed(init, length);
}

rng *rng::instance()
{
	if (m_instance==0) {
		m_instance = new rng;
	}
	return m_instance;
}

void rng::endInstance()
{
	if (m_instance != 0) {
		delete m_instance;
	}
}

