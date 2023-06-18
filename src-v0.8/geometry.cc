#include "geometry.h"
//#include "math.h"
//#include "stdio.h"
#include <cmath>
#include <cstdio>
#include "stdlib.h"

#define DBG_LEVEL 1
#include "debug.h"

geometry::geometry() {}
geometry::~geometry() {}

/*
	Esta funcion determina si el punto "p" esta dentro del circulo "c"
*/
bool geometry::insideCircle(point_t p, circle_t c)
{
	double deltaX = p.m_x - c.m_x;
	double deltaY = p.m_y - c.m_y;

	DBG_INFO("User is at (%.2f,%.2f)",p.m_x,p.m_y);
	DBG_INFO("BS is at (%.2f,%.2f), with radius %.2f",c.m_x,c.m_y,c.radius);
	
	double deltaR = sqrt((double)(deltaX*deltaX+deltaY*deltaY));
	//printf("deltaR %.2f c.radius %.2f\n", deltaR, c.radius);
	//if (deltaR < c.radius) return true;
	//else return false;
	return (deltaR < c.radius);
}

/*
	Considerando que el punto p1 esta dentro del circulo "c" y que el punto p2 esta fuera o forma 
	parte de la circunferencia, esta funcion devuelve el punto de intersección entre el segmento de
	recta formado por los puntos p1 y p2, y la circunferencia "c".
*/
point_t geometry::intersection(point_t p1, point_t p2, circle_t c)
{
	double x1, y1, x2, y2, n;
	double m, z;
	double A, B, C;
	point_t p;

	// Ecuacion de la circunferencia; (x-a)^2 + (y-b)^2 = r^2
	double a = c.m_x;
	double b = c.m_y;
	double r = c.radius;
	
	if ((p1.m_x - p2.m_x) == 0) { // Ecuacion de la recta: x = n
		n = p1.m_x;
		// Coeficientes de la ecuacion de 2do. grado que determina el punto de interseccion		
		A = 1;
		B = -2*b;
		C = b*b + (n-a)*(n-a) - r*r;

		//printf("A=%.2f B=%.2f C=%.2f\n", A, B, C);
			
		y1 = (-B + sqrt(B*B - 4*A*C))/(2*A);
		y2 = (-B - sqrt(B*B - 4*A*C))/(2*A);

		//printf("y1=%.2f y2=%.2f\n", y1, y2);
		
		p.m_x = n;
		p.m_y = y1;
		if (isBetween(p1,p2,p)) {
			return p;
		}
		else {
			p.m_x = n;
			p.m_y = y2;		
			if (isBetween(p1,p2,p)) {
				return p;
			}
			else {
				DBG_ERR("Error en interseccion 1");
				exit (-1);
			}
		}	
	}
	else { 	// Ecuacion de la recta: y = m * x + z
		m = (double)((p1.m_y - p2.m_y)/(p1.m_x - p2.m_x));
		z = (double)(p1.m_y - m*p1.m_x);
		
		// Coeficientes de la ecuacion de 2do. grado que determina el punto de interseccion
		A = (double)(1+m*m);
		B = 2*m*(z-b) - 2*a;
		C = a*a + (z-b)*(z-b) - r*r;
		
		//printf("A=%.2f B=%.2f C=%.2f\n", A, B, C);
			
		x1 = (-B + sqrt(B*B - 4*A*C))/(2*A);
		x2 = (-B - sqrt(B*B - 4*A*C))/(2*A);

		//printf("x1=%.2f x2=%.2f\n", x1, x2);
		
		y1 = m*x1 + z;
		p.m_x = x1;
		p.m_y = y1;	
		if (isBetween(p1,p2,p)) {
			return p;
		}
		else {
			y2 = m*x2 + z;
			p.m_x = x2;
			p.m_y = y2;		
			if (isBetween(p1,p2,p)) {
				return p;
			}
			else {
				DBG_ERR("Error en interseccion 2");
				exit (-1);
			}
		}
	}
}

/*
	Asumiendo que el punto "c" pertenece a la recta formada por los puntos "a" y "b", esta funcion determina si 
	el punto "c" se encuentra entre los puntos "a" y "b"
*/

bool geometry::isBetween(point_t a, point_t b, point_t c)
{
	
	double dotproduct = (c.m_x - a.m_x) * (b.m_x - a.m_x) + (c.m_y - a.m_y)*(b.m_y - a.m_y);
	if (dotproduct < 0) return false;

	double squaredlengthba = (b.m_x - a.m_x)*(b.m_x - a.m_x) + (b.m_y - a.m_y)*(b.m_y - a.m_y);
	if (dotproduct > squaredlengthba) return false;

	return true;
}

double geometry::distancePoints(point_t pos1, point_t pos2)
{
	double distance, distanceX, distanceY;
	double xa_, ya_, xb_, yb_;
	
	DBG_IN("geometry::distancePoints");

	xa_ = pos1.m_x;
	ya_ = pos1.m_y;
	xb_ = pos2.m_x;
	yb_ = pos2.m_y;
	
	if(xa_ >= xb_) {
		distanceX = xa_ - xb_;
	} else {
		distanceX = xb_ - xa_;
	}

	if(ya_ >= yb_) {
		distanceY = ya_ - yb_;
	} else {
		distanceY = yb_ - ya_;
	}

	distance = pow(distanceX,2) + pow(distanceY,2);
	distance = sqrt(distance);

	DBG_OUT("geometry::distancePoints");
	
	return distance;
}

float geometry::yValueLineEquation(float x1, float y1, float x2, float y2, float xValue)
{
	float m,b;
	
	if (x1==x2) {
		DBG_ERR("Los valores de las abscisas deben ser diferentes");
		exit(-1);
	}
	
	m = (y2-y1)/(x2-x1);
	b = y1 - m*x1;
	
	return (m*xValue + b);
	
}