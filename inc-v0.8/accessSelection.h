#ifndef _INC_ACCESSSELECTION_H
#define _INC_ACCESSSELECTION_H

#define EPSILON 0.0001   // Define tu propia tolerancia
#define FLOAT_EQ(x,v) (((v - EPSILON) < x) && (x < ( v + EPSILON)))

#include "basestation.h"
#include "RATtype.h"
#include "configScenario.h"
#include <algorithm>

using namespace std;

class userTerminal;

typedef struct {
  basestation *bs;
  float offeredPrice;
  float weight;
} dataDS_t;

typedef vector<dataDS_t> ds_t;
typedef ds_t::iterator dsIter_t;

class accessSelection {
	public:
		accessSelection();
		~accessSelection();
		void getBSs(bs_t &bsList_);
		void getcfgSce(configScenario *&cfgSce_);
		void getDetectedSet(userTerminal *ut_);
		void getValidatedSet(userTerminal *ut, servData_t *sData);
		basestation* getCandidateSet(userTerminal *ut, float resources, servData_t *sData);
		void releaseResourcesOfBS(userTerminal *ut, float resources, servData_t *sData, bool fromHO_ = false);
		void assignResourcesToBS(userTerminal *ut, basestation *bs, float resources, servData_t *sData, unsigned char code);
		bool IsSnapshotSetup(void);
	private:
		bs_t m_bsList; //apunta a los mismos valores almacenados en m_bsList de scenario
		//! Subconjunto de m_bsList constituido por las BSs detectadas por el UT.
		ds_t m_dsList;
		configScenario *m_cfgSce;
		bool inCoverage(userTerminal *ut, basestation *bs);
		float getBSAvailableLoad(basestation *bs);
		float getBSAvailableRelativeLoad(basestation *bs);
		void assignLoadBS(basestation *bs, float resources, servData_t *sData);
		double LinkQualityConstraint(userTerminal *ut, basestation *bs);
		float HandoverConstraint(userTerminal *ut, basestation *bs, int servType);
		float OperadorConstraint(userTerminal *ut, basestation *bs);
		float LoadConstraint(basestation *bs);
		//float PriceConstraint(userTerminal *ut, basestation *bs);
		float PriceConstraint(userTerminal *ut, basestation *bs, float offerPrice);
		float setOfferPrice(userTerminal *ut, basestation *currBS, basestation *bs_i, servData_t *sData);
		float truncate(float f, unsigned int n);

};

#endif //  _INC_ACCESSSELECTION_H