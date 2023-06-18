#include "movement.h"

#define DBG_LEVEL 1
#include "debug.h"

movement::movement(void)
{
	
}

movement::~movement(void)
{

}

point_t movement::genNewPosition(void)
{
	point_t pt;
	
	pt.m_x = rng::instance()->uniform(m_xMax);
	pt.m_y = rng::instance()->uniform(m_yMax);
	//DBG_INFO("New point x=%.2f y=%.2f", pt.m_x, pt.m_y);
	
	return pt;
}

double movement::initialPauseTime(void)
{
	return rng::instance()->uniform(m_minPauseTime, m_maxPauseTime);
}

void movement::printMovParameters(void)
{

	DBG_INFO("\n");
	DBG_INFO("m_movID=%.2f",m_movID);
	DBG_INFO("m_iniPos x=%.2f y=%.2f", m_iniPos.m_x, m_iniPos.m_y);
	DBG_INFO("m_finPos x=%.2f y=%.2f", m_finPos.m_x, m_finPos.m_y);
	DBG_INFO("m_currSpeed=%.2f", m_currSpeed);
	DBG_INFO("m_currDirection=%.2f", m_currDirection);
	DBG_INFO("m_newDirection=%.2f", m_newDirection);
	DBG_INFO("m_state=%d", m_state);
	DBG_INFO("m_nextEvent=%.2f", m_nextEvent);
	DBG_INFO("m_newiniPos x=%.2f y=%.2f", m_newiniPos.m_x, m_newiniPos.m_y);
	DBG_INFO("m_timeLeft=%.2f", m_timeLeft);
	DBG_INFO("m_model=%d",m_model);
}

void movement::updateNodePosition(void)
{
	DBG_IN("movement::updateNodePosition");

	//DBG_INFO("Change state from %d",m_state);
	
	m_movID++;
	if (m_state == PAUSED) {
		// I need to change to moving
		m_currSpeed = rng::instance()->uniform(m_minSpeed, m_maxSpeed);
		initiateNodeMovement();
		m_state = MOVING;
	}
	else if ((m_state == MOVING) && (m_timeLeft > 0)) {
		//m_state = MOVING;
		m_iniPos = m_newiniPos;
		m_currDirection = m_newDirection;
		initiateNodeMovement();	
	}
	else if ((m_state == MOVING) && (m_timeLeft == 0)){
		m_state = PAUSED;
		m_iniPos = m_finPos;
		m_currSpeed = 0;
		m_currDirection = 0;
		m_newDirection = 0;
		m_nextEvent = rng::instance()->uniform(m_minPauseTime,m_maxPauseTime);
	}
	else {
		DBG_ERR("ERROR FATAL! No deberia entrar aqui. m_state=%d m_timeLeft=%.2f",m_state,m_timeLeft);
	}

	DBG_OUT("movement::updateNodePosition");
}

/*
	Calcula nuevos valores para los parametros del movimiento. Para los casos en el que los
	modelos de movimiento sean diferentes a RWP_PlAIN aun falta determinar la validez de los
	nuevos valores.
*/

void movement::initiateNodeMovement(void)
{
    double dist_;

    DBG_IN("movement::initiateNodeMovement");

    //m_currSpeed = rng::instance()->uniform(m_minSpeed, m_maxSpeed);

    switch(m_model) {
    case RWP_PLAIN:
		m_finPos = genNewPosition();
		m_currDirection = atan(fabs((m_iniPos.m_y - m_finPos.m_y)/(m_iniPos.m_x - m_finPos.m_x)));
		if(m_finPos.m_x > m_iniPos.m_x) {
			if(m_finPos.m_y > m_iniPos.m_y) {
				// Not change the angle
			} else {
				m_currDirection = 2*PI - m_currDirection;
			}
		} else {
			if(m_finPos.m_y > m_iniPos.m_y) {
				m_currDirection = PI - m_currDirection;
			// Not change the angle
			} else {
				m_currDirection = PI + m_currDirection;
			}
		}
		dist_ = sqrt(pow(m_finPos.m_x - m_iniPos.m_x,2) + pow(m_finPos.m_y - m_iniPos.m_y,2));
		m_nextEvent = dist_ / m_currSpeed;
		//DBG_INFO("x_fin=%.2f y_fin=%.2f speed=%.2f m_nextEvent=%.2f",m_finPos.m_x, m_finPos.m_y, m_currSpeed, m_nextEvent);
		break;
    case RWP_WRAP:
	case RWP_REFLECT:
		if(m_state == PAUSED) {
			m_currDirection = rng::instance()->uniform(0,2*PI);
			m_nextEvent = rng::instance()->uniform(m_minTravelTime, m_maxTravelTime);
			calculateNewPosition(m_nextEvent);
		} else {
			// calcular la nueva direccion reflejada o wrapping
			calculateNewPosition(m_timeLeft);
		}
		break;
    default:
		break;
    }

    DBG_OUT("movement::initiateNodeMovement");
}

