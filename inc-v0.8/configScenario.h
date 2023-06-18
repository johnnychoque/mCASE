#ifndef _INC_CONFIGSCENARIO_H
#define _INC_CONFIGSCENARIO_H

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <string.h>
#include <vector>
#include <sstream>
using namespace std;

#include "configFile.h"
#include "common.h"
#include "RATtype.h"
#include "terminalType.h"
#include "userType.h"
#include "serviceType.h"
#include "movementType.h"
#include "BStype.h"
#include "operatorType.h"
#include "businessType.h"
//#include "accessSelection.h"

class configScenario {
	public:
		configScenario();
		~configScenario();
		int Configure(configFile configFile_);
		
		inline double &xMax(void) {return m_xMax;}
		inline double &yMax(void) {return m_yMax;}
		inline bool &toroidal(void) {return m_toroidal;}
		inline double &simulTime(void) {return m_simulTime;}
		inline double &stepTime(void) {return m_stepTime;}
		void addRATtype(int index_, double range_, int capacity_);
		void addTermType(int index_, float prob_, RATids_t RATids_);
		terminalType_t gettermTypeList(void) {return termTypeList;}
		inline int &numOfUsers(void) {return m_numOfUsers;}
		void addUserType(int index_, float prob_, userServs_t userServs_, float price_);
		userType_t getuserTypeList(void) {return userTypeList;}
		int getnumOfTypeUsers(void);
		void addServiceType(int index_, float tia_, float ts_,
				float capacity_,bool realtime_);
		serviceType_t getservTypeList(void) {return servTypeList;}	
		int getnumOfServices(void);
		void addMovementType(int index_, int model_, float movProb_,
				struct movParams params_);
		movType_t getmovTypeList(void) {return movTypeList;}
		void addBStype(int index_, int RATid_, double minDist_);
		void addOPtype(int index_, numOfBSlist_t numOfBSlist_, priceOfBSlist_t priceOfBSlist_,
				float marketProb_, float bsPriceFactor_, float opPriceFactor_);
		operatorType_t getOPtypeList(void) {return OPtypeList;}
		void addBusinessType(int index_, int type_, OPids_t OPids_);
		string &ssfilePrefix(void) {return m_ssfilePrefix;}
		int getBSminDistance(int BStypeID_);
		double getBSrange(int BStypeID_);
		double getBScapacity(int BStypeID_);
		bool &snapshotSetup(void) {return m_ssSetup;}
		int &traceFileSetup(void) {return m_traceFileSetup;}
		bool compareRATids(int termTypeID_, int bsTypeID_);
		float getBSprice(int BStypeID_, int OPtypeID_);
		float getUserPrice(int userTypeID_);
		float getbsPriceFactor(int OPtypeID_);
		float getopPriceFactor(int OPtypeID_);
		//friend void accessSelection::getRATtypeList(const configScenario &cfgSce_);
		float &lowerThrLoad(void) {return m_lowerThrLoad;}
		float &upperThrLoad(void) {return m_upperThrLoad;}
		float &hoDiscount(void) {return m_hoDiscount;}
		float &qualityWeight(void) {return m_qualityWeight;}
		float &handoverWeight(void) {return m_handoverWeight;}
		float &operatorWeight(void) {return m_operatorWeight;}
		float &loadWeight(void) {return m_loadWeight;}
		float &priceWeight(void) {return m_priceWeight;}
		float &pmStep(void) {return m_pmStep;}
		int &pmSetup(void) {return m_pmSetup;}
		float &lqStep(void) {return m_lqStep;}
		int &lqSetup(void) {return m_lqSetup;}
		float &ldStep(void) {return m_ldStep;}
		
	private:
		double m_xMax;
		double m_yMax;
		bool m_toroidal;
		double m_simulTime;
		double m_stepTime;		
		int m_numOfUsers;
		string m_ssfilePrefix; // Snapshot file prefix
		bool m_ssSetup;
		int m_traceFileSetup;
		
		//! Price-Load function Params 
		float m_lowerThrLoad;
		float m_upperThrLoad;
		
		//! Handover function param
		float m_hoDiscount;
		
		//! Weight Constraints
		float m_qualityWeight;
		float m_handoverWeight;
		float m_operatorWeight;
		float m_loadWeight;
		float m_priceWeight;
		
		//! Periodic monitoring params
		float m_pmStep;
		int m_pmSetup;

		//! Link quality params
		int m_lqSetup;
		float m_lqStep;

		//! Load monitoring param
		float m_ldStep;
		
		RATtype_t RATtypeList;
		terminalType_t termTypeList;
		userType_t userTypeList;
		serviceType_t servTypeList;
		movType_t movTypeList;
		BStype_t BStypeList;
		operatorType_t OPtypeList;
		businessType_t BMtypeList;
		
};

#endif // _INC_CONFIGSCENARIO_H
