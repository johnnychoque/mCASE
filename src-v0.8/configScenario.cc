#include "configScenario.h"

using namespace std;

#define DBG_LEVEL 3
#include "debug.h"

configScenario::configScenario(void)
{
	DBG_IN("configScenario::configScenario");

	m_toroidal = false;

	DBG_OUT("configScenario::configScenario");
}

configScenario::~configScenario() 
{
	RATtype_iter_t RATiter;
	terminalType_iter_t termIter;
	userType_iter_t userIter;
	serviceType_iter_t servIter;
	BStype_iter_t BSiter;
	movType_iter_t movIter;
	operatorType_iter_t OPiter;
	businessType_iter_t BMiter;
	
	DBG_IN("configScenario::~configScenario");
	
	for (RATiter = RATtypeList.begin(); RATiter != RATtypeList.end(); RATiter++) {
		delete((*RATiter).second);
	}
	RATtypeList.clear();
	
	for (termIter = termTypeList.begin(); termIter != termTypeList.end(); termIter++) {
		delete((*termIter).second);
	}
	termTypeList.clear();
	
	for (userIter = userTypeList.begin(); userIter != userTypeList.end(); userIter++) {
		delete((*userIter).second);
	}
	userTypeList.clear();
	
	for (servIter = servTypeList.begin(); servIter != servTypeList.end(); servIter++) {
		delete((*servIter).second);
	}
	servTypeList.clear();
	
	for (movIter = movTypeList.begin(); movIter != movTypeList.end(); movIter++) {
		delete((*movIter).second);
	}
	movTypeList.clear();
	
	for (BSiter = BStypeList.begin(); BSiter != BStypeList.end(); BSiter++) {
		delete((*BSiter).second);
	}
	BStypeList.clear();
	
	for (OPiter = OPtypeList.begin(); OPiter != OPtypeList.end(); OPiter++) {
		delete((*OPiter).second);
	}
	OPtypeList.clear();
	
	for(BMiter = BMtypeList.begin(); BMiter != BMtypeList.end(); BMiter++) {
		delete((*BMiter).second);
	}
	BMtypeList.clear();
	
	DBG_OUT("configScenario::~configScenario");
}

int configScenario::getBSminDistance(int BStypeID_)
{
	return BStypeList[BStypeID_]->minDistance();
}

double configScenario::getBSrange(int BStypeID_)
{
	return RATtypeList[BStypeList[BStypeID_]->RATid()]->range();
}

double configScenario::getBScapacity(int BStypeID_)
{
	return RATtypeList[BStypeList[BStypeID_]->RATid()]->capacity();
}

float configScenario::getBSprice(int BStypeID_, int OPtypeID_)
{
	vector<float> priceOfBSlist;
	//int opTypeID_;
	
	//opTypeID_ = BStypeList[BStypeID_]->opTypeID();
	priceOfBSlist = OPtypeList[OPtypeID_]->getpriceOfBS();
	return priceOfBSlist[BStypeID_];
}

float configScenario::getUserPrice(int userTypeID_)
{
	return userTypeList[userTypeID_]->userPrice();
}

float configScenario::getbsPriceFactor(int OPtypeID_)
{
	return OPtypeList[OPtypeID_]->bsPriceFactor();
}

float configScenario::getopPriceFactor(int OPtypeID_)
{
	return OPtypeList[OPtypeID_]->opPriceFactor();
}

void configScenario::addRATtype(int index_, double range_, int capacity_)
{
	RATtype *newRAT;

	DBG_IN("configScenario::addRATtype");

	newRAT = new RATtype;
	newRAT->index() = index_;
	newRAT->range() = range_;
	newRAT->capacity() = capacity_;

	//DBG_INFO("Adding new RAT (%d) to the scenario, range = %.3f, capacity = %d",index_, range_, capacity_);
	RATtypeList[index_] = newRAT;

	DBG_OUT("configScenario::addRATtype");
}

