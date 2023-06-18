#ifndef _INC_USERTERMINAL_H
#define _INC_USERTERMINAL_H

#include <map>
#include <string>
#include <vector>
#include "event.h"
#include "movement.h"
#include "service.h"
#include "accessSelection.h"
#include "basestation.h"
#include "common.h"
#include "statistics.h"
#include <algorithm>

using namespace std;
typedef struct {
	bool state;
	servData_t *sData;
} stateData_t;

class userTerminal;

typedef map<int,userTerminal *> ut_t;
typedef ut_t::iterator ut_iter_t;

/*
	servState_t guarda el informacion acerca de los servicios que han sido asignados
	al terminal. El campo key es el tipo de servicio, mientras que stateData_t guarda 
	el estado del servicio (IDLE, ACTIVE) y un puntero al registro de servicios m_servReg
*/
//typedef map<int,bool> servState_t;
typedef map<int,stateData_t> servState_t;
typedef servState_t::iterator servState_iter_t;

/*
	currBSconn_t indica la BS por la que un servicio de una UT esta
	siendo enviada. El campo key es el tipo de servicio.
*/
typedef map<int,basestation *> currBSconn_t;
typedef currBSconn_t::iterator currBSconn_iter_t;

class userTerminal : public eventHandler {
	public:
		//userTerminal();
		userTerminal(accessSelection *&accSel_);
		~userTerminal();
		
		void onEvent(struct eventMessage *message);
		int &utID(void) {return m_utID;}
		int &userTypeID(void) {return m_userTypeID;}
		int &termTypeID(void) {return m_termTypeID;}
		int &myOperator(void) {return m_myOperator;}
		float &maxPrice(void) {return m_maxPrice;}
		//int &currOperator(void) {return m_currOperator;}
		
		void setMovementParams(int model, struct movParams params);
		int getMovModel(void);
		void setAreaLimits(double xMax, double yMax);
		void makeMovements(double simulTime);
		void setUTmoveNone(void);
		void initialConfiguration(void);
		//point_t makeMovPhoto(double currTime);
		point_t getcurrPosition(double currTime);
		point_t getIniPosition(void);
		void printMovementsToFile(FILE *movTraceFile_);		
		void insertMovement(movData_t *mov_);
		unsigned long int insertService(servData_t *serv_);
		
		void makeServices(double simulTime);
		void setServiceParams(userType_t ulist, serviceType_t slist);
		servState_t getServicesState(void);
		void cleanServState(int servType);
		
		void connectToBS(int servType, basestation *bs);
		void disconnectFromBS(int servType);
		basestation *findServConnected(int servType);

		void printServicesToFile(FILE *servTraceFile_);
		
		float &pmStep(void) {return m_pmStep;}
		void addPMevents(servData_t *sData);
		void cleanPMevents(servData_t *sData);
		void addCCevent(servData_t *sData, basestation *bs);
		void cleanCCevent(servData_t *sData, unsigned char code);
		float &lqStep(void) {return m_lqStep;}
		void addLQevents(servData_t *sData);
		void cleanLQevents(servData_t *sData);
		
	private:
	
		movement *m_movement;
		service *m_service;
		accessSelection *m_accSel;
		
		vector<movData_t *> m_movReg; // Registro de movimientos
		vector<servData_t *> m_servReg; // Registro de servicios
		
		int m_currMovIndex;
		//servData_t *m_currServReg;
		servState_t m_servState;
		currBSconn_t m_currBSconn;
		
		//! m_utID es el identificador del UT
		int m_utID;
		//! m_userTypeID define indirectamente los tipos de servicios que usará el usuario
		int m_userTypeID;
		//! m_termTypeID define indirectamente las RATS que usará el terminal 
		int m_termTypeID;
		//! m_myOperator define el operador preferido asignado durante la fase inicial de despliegue
		int m_myOperator;
		//! m_maxPrice define el precio maximo que el usuario esta dispuesto a pagar por un servicio
		float m_maxPrice;
		
		//! Se monitoriza el estado del servicio en cada intervalo de tiempo definido por m_pmStep
		float m_pmStep;
		//! Evalua la calidad del enlace del servicio en cada intervalo de tiempo definido por m_lqStep
		float m_lqStep;
		
		//! m_inCoverageBS guarda los bsIDs de las BS que tienen cobertura sobre el UT
		vector<int> m_inCoverageBS;
		
		void printMovement(movData_t *smov_);
		//point_t getcurrPosition(movData_t *movEvent_, double tDelta);

		void updateMovIndex(void);
		bool pointOutOfRange(point_t po);
		int updateServState(servData_t *sData);
		float getcurrServCapacity(servData_t *sData);
		void resourceManagementBS(servData_t *sData, unsigned char code);
		void requestResourcesToBS(float resourcesRequested_, servData_t *sData, unsigned char code);
		void deletePMevent(servData_t *sData, unsigned long int evID);
		void deleteLQevent(servData_t *sData, unsigned long int evID);		
		void searchCCeventsForServices(void);
		void detectLinkQuality(servData_t *sData);
};

#endif //  _INC_USERTERMINAL_H