#ifndef _INC_MOVEMENT_H
#define _INC_MOVEMENT_H

/**
 * movement class. Define los parametros de movimiento y los valores actuales de las
 * variables del movimiento para un userTerminal en particular.
 */
 
#include "rng.h"
#include "common.h"

class movement {
	public:
		movement();
	    ~movement();

		inline int &model(void) {return m_model;}
		inline double &xMax(void) {return m_xMax;}
		inline double &yMax(void) {return m_yMax;}
		inline double &minSpeed(void) {return m_minSpeed;}
		inline double &maxSpeed(void) {return m_maxSpeed;}
		inline double &minPauseTime(void) {return m_minPauseTime;}
		inline double &maxPauseTime(void) {return m_maxPauseTime;}
		inline double &minTravelTime(void) {return m_minTravelTime;}
		inline double &maxTravelTime(void) {return m_maxTravelTime;}
		inline double &minDirection(void) {return m_minDirection;}
		inline double &maxDirection(void) {return m_maxDirection;}

		inline double &movID(void) {return m_movID;}
		inline point_t &iniPos(void) {return m_iniPos;}
		inline point_t &finPos(void) {return m_finPos;}
		inline double &currSpeed(void) {return m_currSpeed;}
		inline double &currDirection(void) {return m_currDirection;}
		inline double &newDirection(void) {return m_newDirection;}
		inline int &state(void) {return m_state;}
		inline double &nextEvent(void) {return m_nextEvent;}
		inline double &timeLeft(void) {return m_timeLeft;}

		point_t genNewPosition(void);
		double initialPauseTime(void);
		void updateNodePosition(void);
		void initiateNodeMovement(void);
		void calculateNewPosition(double time_);
		
	private:
		int m_model;
		double m_xMax;
		double m_yMax;
		double m_minSpeed;
		double m_maxSpeed;
		double m_minPauseTime;
		double m_maxPauseTime;
		double m_minTravelTime;
		double m_maxTravelTime;		
		double m_minDirection;
		double m_maxDirection;
		
		double m_movID;
		point_t m_iniPos;
		point_t m_finPos;
		double m_currSpeed;
		double m_currDirection;
		
		int m_state;
		double m_nextEvent;
		double m_newDirection;
		point_t m_newiniPos;
		double m_timeLeft;
		
		void printMovParameters(void);
		
};

#endif // _INC_MOVEMENT_H