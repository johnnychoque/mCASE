#ifndef _INC_COMMON_H
#define _INC_COMMON_H

#include <vector>
#include <cstdlib>

using namespace std;

#define PI 3.14159265

typedef struct {
    double m_x;
    double m_y;
} point_t;

/*enum {
	CONSUMER,
	BUSINESS
};*/

enum {
	MOVNONE,
	RWP_PLAIN,
	RWP_REFLECT,
	RWP_WRAP
};

struct movParams {
	double minSpeed;
	double maxSpeed;
	double minPauseTime;
	double maxPauseTime;
	double minTravelTime;
	double maxTravelTime;		
	double minDirection;
	double maxDirection;	
};

typedef struct {
	int utID;
	int movID;
	point_t iniPos; // Initial Position
	point_t finPos; // temporalmente added
	double direction;
	double speed;
	double tStart;
} movData_t;

typedef struct {
	//int utID;
	int callID;
	int servType;
	double tEnd;
	double tWhen;
	int state;
	unsigned long int endEvID;
	vector<unsigned long int> listPMevIDs;
	unsigned long int ccEvID;
	vector<unsigned long int> listLQevIDs;
	float userPrice;
} servData_t;

enum {
	COOPERATIVE,
	COMPETITIVE
};

// for movements
enum {
	PAUSED,
	MOVING
};

// for services
enum {
	IDLE,
	ACTIVE
};

// for events
enum {
	NULL_EVENT,
	UT_MOVEMENT_EVENT,
	UT_SERVICE_EVENT,
	SNAPSHOT_EVENT,
	UT_MONITORING_EVENT,
	UT_CC_EVENT,
	UT_LINKQUALITY_EVENT,
	BS_LOADMONITORING_EVENT
};

// for traces
enum {
	NO_TRACES,
	MAKE_TRACES,
	LOAD_TRACES
};

// for periodic monitoring and link quality
enum {
	MONITORING_ON,
	MONITORING_OFF,
	LINKQUALITY_ON,
	LINKQUALITY_OFF
};

// for call results
enum {
	SUCCESS_CALL,
	REJECT_CALL,
	DROP_CALL
};

#define ResultCode_ERROR	0x9001
#define ResultCode_OK		0x9002

#endif  // _INC_COMMON_H