void movement::calculateNewPosition(double time_)
{
	double dist_;
	double xTime_, yTime_, edgeTime_;
	int dir_, edge_;

	DBG_IN("movement::calculateNewPosition");

	//DBG_INFO("NewPos ini currDirection = %.2f",m_currDirection);
	// We check whether the movement belongs to the first, second, third of fourth cuadrant
	if(m_currDirection > 0 && m_currDirection <= PI/2) {
		// First cuadrant -- moving to xMax and yMax
		dir_ = 1;
		xTime_ = (m_xMax - m_iniPos.m_x)/(m_currSpeed*cos(m_currDirection));
		yTime_ = (m_yMax - m_iniPos.m_y)/(m_currSpeed*sin(m_currDirection));		
	} else if(m_currDirection > PI/2 && m_currDirection <= PI) {
		// Second cuadrant -- moving to 0 and ymax
		dir_ = 2;
		xTime_ = -1.0*m_iniPos.m_x/(m_currSpeed*cos(m_currDirection));
		yTime_ = (m_yMax - m_iniPos.m_y)/(m_currSpeed*sin(m_currDirection));		
	} else if(m_currDirection > PI && m_currDirection <= 3*PI/2) {
		// Third cuadrant -- moving to 0 and 0
		dir_ = 3;
		xTime_ = -1.0*m_iniPos.m_x/(m_currSpeed*cos(m_currDirection));
		yTime_ = -1.0*m_iniPos.m_y/(m_currSpeed*sin(m_currDirection));
	} else {
		// Fourth cuadrant -- moving to xmax and 0
		dir_ = 4;
		xTime_ = (m_xMax - m_iniPos.m_x)/(m_currSpeed*cos(m_currDirection));
		yTime_ = -1.0*m_iniPos.m_y/(m_currSpeed*sin(m_currDirection));
	}
	// We need to know which of the edges is the one we reach first
	edgeTime_ = (xTime_ < yTime_) ? xTime_ : yTime_;
	// if we cross x - edge_ = 0
	// if we cross y - edge_ = 1
	edge_ = (xTime_ < yTime_) ? 0 : 1;
	//DBG_INFO("Edgetime = %.2f time = %.2f diff=%.2f",edgeTime_, time_, edgeTime_-time_);
	// We check wheter we cross an edge before reaching the final destination of the 
	// current movement
	if(edgeTime_ < time_) {
		// It reaches an edge before the final position
		if(edge_ == 0) { // Calcula el newiniPos cuando se cruza por el lado horizontal
			m_newiniPos.m_y = m_iniPos.m_y + m_currSpeed*edgeTime_*sin(m_currDirection);
			m_finPos.m_y = m_newiniPos.m_y;	
			switch(dir_) {
			case 1:
				if(m_model == RWP_WRAP) {
					m_newiniPos.m_x = 0;
					m_finPos.m_x = m_xMax;
					m_newDirection = m_currDirection;
				} else {
					m_newiniPos.m_x = m_xMax;
					m_finPos.m_x = m_xMax;
					m_newDirection = PI - m_currDirection;
				}
				break;
			case 2:
				if(m_model == RWP_WRAP) {
					m_newiniPos.m_x = m_xMax;
					m_finPos.m_x = 0;
					m_newDirection = m_currDirection;
				} else {
					m_newiniPos.m_x = 0;
					m_finPos.m_x = 0;
					m_newDirection = PI - m_currDirection;
				}
				break;
			case 3:
				if(m_model == RWP_WRAP) {
					m_newiniPos.m_x = m_xMax;
					m_finPos.m_x = 0;
					m_newDirection = m_currDirection;
				} else {
					m_newiniPos.m_x = 0;
					m_finPos.m_x = 0;
					m_newDirection = 3*PI - m_currDirection;
				}
				break;
			case 4:
				if(m_model == RWP_WRAP) {
					m_newiniPos.m_x = 0;
					m_finPos.m_x = m_xMax;
					m_newDirection = m_currDirection;
				} else {
					m_newiniPos.m_x = m_xMax;
					m_finPos.m_x = m_xMax;
					m_newDirection = 3*PI - m_currDirection;
				}
				break;
			}
		} else { // Calcula el newiniPos cuando se cruza por el lado vertical
			m_newiniPos.m_x = m_iniPos.m_x + m_currSpeed*edgeTime_*cos(m_currDirection);
			m_finPos.m_x = m_newiniPos.m_x;			
			switch(dir_) {
			case 1:
				if(m_model == RWP_WRAP) {
					m_newiniPos.m_y = 0;
					m_finPos.m_y = m_yMax;
					m_newDirection = m_currDirection;
				} else {
					m_newiniPos.m_y = m_yMax;
					m_finPos.m_y = m_yMax;
					m_newDirection = 2*PI - m_currDirection;
				}
				break;
			case 2:
				if(m_model == RWP_WRAP) {
					m_newiniPos.m_y = 0;
					m_finPos.m_y = m_yMax;
					m_newDirection = m_currDirection;
				} else {
					m_newiniPos.m_y = m_yMax;
					m_finPos.m_y = m_yMax;
					m_newDirection = 2*PI - m_currDirection;
				}
				break;
			case 3:
				if(m_model == RWP_WRAP) {
					m_newiniPos.m_y = m_yMax;
					m_finPos.m_y = 0;
					m_newDirection = m_currDirection;
				} else {
					m_newiniPos.m_y = 0;
					m_finPos.m_y = 0;
					m_newDirection = 2*PI - m_currDirection;
				}
				break;
			case 4:
				if(m_model == RWP_WRAP) {
					m_newiniPos.m_y = m_yMax;
					m_finPos.m_y = 0;
					m_newDirection = m_currDirection;
				} else {
					m_newiniPos.m_y = 0;
					m_finPos.m_y = 0;
					m_newDirection = 2*PI - m_currDirection;
				}
				break;
			}
		}
		m_timeLeft = time_-edgeTime_;
		m_nextEvent = edgeTime_;
		m_state = MOVING;

	} else {
		dist_ = m_currSpeed * time_;
		m_finPos.m_x = m_iniPos.m_x + dist_*cos(m_currDirection);
		m_finPos.m_y = m_iniPos.m_y + dist_*sin(m_currDirection);
		m_nextEvent = time_;
		m_timeLeft = 0;
	}
    //DBG_INFO("NewPos fin currDirection = %.2f",m_currDirection);
	//DBG_INFO("NewPos fin newDirection = %.2f",m_newDirection);
	DBG_OUT("movement::calculateNewPosition");
}
