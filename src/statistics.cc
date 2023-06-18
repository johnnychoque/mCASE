#include <iostream>
#include "statistics.h"

#define DBG_LEVEL 1
#include "debug.h"

statistics *statistics::m_instance = 0;

statistics* statistics::instance()
{
	if(m_instance == 0) {
		m_instance = new statistics;
	}
	return m_instance;
}

void statistics::endInstance(void)
{
	statsPerServ_iter_t iter;
	
	if (m_instance != 0) {
		for (iter=m_statsPerServ.begin(); iter!=m_statsPerServ.end(); iter++) {
			delete ((*iter).second);
		}
		m_statsPerServ.clear();
		delete m_instance;
	}
}

void statistics::initStatsPerServ(int callID)
{
	statsPerServ *stats;
	pair<map<int, statsPerServ*>::iterator,bool> ret;
	
	stats = new statsPerServ;
	ret = m_statsPerServ.insert(pair<int, statsPerServ*>(callID,stats));
	if (ret.second == false) {
		DBG_ERR("Ya existe un callID %d en m_statsPerServ", callID);
		exit(-1);
	}
	DBG_INFO2("callID %d insertado en m_statsPerServ", callID);
}

void statistics::statsInsert_tStart(int callID, double tStart)
{
	statsPerServ_iter_t iter;
	
	iter = m_statsPerServ.find(callID);
	if (iter == m_statsPerServ.end()) {
		DBG_ERR("statsInsert_tStart: callID %d no se encuentra en m_statsPerServ", callID);
		exit(-1);
	}

	(*iter).second->m_tStart = tStart;
	
}

void statistics::statsInsert_tEnd(int callID, double tEnd)
{
	statsPerServ_iter_t iter;
	
	iter = m_statsPerServ.find(callID);
	if (iter == m_statsPerServ.end()) {
		DBG_ERR("statsInsert_tEnd: callID %d no se encuentra en m_statsPerServ", callID);
		exit(-1);
	}

	(*iter).second->m_tEnd = tEnd;
	
}

void statistics::statsInsert_tDrop(int callID, double tDrop)
{
	statsPerServ_iter_t iter;
	
	iter = m_statsPerServ.find(callID);
	if (iter == m_statsPerServ.end()) {
		DBG_ERR("statsInsert_tDrop: callID %d no se encuentra en m_statsPerServ", callID);
		exit(-1);
	}

	(*iter).second->m_tDrop = tDrop;
	
}

void statistics::statsInsert_tHOs(int callID, double tHO)
{
	statsPerServ_iter_t iter;
	
	iter = m_statsPerServ.find(callID);
	if (iter == m_statsPerServ.end()) {
		DBG_ERR("statsInsert_tHOs: callID %d no se encuentra en m_statsPerServ", callID);
		exit(-1);
	}

	(*iter).second->m_tHOs.push_back(tHO);
	
}

void statistics::statsInsert_price(int callID, float price)
{
	statsPerServ_iter_t iter;
	
	iter = m_statsPerServ.find(callID);
	if (iter == m_statsPerServ.end()) {
		DBG_ERR("statsInsert_price: callID %d no se encuentra en m_statsPerServ", callID);
		exit(-1);
	}

	DBG_INFO2("******** userType %d price %.2f", (*iter).second->m_userType, price);
	(*iter).second->m_price.push_back(price);
	
}

void statistics::statsInsert_currBS(int callID, int currBS)
{
	statsPerServ_iter_t iter;
	
	iter = m_statsPerServ.find(callID);
	if (iter == m_statsPerServ.end()) {
		DBG_ERR("statsInsert_currBS: callID %d no se encuentra en m_statsPerServ", callID);
		exit(-1);
	}

	(*iter).second->m_currBS.push_back(currBS);

}