void configScenario::addTermType(int index_, float prob_, RATids_t RATids_)
{
	terminalType *newTerm;
	vector<int>::iterator iter;	
	
	DBG_IN("configScenario::addUTtype");

	newTerm = new terminalType;
	newTerm->index() = index_;
	newTerm->termProb() = prob_;
	newTerm->setRATids(RATids_);
	
	//DBG_INFO("Adding new termninal(%d) to the scenario, prob = %.3f",index_, prob_);
		
	// for (iter=RATids_.begin(); iter != RATids_.end(); iter++) {
		// DBG_INFO("RATid=%d",*iter);
	// }
	
	termTypeList[index_] = newTerm;

	DBG_OUT("configScenario::addUTtype");
}

void configScenario::addUserType(int index_, float prob_, userServs_t userServs_, float price_)
{
	userType *newUser;
	vector<int>::iterator it;
	
	DBG_IN("configScenario::addUserType");

	newUser = new userType;
	newUser->index() = index_;
	newUser->userProb() = prob_;
	newUser->setuserServs(userServs_);
	newUser->userPrice() = price_;
	
	// DBG_INFO("Service IDs for UT %d",index_);
	// for (it=userServs_.begin(); it!=userServs_.end(); it++) {
		// DBG_INFO("Service ID %d", *it);
	// }
	userTypeList[index_] = newUser;
	
	DBG_OUT("configScenario::addUserType");
}

void configScenario::addServiceType(int index_, float tia_, float ts_,float capacity_,bool realtime_)
{
	serviceType *newServ;

	DBG_IN("configScenario::addServiceType");
	
	newServ = new serviceType;
	newServ->index() = index_;
	newServ->tia() = tia_;
	newServ->ts() = ts_;
	newServ->capacity() = capacity_;
	newServ->realtime() = realtime_;
	
	servTypeList[index_] = newServ;
	
	DBG_OUT("configScenario::addServiceType");
}

int configScenario::getnumOfServices(void)
{
	DBG_IN("configScenario::getnumOfServices");
	
	return servTypeList.size();
	
	DBG_OUT("configScenario::getnumOfServices");
}

int configScenario::getnumOfTypeUsers(void)
{
	return userTypeList.size();
}

void configScenario::addMovementType(int index_, int model_, float movProb_, struct movParams params_)
{
	movementType *newMov;
	
	newMov = new movementType;
	newMov->index() = index_;
	newMov->model() = model_;
	newMov->movProb() = movProb_;
	newMov->params() = params_;

	movTypeList[index_] = newMov;
}

void configScenario::addBStype(int index_, int RATid_, double minDist_)
{
	BStype *newBS;
	vector<int>::iterator iter;
	
	DBG_IN("configScenario::addBStype");
	
	newBS = new BStype;
	newBS->index() = index_;
	newBS->RATid() = RATid_;
	newBS->minDistance() = minDist_;
	
	// DBG_INFO("Adding new BS(%d)",index_);
	// for (iter=RATids_.begin(); iter!=RATids_.end(); iter++) {
		// DBG_INFO("RATid=%d",*iter);
	// }
	
	BStypeList[index_] = newBS;
	
	DBG_OUT("configScenario::addBStype");
}

void configScenario::addOPtype(int index_, numOfBSlist_t numOfBSlist_, priceOfBSlist_t priceOfBSlist_,
								float marketProb_, float bsPriceFactor_, float opPriceFactor_)
{
	operatorType *newOP;
	numOfBSlist_t tmplist;
	vector<int>::iterator iter;
	// int i=0;
	
	DBG_IN("configScenario::addOPtype");

	newOP = new operatorType;
	newOP->index() = index_;
	newOP->setnumOfBS(numOfBSlist_);
	newOP->setpriceOfBS(priceOfBSlist_);
	newOP->marketProb() = marketProb_;
	newOP->bsPriceFactor() = bsPriceFactor_;
	newOP->opPriceFactor() = opPriceFactor_;

	tmplist = newOP->getnumOfBS();
	// DBG_INFO("Adding new BSs to operator(%d)",index_);
	// for (iter=tmplist.begin(); iter!=tmplist.end(); iter++) {
		// DBG_INFO("BS%d=%d",i,*iter);
		// i++;
	// }
	
	OPtypeList[index_] = newOP;
	
	DBG_OUT("configScenario::addOPtype");
}

