#ifndef _INC_SCENARIO_H
#define _INC_SCENARIO_H

#include "userTerminal.h"
#include "basestation.h"
#include "rng.h"
#include "configScenario.h"
#include "accessSelection.h"
#include "event.h"
#include "common.h"
#include "geometry.h"
#include "statistics.h"
#include <fstream>

class scenario: public eventHandler {
	public:
		scenario();
		~scenario();
		void onEvent(struct eventMessage *message);
		int  makeConfiguration(configFile configFile_);
		void makeUTdeployment(void);
		void makeBSdeployment(void);
		void movementGenerator(void);
		void serviceGenerator(void);
		void movementPhotos(void);
		void setSnapshotEvents(void);
		void startSimulation(void);
		void CreateBSdeployFile(void);
		void ReadBSdeployFile(string BSdeployFilename = "results/BSs_list.txt");
		void CreateSS_BSdeployFile(void);
		void CreateUTdeployFile(void);
		bool IsSnapshotSetup(void);
		int  traceFileSetup(void);
		void CreateMovementTraceFile(void);
		void CreateServiceTraceFile(void);
		void ReadUTdeployFile(void);
		void ReadMovementTraceFile(void);
		void ReadServiceTraceFile(void);
		void setLoadMonitoringEvents(void);
		void dumpStatistics(void);
		
	private:
		void configureUT(int id_, userTerminal *ut_);
		void configureBS(int i, int j, operatorType *operator_);
		int setTerminalTypeID(userTerminal *ut_);
		bool validateTermTypeID(int termTypeID, userTerminal *ut_);
		int setUserTypeID(void);
		int setOperatorID(void);
		void makeSnapshots(void);
		void priceParameters(basestation *bs);
		void bsLoadMonitoring(void);
		void dumpServiceSummaryStats(void);
		void dumpUserSummaryStats(void);
		void dumpLinkQualityStats(void);
		void dumpHOperServStats(void);
		void dumpHOperUserStats(void);
		void dumpLoadStats(void);
		void dumpListOfParams(void);
		void dumpPriceStats(void);
		void dumpPricePerServStats(void);
		void dumpPricePerUserStats(void);
		
		movementType *setMovementType(void);
		movementType *getMovementType(int model);
		
		configScenario *m_cfgSce;
		accessSelection *m_accSel;
		ut_t m_utList;
		bs_t m_bsList;
		unsigned long int m_ssCounter;
};

#endif // _INC_SCENARIO_H