void statistics::statsInsert_prefOP(int callID, bool value)
{
	statsPerServ_iter_t iter;
	
	iter = m_statsPerServ.find(callID);
	if (iter == m_statsPerServ.end()) {
		DBG_ERR("statsInsert_prefOP: callID %d no se encuentra en m_statsPerServ", callID);
		exit(-1);
	}

	(*iter).second->m_prefOP.push_back(value);

}

void statistics::statsInsert_lq(int callID, float lq)
{
	statsPerServ_iter_t iter;
	
	iter = m_statsPerServ.find(callID);
	if (iter == m_statsPerServ.end()) {
		DBG_ERR("statsInsert_lq: callID %d no se encuentra en m_statsPerServ", callID);
		exit(-1);
	}

	(*iter).second->m_linkQuality.push_back(lq);

}

void statistics::statsInsert_userType(int callID, int userType)
{
	statsPerServ_iter_t iter;
	
	iter = m_statsPerServ.find(callID);
	if (iter == m_statsPerServ.end()) {
		DBG_ERR("statsInsert_utID: callID %d no se encuentra en m_statsPerServ", callID);
		exit(-1);
	}

	(*iter).second->m_userType = userType;
	
}

void statistics::statsInsert_servType(int callID, int servType)
{
	statsPerServ_iter_t iter;
	
	iter = m_statsPerServ.find(callID);
	if (iter == m_statsPerServ.end()) {
		DBG_ERR("statsInsert_servType: callID %d no se encuentra en m_statsPerServ", callID);
		exit(-1);
	}

	(*iter).second->m_servType = servType;
	
}

void statistics::statsInsert_result(int callID, int result)
{
	statsPerServ_iter_t iter;
	
	iter = m_statsPerServ.find(callID);
	if (iter == m_statsPerServ.end()) {
		DBG_ERR("statsInsert_result: callID %d no se encuentra en m_statsPerServ", callID);
		exit(-1);
	}

	(*iter).second->m_result = result;

}

void statistics::initStatsBSload(int bsID)
{
	pair<map<int, vector<float> >::iterator,bool> ret;
	
	//insert(pair<int, vector<MyClass> >(10, vector<MyClass>()));
	
	// Inserta en el mapa el bsID con un vector vacio
	ret = m_statsBSload.insert(pair<int, vector<float> >(bsID,vector<float>()));
	if (ret.second == false) {
		DBG_ERR("Ya existe un bsID %d en m_statsBSload", bsID);
		exit(-1);
	}
	
}

void statistics::statsInsert_bsLoad(int bsID, float load)
{
	statsBSload_iter_t iter;
	
	iter = m_statsBSload.find(bsID);
	if (iter == m_statsBSload.end()) {
		DBG_ERR("statsInsert_bsLoad: bsID %d no se encuentra en m_statsBSload", bsID);
		exit(-1);	
	}
	(*iter).second.push_back(load);
}

/*
	Se devuelve a traves de summStats el total de servicios de cada tipo servType
	y de cada estado de la llamada. El key del mapa es servType y el vector<in>
	contiene el total segun el estado de cada llamada en el orden siguiente:
	
	key: servTypeID
	vector[0] = initiatedServices
	vector[1] = successfulServices
	vector[2] = rejectedServices
	vector[3] = droppedServices
*/
void statistics::getServiceSummaryStats(map<int, vector<int> >& summStats)
{
	vector<int> state_init(4,0);
	map<int, vector<int> >::iterator iterSumm;
	statsPerServ_iter_t iterServ;
	statsPerServ *statServ;
	int servType;
	
	for (iterServ = m_statsPerServ.begin(); iterServ != m_statsPerServ.end(); iterServ++) {
		statServ = iterServ->second;
		servType = statServ->m_servType;
		iterSumm = summStats.find(servType);
		if (iterSumm == summStats.end()) { // add servType al mapa
			summStats.insert(pair<int, vector<int> >(servType, state_init));
		}
		iterSumm = summStats.find(servType);
		switch (statServ->m_result) {
			case SUCCESS_CALL:
				(*iterSumm).second[0] = (*iterSumm).second[0]++;
				(*iterSumm).second[1] = (*iterSumm).second[1]++;
				break;
			case REJECT_CALL:
				(*iterSumm).second[0] = (*iterSumm).second[0]++;
				(*iterSumm).second[2] = (*iterSumm).second[2]++;
				break;
			case DROP_CALL:
				(*iterSumm).second[0] = (*iterSumm).second[0]++;
				(*iterSumm).second[3] = (*iterSumm).second[3]++;
				break;
			default:
				DBG_ERR("Unrecognized m_result stats %d", statServ->m_result);
				break;				
		}		
	}
	
}

