#include "service.h"

#define DBG_LEVEL 1
#include "debug.h"

service::service(void)
{
	
}

service::~service(void)
{
	utServs_iter_t utsIter;
	
	for (utsIter = m_utServs.begin(); utsIter!=m_utServs.end(); utsIter++) {
		delete((*utsIter).second);
	}
	m_utServs.clear();
}

/*
	setServicesAssigned inserta los parametros de cada tipo de servicio asignado a un UT 
	en el vector m_utServs.
	
	serv: Lista de servicios (service IDs) asignados a un UT
	slist: Lista que define cada tipo de servicios (parametros)
	
*/

void service::setServicesAssigned(userServs_t serv, serviceType_t slist)
{
	vector<int>::iterator itv;
	serviceType_iter_t itm;
	servParams *params;
	pair<map<int,servParams*>::iterator,bool> ret;
 
	DBG_INFO2("Service assigned size %d",serv.size());
	for (itv=serv.begin(); itv < serv.end(); itv++ ) {
		DBG_INFO2("Service ID %d",*itv);
	}
	
	for (itv=serv.begin(); itv < serv.end(); itv++ ) {
		itm = slist.find(*itv);
		if (itm == slist.end()) {
			DBG_ERR("Servicio %d no se encuentra en lista de tipos de servicios!!", *itv);
			exit(-1);
		}
		DBG_INFO2("Service ID %d is found", *itv);
		params = new servParams;
		params->m_tia = (*itm).second->tia();
		params->m_ts = (*itm).second->ts();
		params->m_capacity = (*itm).second->capacity();
		params->m_realtime = (*itm).second->realtime();
		params->m_servTypeID = (*itm).first;
		//m_utServs[(*itm).first] = params;
		ret = m_utServs.insert(pair<int,servParams*>((*itm).first,params));

		if (ret.second == false) {
			DBG_ERR("Element %d already existed into m_utServs", (*itm).first);
			exit(-1);
		}

	}
}

vector<int> service::getServTypeIDAssigned(void)
{
	utServs_iter_t its;
	vector<int> servIDs;
	
	for (its = m_utServs.begin(); its != m_utServs.end(); its++) {
		servIDs.push_back(its->second->m_servTypeID);
	}
	return servIDs;
}

void service::createServicesEvents(double simulTime, vector<servData_t *> &serv)
{
	utServs_iter_t its;
	int sType;
	servParams *sParams;
	double currTime;
	bool firstServ;
	static int callID_ = 0;
	servData_t *sData, *sDataPrev;
	vector<servData_t *>::iterator sIter;
	string tsFilename;
	string tiaFilename;
	string srvFilename;
	// FILE *tsFile;
	// FILE *tiaFile;
	// FILE *srvFile;
	// char buff[4];
	//int eventCounter;
	//double tStart, tEnd;
	//bool cycle = false;
	
	// Para cada servicio asignado al UT se crea los eventos durante todo el tiempo de simulacion 
	for (its=m_utServs.begin(); its!=m_utServs.end(); its++) {
		sType = its->first;
		sParams = its->second;
		m_state = IDLE; 
		currTime = 0.0;
		firstServ = true;
		//eventCounter = 0;
		// sprintf(buff,"%d",sType);
		// tsFilename = "ts_servID_" + string(buff) + ".txt";
		// tiaFilename = "tia_servID_" + string(buff) + ".txt";
		// srvFilename = "srv_servID_" + string(buff) + ".txt";
		// tsFile = fopen(tsFilename.c_str(), "w");
		// tiaFile = fopen(tiaFilename.c_str(), "w");
		// srvFile = fopen(srvFilename.c_str(), "w");
		//DBG_INFO("Generating ts, tia and srv files for service ID %d",sType);
		do {
			sData = new servData_t;
			switch (m_state) {
			case IDLE:
				// De IDLE pasa a ACTIVE
				m_eventTime = rng::instance()->exponential(sParams->m_tia);
				// fprintf(tiaFile,"%0.2f\n",m_eventTime);
				m_state = ACTIVE;
				sData->state = m_state;
				break;
			case ACTIVE:
				// De ACTIVE pasa a IDLE
				m_eventTime = rng::instance()->exponential(sParams->m_ts);
				// fprintf(tsFile,"%0.2f\n",m_eventTime);
				m_state = IDLE;
				sData->state = m_state;
				break;
			}
			currTime = currTime + m_eventTime;
			// Solo para verificar tiempos de servicios con MatLab
			// if (cycle) {
				// tEnd = currTime;
				// fprintf(srvFile,"%2d %0.2f %0.2f\n",sType, tStart,tEnd);
				// cycle = false;
			// }
			// else {
				// tStart = currTime;
				// cycle = true;				
			// }

			// Despues que el primer servicio del mismo tipo ha sido creado se actualiza su tiempo final
			if (!firstServ) {
				sDataPrev = serv.back();
				sDataPrev->tEnd = currTime;
			}
			
			sData->callID = callID_++;
			sData->servType = sType;
			sData->tWhen = currTime;
			sData->tEnd = 0;
			sData->endEvID = 0; // Para el caso de fin de llamada se guardara posteriormente el ID del evento correspondiente
			sData->ccEvID = 0; // Se asume que ningun evento CC tendra ID = 0, entonces 0 significa que no hay CC event asignado al UT
			// DBG_INFO("Service event callID %d tWhen %8.2f tEnd %.2f servType %d state %d", 
				// sData->callID, sData->tWhen, sData->tEnd, sData->servType, sData->state);
			sData->userPrice = 0; // Cuando se determine la BS por la que se curse el servicio se asignara el precio correspondiente
			serv.push_back(sData);
			firstServ = false;
			//eventCounter++;
		} while (currTime < simulTime);
		
		// Eliminar el evento creado despues del simulTime
		//DBG_INFO("Borrando last callID %d state %d", sData->callID, sData->state);
		serv.pop_back();
		callID_--; // Necesario para mantener numeracion sucesiva
		// Si el evento eliminado es IDLE tambien eliminar el anterior (ACTIVE) para evitar dejar el ACTIVE sin su respectivo IDLE.
		if (sData->state == IDLE) { 
			//DBG_INFO("Borrando before last callID %d state %d", sDataPrev->callID, sDataPrev->state);
			serv.pop_back();
			callID_--; // Necesario para mantener numeracion sucesiva
			delete sDataPrev;
		}
		delete sData;
		
		// Si el ultimo evento es ACTIVE, añadir evento final IDLE por la necesidad de tener acceso a endEVID.
		/**
		if (sData->state == ACTIVE) { 
			sData = new servData_t;
			sData->callID = callID_++;
			sData->servType = sType;
			sData->tEnd = 0;
			m_eventTime = rng::instance()->exponential(sParams->m_ts);
			currTime = currTime + m_eventTime;
			sData->tWhen = currTime;
			sData->state = IDLE;
			sData->endEvID = 0;
			sData->ccEvID = 0;
			sDataPrev = serv.back();
			sDataPrev->tEnd = currTime;			
			serv.push_back(sData);
		} */
	
		//DBG_INFO("with %d events ...", eventCounter);
		// fclose(tsFile);
		// fclose(tiaFile);
		// fclose(srvFile);
	}
	
	// Bucle for solo para comprobar actualizacion del campo "tEnd"
	// for (sIter = serv.begin(); sIter != serv.end(); sIter++) {
		// sData = *sIter;
		// DBG_INFO("Service event callID %d tWhen %.2f tEnd %.2f servType %d state %d", 
			// sData->callID, sData->tWhen, sData->tEnd, sData->servType, sData->state);
	// }
		
}