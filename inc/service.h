#ifndef _INC_SERVICE_H
#define _INC_SERVICE_H

#include "rng.h"
#include "common.h"
#include "serviceType.h"
#include "userType.h"

typedef struct {
	float m_tia; // InterArrival Time
	float m_ts;  // Service Time
	float m_capacity;
	bool m_realtime;
	int m_servTypeID;
} servParams;

typedef map<int, servParams*> utServs_t;
typedef utServs_t::iterator utServs_iter_t;

//typedef vector<int> utServs_t;

class service {
	public:
		service();
	    ~service();

		void setServicesAssigned(userServs_t usVector, serviceType_t slist);
		utServs_t getServicesAssigned(void) {return m_utServs;}
		vector<int> getServTypeIDAssigned(void);
		void createServicesEvents(double simulTime, vector<servData_t *> &serv);
		
		// void setServiceTypeList();
		// inline void setUTservs(utServs_t utServs_) {m_utServs = utServs_;}

	private:
		unsigned int m_callID;
		//unsigned short int m_type;
		int m_state;
		double m_eventTime;
		
		utServs_t m_utServs;
		
};	

#endif // _INC_SERVICE_H