void statistics::getUserSummaryStats(map<int, vector<int> >& summStats)
{
	vector<int> state_init(4,0);
	map<int, vector<int> >::iterator iterSumm;
	statsPerServ_iter_t iterServ;
	statsPerServ *statServ;
	int userType;
	
	for (iterServ = m_statsPerServ.begin(); iterServ != m_statsPerServ.end(); iterServ++) {
		statServ = iterServ->second;
		userType = statServ->m_userType;
		iterSumm = summStats.find(userType);
		if (iterSumm == summStats.end()) { // add userType al mapa
			summStats.insert(pair<int, vector<int> >(userType, state_init));
		}
		iterSumm = summStats.find(userType);
		switch (statServ->m_result) {
			case SUCCESS_CALL:
				(*iterSumm).second[0] = (*iterSumm).second[0]++;
				(*iterSumm).second[1] = (*iterSumm).second[1]++;
				break;
			case REJECT_CALL:
				(*iterSumm).second[0] = (*iterSumm).second[0]++;
				(*iterSumm).second[2] = (*iterSumm).second[2]++;
				break;
			case DROP_CALL:
				(*iterSumm).second[0] = (*iterSumm).second[0]++;
				(*iterSumm).second[3] = (*iterSumm).second[3]++;
				break;
			default:
				DBG_ERR("Unrecognized m_result stats %d", statServ->m_result);
				break;				
		}		
	}

}

unsigned int statistics::getTotalServStats(void)
{
	return m_statsPerServ.size();
}

//unsigned int statistics::getLQvectorStats(int index, vector<float> &lqVector, int& servType, float& duration, int& numHOs, float& rateOP)
/** version original
unsigned int statistics::getLQvectorStats(int index, servParamValues& sValues)
{
	statsPerServ_iter_t iter;
	statsPerServ *statServ;
	unsigned int i;
	float tStart, tEnd, totTime;
	
	iter = m_statsPerServ.begin();
	advance(iter, index);
	statServ = iter->second;
	if (statServ->m_result == SUCCESS_CALL) {
		sValues.lqVector = statServ->m_linkQuality;
		sValues.servType = statServ->m_servType;
		sValues.duration = statServ->m_tEnd - statServ->m_tStart;
		sValues.numHOs = statServ->m_tHOs.size();
		// calcular porcentaje de tiempo con el operador preferido
		totTime = 0;
		if (statServ->m_tHOs.size() > 0) {
			for (i=0; i< statServ->m_prefOP.size(); i++) {
				if (statServ->m_prefOP[i]) {
					if (i==0) {
						tStart = statServ->m_tStart;
						tEnd = statServ->m_tHOs[i];
					}
					else if (i==(statServ->m_prefOP.size()-1)) {
						tStart = statServ->m_tHOs[i-1];
						tEnd = statServ->m_tEnd;
					}
					else {
						tStart = statServ->m_tHOs[i-1];
						tEnd = statServ->m_tHOs[i];
					}
					totTime = totTime + (tEnd - tStart);
					if (tStart > tEnd) {
						DBG_ERR("Error calculando pref OP time, tStart %.2f tEnd %.2f", tStart, tEnd);
						exit(-1);
					}
				}
			}
		}
		else { // No se ha realizado ningun HO
			if (statServ->m_prefOP.size()!=1) {
				DBG_ERR("El tamaño del n_prefOP deberia ser 1");
				exit(-1);
			}
			if (statServ->m_prefOP[0]) {
				totTime = statServ->m_tEnd-statServ->m_tStart;
			}
			else {
				totTime = 0;
			}
		}

		sValues.rateOP = totTime/(statServ->m_tEnd-statServ->m_tStart);
	}
	
	return statServ->m_result;
} */