void configScenario::addBusinessType(int index_, int type_, OPids_t OPids_)
{
	businessType *newBM;
	
	DBG_IN("configScenario::addBussinessType");

	newBM = new businessType;
	newBM->index() = index_;
	newBM->typeOfBM() = type_;
	newBM->setOPids(OPids_);
	
	BMtypeList[index_] = newBM;
	
	DBG_OUT("configScenario::addBussinessType");
}

bool configScenario::compareRATids(int termTypeID_, int bsTypeID_)
{
	vector<int> utRATids_;
	vector<int>::iterator utIter;
	bool result=false;
	
	DBG_IN("configScenario::compareRATids");
	utRATids_ = termTypeList[termTypeID_]->getRATids();
	
	for (utIter=utRATids_.begin(); utIter != utRATids_.end(); utIter++) {
		//DBG_INFO("utRATid=%d bsRATid=%d",*utIter,BStypeList[bsTypeID_]->RATid());
		if (*utIter == BStypeList[bsTypeID_]->RATid()) {
			result = true;
			//DBG_INFO("RATs iguales!!!");
			break;
		}
	}
	//if (!result) DBG_INFO("RATs diferentes");
	
	DBG_OUT("configScenario::compareRATids");
	return result;
}

int configScenario::Configure(configFile configFile_)
{
	unsigned int i, j;
	string value;
	vector<string> values;
	unsigned int numRATtypes;
	ostringstream aux1, aux2;
	double rangeRAT;
	float capacityRAT;
	unsigned int numTermTypes;
	float termProb;
	vector<int> teRATids;
	listEntry_::iterator iter;
	unsigned int numUserTypes;
	float userProb;
	vector<int> userServs;
	float userPrice;
	unsigned int numServTypes;
	float tia, ts, capacity;
	bool realtime;
	unsigned int numMovTypes;
	unsigned int movModel;
	float movProb;
	struct movParams params;
	unsigned int numBStypes;
	int bsRATid;
	int minDist;
	unsigned int numOPtypes;
	unsigned int numBMtypes;
	vector<int> numOfBSlist;
	vector<float> priceOfBSlist;
	float marketProb;
	float bsPriceFactor=0.0;
	float opPriceFactor;
	int BMtype;
	OPids_t OPids;
	unsigned int tmpValue;
	
	DBG_IN("configScenario::Configure");
	
	/* (1) Set simulation parameters */
	if(configFile_.getKeyValue("SIMULATION","DIMX",value) < 0) {
		DBG_ERR("X dimension not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	xMax() = atof(value.c_str());

	if(configFile_.getKeyValue("SIMULATION","DIMY",value) < 0) {
		DBG_ERR("Y dimension not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	yMax() = atof(value.c_str());

	DBG_INFO("Dimension X=%.2f, Y=%.2f", xMax(), yMax());
	
	if(configFile_.getKeyValue("SIMULATION","TOROIDAL",value) < 0) {
		DBG_ERR("Toroidal value not specified... Please fix!!!");
		return ResultCode_ERROR;
	}	

	if (value=="yes") {
		DBG_INFO("Setting toroidal distance model...");
		toroidal() = true;
	} else {
		DBG_INFO("Setting euclidean distance model...");
		toroidal() = false;
	}
	
	if(configFile_.getKeyValue("SIMULATION","TIME",value) < 0) {
		DBG_ERR("Total simulation time not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	DBG_INFO("Total simulation time = %.2f",atof(value.c_str()));
	simulTime() = atof(value.c_str());

  /* (3) Create RAT types list -- RATlist */
	if(configFile_.getKeyValue("RATS","TYPES",value) < 0) {
		DBG_ERR("Types of RATs unknown, please fix it!!!");
		return ResultCode_ERROR;	
	}
	DBG_INFO("Types of RATs = %d",atoi(value.c_str()));
	numRATtypes = (unsigned int)atoi(value.c_str());

	for(i=0; i<numRATtypes ; i++) {
		aux1 << "RAT" << i;
		if(configFile_.getKeyValue((char *) aux1.str().c_str(),"RANGE",value) < 0) {
			DBG_ERR("Range of Iface %d is unknown... Please fix!!!",i);
			return ResultCode_ERROR;
		}
		rangeRAT = atof(value.c_str());
		if(configFile_.getKeyValue((char *) aux1.str().c_str(), "CAPACITY", value) < 0) {
			DBG_ERR("Capacity of RAT %d is unknown... Please fix!!!",i);
			return ResultCode_ERROR;
		} 
		capacityRAT = atof(value.c_str());
		addRATtype(i,rangeRAT,capacityRAT);
		aux1.str("");
		aux1.clear();
	}
	
	/* (4) Create terminal types list */
	if(configFile_.getKeyValue("TERMINALS","TYPES",value) < 0) {
		DBG_ERR("Types of terminals unknown, please fix!!!");
		return ResultCode_ERROR;
	}
	DBG_INFO("Types of terminals = %d",atoi(value.c_str()));
	numTermTypes = (unsigned int)atoi(value.c_str());

	for(i=0; i<numTermTypes ; i++) {
		aux1 << "TERMINAL" << i;
		if(configFile_.getKeyValue((char *) aux1.str().c_str(),"PROBABILITY",value) < 0) {
			DBG_ERR("Probability of TERMINAL%d is unknown... Please fix!!!",i);
			return ResultCode_ERROR;
		}
		termProb = atof(value.c_str());
		if(configFile_.getListValues((char *) aux1.str().c_str(),"RATID",values) < 0) {
			DBG_ERR("RAT IDs of TERMINAL%d is unknown... Please fix!!!",i);
			return ResultCode_ERROR;
		}
		for (iter = values.begin(); iter != values.end(); iter++) {
			teRATids.push_back(atoi((*iter).c_str()));
		}
		addTermType(i,termProb,teRATids);
		teRATids.clear();
		aux1.str("");
		aux1.clear();
	}		

	/* (5) Set number of users */
	if(configFile_.getKeyValue("USERS","NUMBER",value) < 0) {
		DBG_ERR("Number of users not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	DBG_INFO("Number of users = %d",atoi(value.c_str()));
	numOfUsers() = atoi(value.c_str());

	/* (6) Create service types list */
	if(configFile_.getKeyValue("SERVICES","TYPES",value) < 0) {
		DBG_ERR("Types of services unknown, please fix!!!");
		return ResultCode_ERROR;
	}
	DBG_INFO("Types of services = %d",atoi(value.c_str()));
	numServTypes = (unsigned int)atoi(value.c_str());

	for(i=0; i<numServTypes ; i++) {
		aux1 << "SERVICE" << i;
		if(configFile_.getKeyValue((char *) aux1.str().c_str(), "TIA", value) < 0) {
			DBG_ERR("InterArrival Time %d is unknown... Please fix!!!",i);
			return ResultCode_ERROR;
		}
		tia = atof(value.c_str());
		if(configFile_.getKeyValue((char *) aux1.str().c_str(), "TS", value) < 0) {
			DBG_ERR("Service Time %d is unknown... Please fix!!!",i);
			return ResultCode_ERROR;
		}
		ts = atof(value.c_str());		
		if(configFile_.getKeyValue((char *) aux1.str().c_str(), "CAPACITY", value) < 0) {
			DBG_ERR("Capacity of service%d is unknown... Please fix!!!",i);
			return ResultCode_ERROR;
		}
		capacity = atof(value.c_str());
		if(configFile_.getKeyValue((char *) aux1.str().c_str(), "REALTIME", value) < 0) {
			DBG_ERR("Realtime of service%d is unknown... Please fix!!!",i);
			return ResultCode_ERROR;
		}
		value == "yes" ? realtime = true: realtime = false;

		addServiceType(i,tia,ts,capacity,realtime);
		aux1.str("");
		aux1.clear();
	}
	
	/* (7) Create user types list */
	if(configFile_.getKeyValue("USERS","TYPES",value) < 0) {
		DBG_ERR("Types of users unknown, please fix!!!");
		return ResultCode_ERROR;
	}
	DBG_INFO("Types of users = %d",atoi(value.c_str()));
	numUserTypes = (unsigned int)atoi(value.c_str());

	for(i=0; i<numUserTypes ; i++) {
		aux1 << "USER" << i;
		if(configFile_.getKeyValue((char *) aux1.str().c_str(),"PROBABILITY",value) < 0) {
			DBG_ERR("Probability of user%d is unknown... Please fix!!!",i);
			return ResultCode_ERROR;
		}
		userProb = atof(value.c_str());
		if(configFile_.getListValues((char *) aux1.str().c_str(),"SERVICES",values) < 0) {
			DBG_ERR("Services of user%d is unknown... Please fix!!!",i);
			return ResultCode_ERROR;
		}
		for (iter = values.begin(); iter != values.end(); iter++) {
			tmpValue = (unsigned int) atoi((*iter).c_str());
			if (tmpValue < numServTypes) {
				userServs.push_back(tmpValue);
			}
			else {
				DBG_ERR("Service %d of user %d is invalid... Please fix!!!", tmpValue, i);
				exit(-1);
			}
		}
		if(configFile_.getKeyValue((char *) aux1.str().c_str(),"PRICE",value) < 0) {
			DBG_ERR("Price of user%d is unknown... Please fix!!!",i);
			return ResultCode_ERROR;
		}
		userPrice = atof(value.c_str());
		//DBG_INFO("addUserType %d", i);
		addUserType(i,userProb,userServs,userPrice);
		
		userServs.clear();
		aux1.str("");
		aux1.clear();
	}
		

	
	/* (8) Create movement types list */
	if(configFile_.getKeyValue("MOVEMENTS","TYPES",value) < 0) {
		DBG_ERR("Types of movements unknown, please fix!!!");
		return ResultCode_ERROR;
	}
	DBG_INFO("Types of movements = %d",atoi(value.c_str()));
	numMovTypes = (unsigned int)atoi(value.c_str());

	for(i=0; i<numMovTypes ; i++) {
		aux1 << "MOVEMENT" << i;
		if(configFile_.getKeyValue((char *) aux1.str().c_str(), "MODEL", value) < 0) {
			DBG_ERR("Model of movement%d is unknown... Please fix!!!",i);
			return ResultCode_ERROR;
		}
		if (value == "MOVNONE") movModel = MOVNONE;
		else if (value == "RWP_PLAIN") movModel = RWP_PLAIN;
		else if (value == "RWP_REFLECT" ) movModel = RWP_REFLECT;
		else if (value == "RWP_WRAP") movModel = RWP_WRAP;
		else
			DBG_ERR("value is unknown");
		
		if(configFile_.getKeyValue((char *) aux1.str().c_str(), "PROBABILITY", value) < 0) {
			DBG_ERR("Probability of movement%d is unknown... Please fix!!!",i);
			return ResultCode_ERROR;
		}
		movProb = atof(value.c_str());

		memset(&params,0,sizeof(params));
		if ((movModel==RWP_PLAIN) || (movModel==RWP_REFLECT) || (movModel==RWP_WRAP)) {
			if(configFile_.getKeyValue((char *) aux1.str().c_str(), "SPEEDMIN", value) < 0) {
				DBG_ERR("Speed min of movement%d is unknown... Please fix!!!",i);
				return ResultCode_ERROR;
			}
			params.minSpeed = atof(value.c_str());
			if(configFile_.getKeyValue((char *) aux1.str().c_str(), "SPEEDMAX", value) < 0) {
				DBG_ERR("Speed max of movement%d is unknown... Please fix!!!",i);
				return ResultCode_ERROR;
			}
			params.maxSpeed = atof(value.c_str());
			if(configFile_.getKeyValue((char *) aux1.str().c_str(), "PAUSEMIN", value) < 0) {
				DBG_ERR("Pause min of movement%d is unknown... Please fix!!!",i);
				return ResultCode_ERROR;
			}
			params.minPauseTime = atof(value.c_str());
			if(configFile_.getKeyValue((char *) aux1.str().c_str(), "PAUSEMAX", value) < 0) {
				DBG_ERR("Pause max of movement%d is unknown... Please fix!!!",i);
				return ResultCode_ERROR;
			}
			params.maxPauseTime = atof(value.c_str());
		}

		if ((movModel==RWP_REFLECT) || (movModel==RWP_WRAP)) {
			if(configFile_.getKeyValue((char *) aux1.str().c_str(), "ALPHAMIN", value) < 0) {
					DBG_ERR("Alpha min of movement%d is unknown... Please fix!!!",i);
					return ResultCode_ERROR;
			}
			params.minDirection = atof(value.c_str());
			if(configFile_.getKeyValue((char *) aux1.str().c_str(), "ALPHAMAX", value) < 0) {
				DBG_ERR("Alpha max of movement%d is unknown... Please fix!!!",i);
				return ResultCode_ERROR;
			}
			params.maxDirection = atof(value.c_str());
			if(configFile_.getKeyValue((char *) aux1.str().c_str(), "TIMEMIN", value) < 0) {
				DBG_ERR("Time min of movement%d is unknown... Please fix!!!",i);
				return ResultCode_ERROR;
			}
			params.minTravelTime = atof(value.c_str());
			if(configFile_.getKeyValue((char *) aux1.str().c_str(), "TIMEMAX", value) < 0) {
				DBG_ERR("Time max of movement%d is unknown... Please fix!!!",i);
				return ResultCode_ERROR;
			}
			params.maxTravelTime = atof(value.c_str());
		}

		addMovementType(i, movModel, movProb, params);
		aux1.str("");
		aux1.clear();
	}
	
	/* (8) Create base stations types list */
	if(configFile_.getKeyValue("BASESTATIONS","TYPES",value) < 0) {
		DBG_ERR("Types of base stations unknown, please fix!!!");
		return ResultCode_ERROR;
	}
	DBG_INFO("Types of base stations = %d",atoi(value.c_str()));
	numBStypes = (unsigned int)atoi(value.c_str());

	for(i=0; i<numBStypes ; i++) {
		aux1 << "BS" << i;
		if(configFile_.getKeyValue((char *) aux1.str().c_str(),"RATID",value) < 0) {
			DBG_ERR("RAT id of BS%d is unknown... Please fix!!! %s",i, value.c_str());
			return ResultCode_ERROR;
		}
		bsRATid = atoi(value.c_str());
		if(configFile_.getKeyValue((char *) aux1.str().c_str(),"MINDISTANCE",value) < 0) {
			DBG_ERR("Min distance of BS %d is unknown... Please fix!!!",i);
			return ResultCode_ERROR;
		}
		minDist = atoi(value.c_str());
		
		addBStype(i,bsRATid,minDist);
		aux1.str("");
		aux1.clear();
	}	

	/* (9) Create operator types list */
	if(configFile_.getKeyValue("OPERATORS","TYPES",value) < 0) {
		DBG_ERR("Types of operators unknown, please fix!!!");
		return ResultCode_ERROR;
	}
	DBG_INFO("Types of operators = %d",atoi(value.c_str()));
	numOPtypes = (unsigned int)atoi(value.c_str());

	for(i=0; i<numOPtypes ; i++) {
		aux1 << "OPERATOR" << i;
		for(j=0; j<numBStypes; j++) {
			aux2 << "BS" << j;
			if(configFile_.getKeyValue((char *) aux1.str().c_str(),(char *) aux2.str().c_str(),value) < 0) {
				DBG_ERR("BS%d value of OPERATOR%d is unknown... Please fix!!!",j,i);
				return ResultCode_ERROR;
			}
			numOfBSlist.push_back(atoi(value.c_str()));
			aux2.str(""); aux2.clear();

			aux2 << "PRICEBS" << j;
			if(configFile_.getKeyValue((char *) aux1.str().c_str(),(char *) aux2.str().c_str(),value) < 0) {
				DBG_ERR("PRICEBS%d value of OPERATOR%d is unknown... Please fix!!!",j,i);
				return ResultCode_ERROR;
			}
			priceOfBSlist.push_back(atof(value.c_str()));
			aux2.str(""); aux2.clear();
		}		

		if(configFile_.getKeyValue((char *) aux1.str().c_str(),"BSPRICEFACTOR",value) < 0) {
			DBG_ERR("BS price factor not specified... Please fix!!!");
			return ResultCode_ERROR;
		}
		bsPriceFactor = atof(value.c_str());
		if(configFile_.getKeyValue((char *) aux1.str().c_str(),"OPPRICEFACTOR",value) < 0) {
			DBG_ERR("OP price factor not specified... Please fix!!!");
			return ResultCode_ERROR;
		}
		opPriceFactor = atof(value.c_str());

		aux1.str(""); aux1.clear();
		aux1 << "OP" << i;
		if(configFile_.getKeyValue("MARKETSHARE",(char *) aux1.str().c_str(),value) < 0) {
			DBG_ERR("OP%d value of MARKETSHARE is unknown... Please fix!!!",i);
			return ResultCode_ERROR;
		}
		marketProb = atof(value.c_str());

		addOPtype(i,numOfBSlist,priceOfBSlist,marketProb,bsPriceFactor,opPriceFactor);
		
		numOfBSlist.clear();
		priceOfBSlist.clear();
		aux1.str(""); aux1.clear();
		aux2.str(""); aux2.clear();
	}

	/* (10) Create business model types list */
	if(configFile_.getKeyValue("BUSINESSMODEL","TYPES",value) < 0) {
		DBG_ERR("Types of business models unknown, please fix!!!");
		return ResultCode_ERROR;
	}
	DBG_INFO("Types of business models = %d",atoi(value.c_str()));
	numBMtypes = (unsigned int)atoi(value.c_str());

	for(i=0; i<numBMtypes ; i++) {
		aux1 << "BUSINESS" << i;
		if(configFile_.getKeyValue((char *) aux1.str().c_str(),"MODEL",value) < 0) {
			DBG_ERR("Values of BM%d is unknown... Please fix!!!",i);
			return ResultCode_ERROR;
		}
		if (value == "COOPERATIVE")
			BMtype = COOPERATIVE;
		else if (value == "COMPETITIVE")
			BMtype = COMPETITIVE;
		else {
			DBG_ERR("Model of BM type is unknown... Please fix!!!");
			return ResultCode_ERROR;
		}
		if(configFile_.getListValues((char *) aux1.str().c_str(),"OPID",values) < 0) {
			DBG_ERR("OP IDs of BM%d is unknown... Please fix!!!",i);
			return ResultCode_ERROR;
		}
		for (iter = values.begin(); iter != values.end(); iter++) {
			OPids.push_back(atoi((*iter).c_str()));
		}
		addBusinessType(i, BMtype, OPids);
		OPids.clear();
		aux1.str("");
		aux1.clear();
	}

	/* (11) Get snapshot parameters */
	if(configFile_.getKeyValue("SNAPSHOTS","SETUP",value) < 0) {
		DBG_ERR("Snapshot setup not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	DBG_INFO("Snapshot setup is %s",value.c_str());
	if (value == "ON") {
		snapshotSetup() = true;
	}
	else if (value == "OFF") {
		snapshotSetup() = false;
	}
	else {
		DBG_ERR("Snapshot setup is not ON or OFF... Please fix!!!");
		return ResultCode_ERROR;
	}
	
	if(configFile_.getKeyValue("SNAPSHOTS","STEP",value) < 0) {
		DBG_ERR("Snapshot step time not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	stepTime() = atof(value.c_str());
	if(configFile_.getKeyValue("SNAPSHOTS","FILE",value) < 0) {
		DBG_ERR("Snapshot file prefix not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	ssfilePrefix() = value.c_str();
	if (snapshotSetup()) {
		DBG_INFO("Snapshot step time = %.2f",stepTime());
		DBG_INFO("Snapshot file prefix = %s",ssfilePrefix().c_str());
	}

	/* (12) Get trace files setup */
	if(configFile_.getKeyValue("TRACEFILES","SETUP",value) < 0) {
		DBG_ERR("Tracefile setup not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	DBG_INFO("Tracefile setup is %s",value.c_str());	
	if (value == "NO_TRACES") {
		traceFileSetup() = NO_TRACES;
	}
	else if (value == "MAKE_TRACES") {
		traceFileSetup() = MAKE_TRACES;
	}
	else if (value == "LOAD_TRACES") {
		traceFileSetup() = LOAD_TRACES;
	}	
	else {
		DBG_ERR("Tracefile setup is not unknown... Please fix!!!");
		return ResultCode_ERROR;
	}

	/* (13) Get Price parameters */
	if(configFile_.getKeyValue("PRICEPARAMS","LOWERTHRLOAD",value) < 0) {
		DBG_ERR("Lower threshold free load not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	lowerThrLoad() = atof(value.c_str());
	if(configFile_.getKeyValue("PRICEPARAMS","UPPERTHRLOAD",value) < 0) {
		DBG_ERR("Upper threshold free load not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	upperThrLoad() = atof(value.c_str());

	DBG_INFO("Params Price function lowerThrLoad %.2f upperThrLoad %.2f",
		lowerThrLoad(), upperThrLoad());
	
	/* (13a) Get Handover parameters */
	if(configFile_.getKeyValue("HOPARAMS","DISCOUNT",value) < 0) {
		DBG_ERR("Handover discount parameter not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	hoDiscount() = atof(value.c_str());

	DBG_INFO("Param Handover function Discount %.2f", hoDiscount());
		
	/* (14) Get constraints */
	if(configFile_.getKeyValue("CONSTRAINTS","QUALITY",value) < 0) {
		DBG_ERR("Quality constraint value not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	qualityWeight() = atof(value.c_str());
	if(configFile_.getKeyValue("CONSTRAINTS","HANDOVER",value) < 0) {
		DBG_ERR("Handover constraint value not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	handoverWeight() = atof(value.c_str());
	if(configFile_.getKeyValue("CONSTRAINTS","OPERATOR",value) < 0) {
		DBG_ERR("Operator constraint value not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	operatorWeight() = atof(value.c_str());
	if(configFile_.getKeyValue("CONSTRAINTS","LOAD",value) < 0) {
		DBG_ERR("Load constraint value not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	loadWeight() = atof(value.c_str());
	if(configFile_.getKeyValue("CONSTRAINTS","PRICE",value) < 0) {
		DBG_ERR("Price constraint value not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	priceWeight() = atof(value.c_str());
	DBG_INFO("Constraints lq %.2f ho %.2f op %.2f ld %.2f pr %.2f", qualityWeight(), handoverWeight(), operatorWeight(), loadWeight(), priceWeight());

	/* (15) Get Periodic Monitoring params */
	if(configFile_.getKeyValue("MONITORING","PMSTEP",value) < 0) {
		DBG_ERR("Periodic Monitoring step value not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	pmStep() = atof(value.c_str());

	if(configFile_.getKeyValue("MONITORING","PMSETUP",value) < 0) {
		DBG_ERR("Periodic Monitoring setup not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	DBG_INFO("Periodic Monitoring setup is %s",value.c_str());	
	if (value == "ON") {
		pmSetup() = MONITORING_ON;
	}
	else if (value == "OFF") {
		pmSetup() = MONITORING_OFF;
	}
	else {
		DBG_ERR("Periodic Monitoring setup is not unknown... Please fix!!!");
		return ResultCode_ERROR;
	}

	/* (16) Get Link Quality params */
	if(configFile_.getKeyValue("MONITORING","LQSTEP",value) < 0) {
		DBG_ERR("Link Quality step value not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	lqStep() = atof(value.c_str());

	if(configFile_.getKeyValue("MONITORING","LQSETUP",value) < 0) {
		DBG_ERR("Link Quality setup not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	DBG_INFO("Link Quality setup is %s",value.c_str());	
	if (value == "ON") {
		lqSetup() = LINKQUALITY_ON;
	}
	else if (value == "OFF") {
		lqSetup() = LINKQUALITY_OFF;
	}
	else {
		DBG_ERR("Link Quality setup is not unknown... Please fix!!!");
		return ResultCode_ERROR;
	}

	/* (17) Get Load monitoring param */
	if(configFile_.getKeyValue("MONITORING","LDSTEP",value) < 0) {
		DBG_ERR("Load monitoring step value not specified... Please fix!!!");
		return ResultCode_ERROR;
	}
	ldStep() = atof(value.c_str());
	
	DBG_OUT("configScenario::Configure");

	return ResultCode_OK;
}
