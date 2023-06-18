#ifndef _INC_GEOMETRY_H
#define _INC_GEOMETRY_H

#include "common.h"
using namespace std;

typedef struct {
	double m_x;
	double m_y;
	double radius;
} circle_t;

class geometry {
	public:
		geometry();
		~geometry();
		
		bool insideCircle(point_t p, circle_t c);
		point_t intersection(point_t p1, point_t p2, circle_t c);
		bool isBetween(point_t a, point_t b, point_t c);
		double distancePoints(point_t pos1, point_t pos2);
		float yValueLineEquation(float x1, float y1, float x2, float y2, float xValue);		
	private:
	
};

#endif  // _INC_GEOMETRY_H