unsigned int statistics::getLQvectorStats(int index, servParamValues& sValues, map<int,serviceType *> servTypeList)
{
	statsPerServ_iter_t iter;
	statsPerServ *statServ;
	unsigned int i;
	int servTypeID;
	map<int,serviceType *>::iterator its;
	float capacity, tStart, tEnd, totTime=0, income=0;
	
	iter = m_statsPerServ.begin();
	advance(iter, index);
	statServ = iter->second;
	if (statServ->m_result == SUCCESS_CALL) {
		sValues.lqVector = statServ->m_linkQuality;
		sValues.servType = statServ->m_servType;
		sValues.duration = statServ->m_tEnd - statServ->m_tStart;
		sValues.numHOs = statServ->m_tHOs.size();
		// calcular porcentaje de tiempo con el operador preferido
		for (i=0; i<= statServ->m_tHOs.size(); i++) {
			// Calculo de tStart
			if (i==0) {
				tStart = statServ->m_tStart;
			}
			else {
				tStart = statServ->m_tHOs[i-1];
			}
			// Calculo de tEnd
			if (i==statServ->m_tHOs.size()) {
				tEnd = statServ->m_tEnd;
			}
			else {
				tEnd = statServ->m_tHOs[i];
			}
			// Verificar que tEnd sea mayor que tStart
			if (tStart > tEnd) {
				DBG_ERR("Error calculando service time, tStart %.2f tEnd %.2f", tStart, tEnd);
				exit(-1);
			}
			// Prefered Operator 
			if (statServ->m_prefOP[i]) {
				totTime = totTime + (tEnd - tStart);
			}
			// Ingresos por servicio
			income += statServ->m_price[i]*(tEnd-tStart); 
		}		
		
		sValues.rateOP = totTime/(statServ->m_tEnd-statServ->m_tStart);

		servTypeID = statServ->m_servType;
		its = servTypeList.find(servTypeID);
		if (its == servTypeList.end()) {
			DBG_ERR("Servicio %d no se encuentra en servTypeList!!", statServ->m_servType);
			exit(-1);
		}
		capacity = (*its).second->capacity();		
		sValues.income = income*capacity;

	}
	
	return statServ->m_result;
}

void statistics::getStateCallStats(int index, int& callID, int& stateCall, int& lqSize)
{
	statsPerServ_iter_t iter;
	
	iter = m_statsPerServ.begin();
	advance(iter, index);
	callID = iter->first;
	stateCall = iter->second->m_result;
	lqSize = iter->second->m_linkQuality.size();
}

/**
void statistics::getHandoverStats(map<int, int>& HOstats)
{
	statsPerServ_iter_t iter;
	statsPerServ *statServ;
	map<int, int>::iterator hoIter;
	int numHO;
	
	for (iter = m_statsPerServ.begin(); iter != m_statsPerServ.end(); iter++) {
		statServ = iter->second;
		if (statServ->m_result == SUCCESS_CALL) {
			numHO = statServ->m_tHOs.size();
			hoIter = HOstats.find(numHO);
			if (hoIter == HOstats.end()) { // Si numHO no se encuentra se inserta e inicializa a 1
				HOstats.insert(pair<int,int>(numHO, 1));
			}
			else { // Si numHO ya se encuentra en el map incremento el campo value
				(*hoIter).second++;
			}
		}
	}

}
*/

