#include "basestation.h"
#include "geometry.h"

#define DBG_LEVEL 1
#include "debug.h"

basestation::basestation()
{
	DBG_IN("basestation::basestation");
	
	m_currLoad = 0;
	m_capacity = 0;
	m_maxPrice = 0;
	m_currPrice = 0;
	
	m_bsID = 0;
	m_bsTypeID = 0;
	m_opTypeID = 0;
	m_posX = 0;
	m_posY = 0;
	m_range = 0;
	m_bsPriceFactor = 0;
	m_opPriceFactor = 0;
	m_lowerThrLoad = 0;
	m_upperThrLoad = 0;
	
	DBG_OUT("basestation::basestation");
}

basestation::~basestation()
{
	DBG_IN("basestation::~basestation");

	DBG_OUT("basestation::~basestation");
}

point_t basestation::getPosition(void)
{
	point_t pos;
	
	pos.m_x = m_posX;
	pos.m_y = m_posY;
	
	return pos;
}

//float basestation::getcurrPrice(float bsAvaiRelatLoad, bool sameOP)
float basestation::getcurrPrice(float bsAvaiRelatLoad)
{
	float result;
	geometry gm;
	
	if (bsAvaiRelatLoad <= m_lowerThrLoad) {
		result = m_maxPrice;
	}
	else if (bsAvaiRelatLoad <= m_upperThrLoad) {
		result = gm.yValueLineEquation(m_upperThrLoad, m_bsPriceFactor*m_maxPrice,
						m_lowerThrLoad, m_maxPrice, bsAvaiRelatLoad);
	}
	else if (bsAvaiRelatLoad <= 1) {
		result = m_bsPriceFactor*m_maxPrice;
	}
	else {
		DBG_ERR("bsAvaiRelatLoad=%.2f cannot be bigger than 1",bsAvaiRelatLoad);
		exit(-1);
	}

	// if (!sameOP) {
		// result = result*(1+m_opPriceFactor);
	// }
	
	return result;
}