/*
	getHandoverStats obtiene el numero de HO realizados por tipo de servicio
	
*/
void statistics::getHOperServStats(map<int, vector<int> >& HOstats, int numOfServs)
{
	vector<int> totServs(numOfServs,0);
	statsPerServ_iter_t iter;
	statsPerServ *statServ;
	map<int, vector<int> >::iterator hoIter;
	int numHO, i;
	
	for (iter = m_statsPerServ.begin(); iter != m_statsPerServ.end(); iter++) {
		statServ = iter->second;
		if (statServ->m_result == SUCCESS_CALL) {
			numHO = statServ->m_tHOs.size();
			hoIter = HOstats.find(numHO);
			if (hoIter == HOstats.end()) { // Si numHO no se encuentra se inserta e inicializa a 0
				HOstats.insert(pair<int,vector<int> >(numHO, totServs));
			}
			hoIter = HOstats.find(numHO);
			for (i=0; i<numOfServs; i++) { // OJO! ¿no entiendo porque he puesto este for?
				if (i==statServ->m_servType) {
					//DBG_INFO("numOfServs %d m_servType %d", numOfServs,i);
					(*hoIter).second[i] = (*hoIter).second[i]++;				
				}
			}
		}
	}

}

void statistics::getHOperUserStats(map<int, vector<int> >& hoStats, int numTypeUsers)
{
	vector<int> iniTypeUsers(numTypeUsers,0);
	statsPerServ_iter_t iter;
	statsPerServ *statServ;
	map<int, vector<int> >::iterator hoIter;
	int numHO, i;
	
	for (iter = m_statsPerServ.begin(); iter != m_statsPerServ.end(); iter++) {
		statServ = iter->second;
		if (statServ->m_result == SUCCESS_CALL) {
			numHO = statServ->m_tHOs.size();
			hoIter = hoStats.find(numHO);
			if (hoIter == hoStats.end()) { // Si numHO no se encuentra se inserta e inicializa a 0
				hoStats.insert(pair<int,vector<int> >(numHO, iniTypeUsers));
			}
			hoIter = hoStats.find(numHO);
			i=statServ->m_userType;
			(*hoIter).second[i] = (*hoIter).second[i]++;				
		}
	}

}

// incomes: key = bsID, value = sum of prices
/**
	Mejor poner revenue en lugar de income??
*/
void statistics::getPriceStats(map<int,float>& incomes, map<int,serviceType *> servTypeList)
{
	statsPerServ_iter_t iter;
	statsPerServ *statServ;
	map<int,serviceType *>::iterator its;
	float capacity, tStart, tEnd;
	int bsID;
	unsigned int i;
	
	for (iter = m_statsPerServ.begin(); iter != m_statsPerServ.end(); iter++) {
		statServ = iter->second;
		if ((statServ->m_result == SUCCESS_CALL) || (statServ->m_result == DROP_CALL)) {
			for (i=0; i<= statServ->m_tHOs.size(); i++) {
				if (i==0) {
					tStart = statServ->m_tStart;
				}
				else {
					tStart = statServ->m_tHOs[i-1];
				}
				if (i==statServ->m_tHOs.size()) {
					if (statServ->m_result == SUCCESS_CALL) {
						tEnd = statServ->m_tEnd;
					}
					else { // DROP_CALL
						tEnd = statServ->m_tDrop;
					}
				}
				else {
					tEnd = statServ->m_tHOs[i];
				}
				if (tStart > tEnd) {
					DBG_ERR("Error calculando pref OP time, tStart %.2f tEnd %.2f", tStart, tEnd);
					exit(-1);
				}
				bsID = statServ->m_currBS[i];
				its = servTypeList.find(statServ->m_servType);
				if (its == servTypeList.end()) {
					DBG_ERR("Servicio %d no se encuentra en servTypeList!!", statServ->m_servType);
					exit(-1);
				}
				capacity = (*its).second->capacity();
				incomes[bsID] += (statServ->m_price[i]*(tEnd-tStart)*capacity); 
			}
		}
	}

}

// incomes: key = servTypeID, value = sum of prices
/* 
	Mejor poner payment en lugar de incomes??
*/
void statistics::getPricePerServStats(map<int,float>& incomes, map<int,serviceType *> servTypeList)
{
	statsPerServ_iter_t iter;
	statsPerServ *statServ;
	map<int,serviceType *>::iterator its;
	float capacity, tStart, tEnd;
	int servTypeID;
	unsigned int i;
	
	for (iter = m_statsPerServ.begin(); iter != m_statsPerServ.end(); iter++) {
		statServ = iter->second;
		if (statServ->m_result == SUCCESS_CALL) {
			for (i=0; i<= statServ->m_tHOs.size(); i++) {
				if (i==0) {
					tStart = statServ->m_tStart;
				}
				else {
					tStart = statServ->m_tHOs[i-1];
				}
				if (i==statServ->m_tHOs.size()) {
					tEnd = statServ->m_tEnd;
				}
				else {
					tEnd = statServ->m_tHOs[i];
				}
				if (tStart > tEnd) {
					DBG_ERR("Error calculando service time, tStart %.2f tEnd %.2f", tStart, tEnd);
					exit(-1);
				}
				servTypeID = statServ->m_servType;
				its = servTypeList.find(servTypeID);
				if (its == servTypeList.end()) {
					DBG_ERR("Servicio %d no se encuentra en servTypeList!!", statServ->m_servType);
					exit(-1);
				}
				capacity = (*its).second->capacity();
				incomes[servTypeID] += (statServ->m_price[i]*(tEnd-tStart)*capacity); 
			}
		}
	}

}

// payment: key = userTypeID, value = sum of payment
// userTypeList: definido en userType.h, map<int,userType *>
void statistics::getPayPerUserStats(map<int,float>& payment, userType_t userTypeList, serviceType_t servTypeList)
{
	statsPerServ_iter_t iter;
	statsPerServ *statServ;
	userType_iter_t itu;
	serviceType_iter_t its;
	float capacity, tStart, tEnd;
	int userTypeID, servTypeID;
	unsigned int i;
	
	for (i=0; i<userTypeList.size(); i++) {
		payment[i] = 0.0;
	}
	
	for (iter = m_statsPerServ.begin(); iter != m_statsPerServ.end(); iter++) {
		statServ = iter->second;
		if (statServ->m_result == SUCCESS_CALL) {
			for (i=0; i<= statServ->m_tHOs.size(); i++) {
				if (i==0) {
					tStart = statServ->m_tStart;
				}
				else {
					tStart = statServ->m_tHOs[i-1];
				}
				if (i==statServ->m_tHOs.size()) {
					tEnd = statServ->m_tEnd;
				}
				else {
					tEnd = statServ->m_tHOs[i];
				}
				if (tStart > tEnd) {
					DBG_ERR("Error calculando service time, tStart %.2f tEnd %.2f", tStart, tEnd);
					exit(-1);
				}
				userTypeID = statServ->m_userType;
				// Solo para verificar que el userType existe en la lista de tipos de usuarios
				itu = userTypeList.find(userTypeID);
				if (itu == userTypeList.end()) {
					DBG_ERR("User %d no se encuentra en userTypeList!!", statServ->m_userType);
					exit(-1);
				}
				servTypeID = statServ->m_servType;
				its = servTypeList.find(servTypeID);
				if (its == servTypeList.end()) {
					DBG_ERR("Servicio %d no se encuentra en servTypeList!!", statServ->m_servType);
					exit(-1);
				}				
				capacity = (*its).second->capacity();
				payment[userTypeID] += (statServ->m_price[i]*(tEnd-tStart)*capacity); 
			}
		}
	}

}
	
statsBSload_t statistics::getLoadStats(void)
{
	return m_statsBSload;
}

