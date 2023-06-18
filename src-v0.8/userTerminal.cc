#include "userTerminal.h"
#include "geometry.h"

#define DBG_LEVEL 1
#include "debug.h"

userTerminal::userTerminal(accessSelection *&accSel_)
{
	DBG_IN("userTerminal::userTerminal");

	m_currMovIndex = 0;
	//m_currServReg = NULL;
	m_movement = new movement;
	m_service = new service;
	m_accSel = accSel_;
	DBG_OUT("userTerminal::userTerminal");
}

userTerminal::~userTerminal()
{
	DBG_IN("userTerminal::~userTerminal");

	delete m_movement;
	delete m_service;
	
	for (unsigned int i=0; i < m_movReg.size(); i++) {
		delete m_movReg[i];
	}
	m_movReg.clear();
	
	for (unsigned int i=0; i < m_servReg.size(); i++) {
		delete m_servReg[i];
	}
	m_servReg.clear();
	
	DBG_OUT("userTerminal::~userTerminal");
}

void userTerminal::onEvent(struct eventMessage *message)
{
	float resourcesRequested_;


	unsigned char evCode;
	unsigned long int evID;
	servData_t *sData;
	
	DBG_IN("userTerminal::onEvent");

	evID = message->id;
	evCode = message->code;
	sData = (servData_t *) message->data;
	
	switch(evCode) {
		case UT_MOVEMENT_EVENT:
			DBG_INFO("UT_MOVEMENT_EVENT utID %d at time %.2f",m_utID,eventQueue::instance()->currentTime());
			updateMovIndex();
			//DBG_INFO2("UT_MOVEMENT_EVENT evID %lu movIdx %d utID %d tStart %f", evID, m_currMovIndex, m_utID, m_movReg[m_currMovIndex]->tStart);
			searchCCeventsForServices();
			break;
		case UT_SERVICE_EVENT:
			DBG_INFO("UT_SERVICE_EVENT evID %lu", evID);
			resourceManagementBS(sData, evCode);
			break;
		case UT_MONITORING_EVENT:
			DBG_INFO("UT_MONITORING_EVENT evID %lu callID %d", evID, sData->callID);
			deletePMevent(sData, evID);
			resourcesRequested_ = getcurrServCapacity(sData);
			requestResourcesToBS(resourcesRequested_, sData, evCode);
			break;
		case UT_LINKQUALITY_EVENT:
			DBG_INFO("UT_LINKQUALITY_EVENT evID %lu callID %d", evID, sData->callID);
			deleteLQevent(sData, evID);
			detectLinkQuality(sData);
			break;
		case UT_CC_EVENT:
			DBG_INFO("UT_CC_EVENT evID %lu callID %d", evID, sData->callID);
			resourcesRequested_ = getcurrServCapacity(sData);
			requestResourcesToBS(resourcesRequested_, sData, evCode);
			break;
		default:
			DBG_ERR("Unrecognized event code %d", evCode);
		break;
	}

	DBG_OUT("userTerminal::onEvent");	

}

void userTerminal::updateMovIndex(void)
{
	
	DBG_IN("userTerminal::updateMovIndex");
	
	try
	{	
		m_currMovIndex++;
		//DBG_WARN("m_currMovIndex %d", m_currMovIndex)
	}
	catch (int e)
	{
		DBG_ERR("updateMovIndex: An exception occurred. Exception Nr. %d", e);
		exit(-1);
	}

	DBG_OUT("userTerminal::updateMovIndex");
}

void userTerminal::detectLinkQuality(servData_t *sData)
{
	double distance, bsRadio, currTime;
	float lqValue;
	geometry gm;
	basestation *bs;
	
	bs = findServConnected(sData->servType);
	currTime = eventQueue::instance()->currentTime();
	distance = gm.distancePoints(getcurrPosition(currTime),bs->getPosition());
	bsRadio = bs->range();
	lqValue = 1-distance/bsRadio;
	if (lqValue < 0) {
		DBG_ERR("LQ negativo %.2f distance %.2f bsRadio %.2f", lqValue, distance, bsRadio);
		DBG_INFO2("bsID %d utID %d callID %d servType %d", bs->bsID(), m_utID, sData->callID, sData->servType);
		exit(-1);
	}
	statistics::instance()->statsInsert_lq(sData->callID, lqValue);
}

void userTerminal::searchCCeventsForServices(void)
{
	servState_iter_t its;
	servData_t *sData;
	basestation *bs;
	bool stateVer;
	
	DBG_INFO2("searchCCeventsForServices for utID %d", m_utID);
	for (its=m_servState.begin(); its!=m_servState.end(); its++) {
		sData = (*its).second.sData;
		stateVer = (*its).second.state;
		//DBG_INFO2("sData points to %p",sData);
		//DBG_INFO2("\tservType %d state %d stateVer %d", sData->servType, sData->state, stateVer);
		if (stateVer) { // Si el servicio esta activo
			DBG_INFO2("\tservType %d state %d stateVer %d", sData->servType, sData->state, stateVer);
			bs = findServConnected(sData->servType); // deberia estar conectado a un BS
			if (bs != NULL) {
				DBG_INFO2("Detecting CC event for movement event, servType %d",sData->servType);
				addCCevent(sData, bs);
			}
			else { // si no esta conectado a un BS entonces error!!
				DBG_ERR("servType %d en utID %d no se encuentra en m_currBSconn", sData->servType, m_utID);
				exit(-1);	
			}
		}
	}
	
}

// VERIFICAR! retorna el estado del servicio ¿actual?
/*
	Actualiza el mapa m_servState de acuerdo con el estado actual (ACTIVE/IDLE) del servicio especificado en *data
*/
int userTerminal::updateServState(servData_t *sData)
{
	servState_iter_t its;
	int servType_, state_;
	stateData_t stateData;
	
	servType_ = sData->servType;
	state_ = sData->state;

	its = m_servState.find(servType_);
	if (its == m_servState.end()) {
		DBG_ERR("Servicio %d no se encuentra en m_servState!!", servType_);
		exit(-1);
	}
	
	switch (state_) {
	case IDLE:
		stateData.state = false;
		stateData.sData = sData;
		//(*its).second = false;
		(*its).second = stateData;
		break;
	case ACTIVE:
		stateData.state = true;
		stateData.sData = sData;
		//(*its).second = true;
		(*its).second = stateData;
		break;
	}

	return state_;
	
}

void userTerminal::cleanServState(int servType)
{
	servState_iter_t its;
	stateData_t stateData;
	
	its = m_servState.find(servType);
	if (its == m_servState.end()) {
		DBG_ERR("Servicio %d no se encuentra en m_servState!!", servType);
		exit(-1);
	}

	stateData.state = false;
	stateData.sData = NULL;
	(*its).second = stateData;
	
}

float userTerminal::getcurrServCapacity(servData_t *sData)
{
	utServs_iter_t its;
	int servType_;
	utServs_t utServs_;
	
	servType_ = sData->servType;
	
	utServs_= m_service->getServicesAssigned();
	its = utServs_.find(servType_);
	if (its == utServs_.end()) {
		DBG_ERR("Servicio %d no se encuentra en m_utServs!!", servType_);
		exit(-1);
	}
	
	return its->second->m_capacity;
}

void userTerminal::resourceManagementBS(servData_t *sData, unsigned char code)
{
	int state_;
	float resourcesRequested_;
	int callID;
	int servType;
	basestation bs_;
	
	//DBG_INFO2("EVENT serv ut%02d at time %.2f",m_utID,eventQueue::instance()->currentTime());
	state_ = updateServState(sData);
	resourcesRequested_ = getcurrServCapacity(sData);
	servType = sData->servType;
	callID = sData->callID;
	
	switch (state_) {
	case IDLE:
		DBG_INFO2("Service IDLE for utID %d servType %d callID-ini %d", m_utID, servType, callID-1);
		m_accSel->releaseResourcesOfBS(this, resourcesRequested_, sData);
		statistics::instance()->statsInsert_result(callID-1, SUCCESS_CALL);
		break;
	case ACTIVE:
		DBG_INFO2("Service ACTIVE for utID %d servType %d callID %d", m_utID, servType, callID);
		statistics::instance()->initStatsPerServ(callID);
		statistics::instance()->statsInsert_tStart(callID, sData->tWhen);
		statistics::instance()->statsInsert_tEnd(callID, sData->tEnd);
		statistics::instance()->statsInsert_userType(callID, m_userTypeID);
		statistics::instance()->statsInsert_servType(callID, servType);
		requestResourcesToBS(resourcesRequested_, sData, code);
		break;
	}
	
}

void userTerminal::requestResourcesToBS(float resourcesRequested_, servData_t *sData, unsigned char code)
{
	basestation *bs_;
	
	m_accSel->getDetectedSet(this);
	m_accSel->getValidatedSet(this, sData);
	bs_ = m_accSel->getCandidateSet(this, resourcesRequested_, sData);
	if(bs_ == NULL && code == UT_MONITORING_EVENT) {
		DBG_WARN("A call can't be continued after a monitoring event");
	}
	m_accSel->assignResourcesToBS(this, bs_, resourcesRequested_, sData, code);
}

/*
	tDeltaMov y tDeltaServ son los intervalos de tiempo desde el inicio del movimiento e inicio del
	servicio, respectivamente, hasta el punto de cruce entre la recta del movimiento y el circulo de 
	la BS. Teoricamente el tiempo de inicio del servicio (tWhen) debe ser igual al tiempo actual dentro
	de la simulacion porque aun esta en fase de asignacion de recursos en la BS. El tiempo de inicio
	del movimiento es un tiempo pasado pero que se utiliza para encontrar el punto de cruce.
*/

void userTerminal::addCCevent(servData_t *sData, basestation *bs)
{
	point_t pi, pf, px, pc;
	circle_t c;
	geometry gm;
	double alpha, speed;
	double tDeltaMov, tCCevent;
	unsigned long int evID;
	double currTime;

	DBG_INFO2("currTime %f tWhen %f", eventQueue::instance()->currentTime(), sData->tWhen);
	// Considerando que un evento con ID=0 nunca sera un evento CC, se fija ccEvID=0 para
	// indicar que el servicio no tiene evento CC
	if (sData->ccEvID != 0) {
		DBG_WARN("servType %d ya tiene un CC event evID %lu", sData->servType, sData->ccEvID);
		exit(-1);
	}
	
	if (m_movReg[m_currMovIndex]->speed > 0) {
		c.m_x = bs->posX();
		c.m_y = bs->posY();
		c.radius = bs->range();
		currTime = eventQueue::instance()->currentTime();
		pc = getcurrPosition(currTime);
		if (!gm.insideCircle(pc, c)) {
			DBG_ERR("El punto actual deberia estar dentro del circulo");
			exit(-1);
		}
		pi = m_movReg[m_currMovIndex]->iniPos;		
		pf = m_movReg[m_currMovIndex]->finPos;
		DBG_INFO2("\tUT final position is (%.2f,%.2f)",pf.m_x,pf.m_y);
		speed = m_movReg[m_currMovIndex]->speed;
		alpha = m_movReg[m_currMovIndex]->direction;
		if (!gm.insideCircle(pf,c)) {
			px = gm.intersection(pc,pf,c);
			tDeltaMov = (px.m_x - pc.m_x)/(speed*cos(alpha));
			if (tDeltaMov < 0) {
				DBG_ERR("tDeltaMov negative %.2f. Please fix it!", tDeltaMov);
				exit(-1);
			}
			tCCevent = currTime + tDeltaMov;
			if (sData->tEnd > tCCevent) {
				evID = eventQueue::instance()->insertEvent(UT_CC_EVENT,tDeltaMov,false,sData,this);
				sData->ccEvID = evID;
				DBG_INFO2("addCCevent utID %d servType %d bsID %d evID %lu", m_utID, sData->servType, bs->bsID(), sData->ccEvID);
			}
		}
	}
}

/*
	Solo se elimina de manera especifica un evento CC cuando cleanCCevent es llamado por un
	evento diferente a UT_CC_EVENT, porque en caso contrario el CC event es eliminado automaticamente
	por el gestor de eventos (event.cc) 
*/
void userTerminal::cleanCCevent(servData_t *sData, unsigned char code)
{
	DBG_INFO2("cleanCCevent");
	DBG_INFO2("Deleting CC event %lu, called from code %d",sData->ccEvID,code);
	if ((code != UT_CC_EVENT) && (sData->ccEvID != 0)){
		//DBG_INFO2("Deleting event %lu",sData->ccEvID);
		eventQueue::instance()->deleteEvent(sData->ccEvID);
	}
	sData->ccEvID = 0;
}

void userTerminal::addLQevents(servData_t *sData)
{
	double tStep;
	unsigned long int evID;
	double aux_;
	int numEvs_, i;

	aux_ = 1/m_lqStep;
	numEvs_ = (int) aux_ - 1;
	tStep = (sData->tEnd - sData->tWhen)*m_lqStep;

	for(i = 0 ; i < numEvs_ ; i++) {
		evID = eventQueue::instance()->insertEvent(UT_LINKQUALITY_EVENT,tStep*(i+1),false,sData,this);
		sData->listLQevIDs.push_back(evID);
	}
	
}

void userTerminal::cleanLQevents(servData_t *sData)
{
	vector<unsigned long int> ids;
	vector<unsigned long int>::iterator itv;
	
	DBG_INFO2("cleanLQevents");
	ids = sData->listLQevIDs;
	for (itv=ids.begin(); itv<ids.end(); itv++) {
		eventQueue::instance()->deleteEvent(*itv);
	}
	(sData->listLQevIDs).clear();
}

void userTerminal::deleteLQevent(servData_t *sData, unsigned long int evID)
{
	vector<unsigned long int>::iterator itv;
	
	DBG_INFO2("deletePMevent");

	itv = find((sData->listLQevIDs).begin(), (sData->listLQevIDs).end(), evID);
	if (itv == (sData->listLQevIDs).end()) {
		DBG_ERR("evID %lu no se encuentra en listLQevIDs!!", evID);
		exit(-1);
	}
	DBG_INFO2("evID %lu found and deleted", evID);
	(sData->listLQevIDs).erase(itv);
}

/*
	Inserta los todos los eventos de monotorizacion periodica que puedan existir dentro del periodo de duracion
	del servicio. Se divide la duracion del servicio en ranuras de tiempo (tStep) en funcion de m_pmStep. Para 
	cada incremento de tiempo tStep se inserta un evento de monotorización y se guarda el ID del evento en el
	vector listPMevIDs para ser utilizada por cleanPMevents().
*/
void userTerminal::addPMevents(servData_t *sData)
{
	double tStep;
	unsigned long int evID;
	double aux_;
	int numEvs_, i;

	aux_ = 1/m_pmStep;
	numEvs_ = (int) aux_ - 1;
	tStep = (sData->tEnd - sData->tWhen)*m_pmStep;

	for(i = 0 ; i < numEvs_ ; i++) {
		evID = eventQueue::instance()->insertEvent(UT_MONITORING_EVENT,tStep*(i+1),false,sData,this);
		sData->listPMevIDs.push_back(evID);
	}
	
}

void userTerminal::cleanPMevents(servData_t *sData)
{
	vector<unsigned long int> ids;
	vector<unsigned long int>::iterator itv;
	
	DBG_INFO2("cleanPMevents");
	ids = sData->listPMevIDs;
	for (itv=ids.begin(); itv<ids.end(); itv++) {
		eventQueue::instance()->deleteEvent(*itv);
	}
	(sData->listPMevIDs).clear();
}

void userTerminal::deletePMevent(servData_t *sData, unsigned long int evID)
{
	//vector<unsigned long int> ids;
	vector<unsigned long int>::iterator itv;
	
	DBG_INFO2("deletePMevent");
	//ids = sData->listPMevIDs;
	itv = find((sData->listPMevIDs).begin(), (sData->listPMevIDs).end(), evID);
	if (itv == (sData->listPMevIDs).end()) {
		DBG_ERR("evID %lu no se encuentra en listPMevIDs!!", evID);
		exit(-1);
	}
	DBG_INFO2("evID %lu found and deleted", evID);
	(sData->listPMevIDs).erase(itv);
}

void userTerminal::makeServices(double simulTime)
{
	vector<servData_t *>::iterator its;
	unsigned long int evID;
	servData_t *sData, *sDataPrev;
	bool initialization=true;
	int currServ;
	
	m_service->createServicesEvents(simulTime, m_servReg);

	for (its=m_servReg.begin(); its!=m_servReg.end(); its++) {
		evID = eventQueue::instance()->insertEvent(UT_SERVICE_EVENT,(*its)->tWhen,false,*its,this);
		/* Proceso para colocar el ID del evento actual (evID) al registro servData previo. Asi cada registro de un servicio activo (1) 
		   tendra un campo que indique el ID del evento final correspondiente, para eliminarlo en caso que el servicio sea rechazado */
		sData = (servData_t *)(*its);
		if (initialization) {
			currServ = sData->servType;
			sDataPrev = sData;
			initialization = false;
		}
		if (currServ == sData->servType) {
			if (sData->state == IDLE) {
				sDataPrev->endEvID = evID;
			}
		}
		else {
			currServ = sData->servType;
		}
		//DBG_INFO2(">s utID %d callID %d servType %d tWhen %.2f state %d endEvID %lu", m_utID, sDataPrev->callID, sDataPrev->servType, sDataPrev->tWhen, sDataPrev->state, sDataPrev->endEvID);
		sDataPrev = sData;
	}
	
	//DBG_INFO2("RESUMEN SERVICIOS");
	//eventQueue::instance()->printEvents();
	// for (its=m_servReg.begin(); its!=m_servReg.end(); its++) {
		// sData = (servData_t *)(*its);
		// DBG_INFO2(">s utID %3d callID %4d servType %d state %d tWhen %8.2lf tEnd %8.2lf endEvID %lu", m_utID, sData->callID, sData->servType, sData->state, sData->tWhen, sData->tEnd, sData->endEvID);
	// }

}

void userTerminal::printServicesToFile(FILE *servTraceFile_)
{
	vector<servData_t *>::iterator its;
	
	for (its=m_servReg.begin(); its!=m_servReg.end(); its++) {
		fprintf(servTraceFile_, "%4d %4d %4d %8.2f %4d\n", m_utID, (*its)->callID,
				(*its)->servType, (*its)->tWhen, (*its)->state);
	}

}

/*
	Inserta los servicios provenientes de la lectura de un archivo de trazas
*/
unsigned long int userTerminal::insertService(servData_t *serv_)
{
	unsigned long int evID;
	
	m_servReg.push_back(serv_);
	evID = eventQueue::instance()->insertEvent(UT_SERVICE_EVENT,serv_->tWhen,false,serv_,this);

	return evID;
}

/*
	Solo se generan movimientos en los UTs con modelo de movimiento diferente a MOVNONE.
*/
void userTerminal::makeMovements(double simulTime)
{
	double to_ = 0.0;
	movData_t *mov_;

	DBG_IN("userTerminal::makeMovements");
	
	//DBG_INFO("Movements for utID %d model %d", m_utID, getMovModel());
	//DBG_INFO("utID movID tStart    xIni    yIni      xFin     yFin  direction  speed");
				
	mov_ = new movData_t;
	mov_->utID = m_utID;
	mov_->movID = m_movement->movID();
	mov_->iniPos = m_movement->iniPos();
	mov_->finPos = m_movement->finPos();
	mov_->direction = m_movement->currDirection();
	mov_->speed = m_movement->currSpeed();
	mov_->tStart = 0;
	//printMovement(mov_);
	m_movReg.push_back(mov_);
		
	to_ = m_movement->nextEvent();
	do {
		mov_ = new movData_t;
		m_movement->updateNodePosition();
		mov_->utID = m_utID;
		mov_->movID = m_movement->movID();
		mov_->iniPos = m_movement->iniPos();
		mov_->finPos = m_movement->finPos();
		mov_->direction = m_movement->currDirection();
		mov_->speed = m_movement->currSpeed();
		mov_->tStart = to_;
		//printMovement(mov_);
		
		m_movReg.push_back(mov_);
		eventQueue::instance()->insertEvent(UT_MOVEMENT_EVENT,to_,false,NULL,this);
		to_ = to_ + m_movement->nextEvent();
	} while (to_ < simulTime);
	
	DBG_OUT("userTerminal::makeMovements");
}

/*
	Para aquellos UTs que no se mueven (modelo de movimiento: MOVNONE) se fija principalmente
	el valor de speed a cero.
*/
void userTerminal::setUTmoveNone(void)
{
	movData_t *mov_;

	DBG_IN("userTerminal::setUTmoveNone");
	
	mov_ = new movData_t;
	mov_->utID = m_utID;
	mov_->iniPos = m_movement->iniPos();
	mov_->finPos = m_movement->finPos();
	mov_->speed = 0;
	m_movReg.push_back(mov_);
	
	DBG_OUT("userTerminal::setUTmoveNone");
}

/*
	Teniendo en cuenta que m_currMovIndex permite localizar los valores del movimiento actual.
	Este metodo calcula la posicion x,y del terminal en el instante de tiempo en que es llamado.
	currTime es el tiempo actual en el cual se toma la foto.
*/
point_t userTerminal::getcurrPosition(double currTime)
{
	point_t pi, po;
	double alpha;
	double tDelta;
	movData_t *movEvent_;
	
	/* tDelta es la diferencia de tiempo entre el tiempo actual (para el caso de las
	fotos se incrementa por el stepTime) y el tiempo en el que se inicia un
	nuevo movimiento
	*/
	tDelta = currTime-m_movReg[m_currMovIndex]->tStart;
	
	//DBG_INFO2("utID=%2d m_currMovIndex=%3d tStart=%.2f currTime=%.2f", m_utID, m_currMovIndex,m_movReg[m_currMovIndex]->tStart, currTime);
	if (tDelta < 0) {
		DBG_ERR("tDelta negativo");
		DBG_INFO2("currTime %f", currTime);
		printMovement(m_movReg[m_currMovIndex]);
		exit(-1);
	}
	
	movEvent_ = m_movReg[m_currMovIndex];
	pi = movEvent_->iniPos;
	alpha = movEvent_->direction;
	
	if (movEvent_->speed == 0) {
		po = pi;
	}
	else {
		po.m_x = pi.m_x + (movEvent_->speed)*tDelta*cos(alpha);
		po.m_y = pi.m_y + (movEvent_->speed)*tDelta*sin(alpha);
	}

	if (pointOutOfRange(po)) {
		DBG_ERR("Out of range utID %d pi.x=%.2f pi.y=%.2f alpha=%.2f tDelta=%.2f po.x=%.2f po.y=%.2f",m_utID, pi.m_x,pi.m_y,alpha,tDelta,po.m_x,po.m_y);
		exit(-1);
	}
	
	return po;
	
}

bool userTerminal::pointOutOfRange(point_t po)
{
	bool range = false;
	
	if ((po.m_x < 0) || (po.m_x > m_movement->xMax())) range = true;
	if ((po.m_y < 0) || (po.m_y > m_movement->yMax())) range = true;
	
	return range;
	
}

int userTerminal::getMovModel(void)
{
	return m_movement->model();
}

void userTerminal::printMovement(movData_t *mov_)
{

	DBG_INFO("%3d %3d %8.2f %8.2f %8.2f %8.2f %8.2f %8.2f %8.2f", mov_->utID, mov_->movID, mov_->tStart, 
				mov_->iniPos.m_x, mov_->iniPos.m_y, mov_->finPos.m_x, mov_->finPos.m_y, mov_->direction, mov_->speed);
	//DBG_INFO2("%3d %3d %8.2f", mov_->utID, mov_->movID, mov_->speed);
}

void userTerminal::printMovementsToFile(FILE *movTraceFile_)
{
	vector<movData_t *>::iterator it;
	
	for (it=m_movReg.begin(); it!=m_movReg.end(); it++) {
		fprintf(movTraceFile_, "%4d %4d %8.2f %8.2f %8.2f %8.2f %8.2f %8.2f %8.2f\n", (*it)->utID, (*it)->movID, (*it)->tStart, 
				(*it)->iniPos.m_x, (*it)->iniPos.m_y, (*it)->finPos.m_x, (*it)->finPos.m_y, (*it)->direction, (*it)->speed);
	}

}

void userTerminal::insertMovement(movData_t *mov_)
{
	static int nextID = 0;
	
	m_movReg.push_back(mov_);
	if (nextID == mov_->utID) {
		nextID++;
	}
	else {
		eventQueue::instance()->insertEvent(UT_MOVEMENT_EVENT,mov_->tStart,false,NULL,this);
	}
}

point_t userTerminal::getIniPosition(void)
{
	return m_movement->iniPos();
}

void userTerminal::initialConfiguration(void)
{
	m_movement->movID() = 0;
	m_movement->iniPos() = m_movement->genNewPosition();
	m_movement->finPos() = m_movement->iniPos();
	m_movement->currSpeed() = 0;
	m_movement->currDirection() = 0;
	m_movement->newDirection() = 0;
	m_movement->state() = PAUSED;
	m_movement->nextEvent() = m_movement->initialPauseTime();
	m_movement->timeLeft() = 0;
	
	//DBG_INFO2("From initial configuration - Next Event = %.2f",m_movement->nextEvent());
	
}

void userTerminal::setAreaLimits(double xMax, double yMax)
{
    DBG_IN("userTerminal::setAreaLimits");

    m_movement->xMax() = xMax;
    m_movement->yMax() = yMax;

    DBG_OUT("userTerminal::setAreaLimits");
}

void userTerminal::setMovementParams(int model, struct movParams params)
{
	m_movement->model() = model;
	
	if (model==RWP_PLAIN) {
		m_movement->minSpeed() = params.minSpeed;
		m_movement->maxSpeed() = params.maxSpeed;
		m_movement->minPauseTime() = params.minPauseTime;
		m_movement->maxPauseTime() = params.maxPauseTime;
	}
	else if ((model==RWP_REFLECT)||(model==RWP_WRAP)) {
		m_movement->minSpeed() = params.minSpeed;
		m_movement->maxSpeed() = params.maxSpeed;
		m_movement->minPauseTime() = params.minPauseTime;
		m_movement->maxPauseTime() = params.maxPauseTime;
		m_movement->minTravelTime() = params.minTravelTime;
		m_movement->maxTravelTime() = params.maxTravelTime;
		m_movement->minDirection() = params.minDirection;
		m_movement->maxDirection() = params.maxDirection;
	}

}

/*
	ulist: Lista de tipos de usuarios
	slist: Lista de tipos de servicios
	Cada tipo de usuario tiene un conjunto se servicios asignados (service IDs)
	Cada tipo de servicio esta definido por un conjunto de paramatros.
*/

void userTerminal::setServiceParams(userType_t ulist, serviceType_t slist)
{
	vector<int> servIDs;
	vector<int>::iterator it;
	stateData_t stateData;
	
	m_service->setServicesAssigned(ulist[m_userTypeID]->getuserServs(), slist);
	//m_service->setUTservs(ulist[m_userTypeID]->getuserServs());
	
	// Inicializacion de todos los servicios asignados a FALSE (indicando de esta manera que al inicio
	// no generan trafico) y el sData a NULL porque al inicio no tiene servicio asignado
	servIDs = m_service->getServTypeIDAssigned();
	for (it=servIDs.begin(); it!=servIDs.end(); it++) {
		stateData.state = false;
		stateData.sData = NULL;
		m_servState.insert(pair<int,stateData_t>(*it,stateData));
	}
	
	// Solo para imprimir
	// servState_iter_t its;
	// ostringstream aux;
	// for (its=m_servState.begin(); its!=m_servState.end(); its++) {
		// aux << " " << its->first;
	// }
	// DBG_INFO2("utID %d services assigned %s",m_utID,(char *) aux.str().c_str());
	
}

servState_t userTerminal::getServicesState(void)
{

	return m_servState;
	
}

/*
	Permite almacenar las BSs con las cuales un UT esta conectado haciendo uso de
	un determinado tipo de servicio. 
	
	??? Para el caso de que dicho servicio ya esta
	siendo cursado a traves de un BS (monitorizar la calidad de servicio), se 
	modificara la BS hacia la cual a cambiado de conexion.
*/
void userTerminal::connectToBS(int servType, basestation *bs)
{
	pair<map<int,basestation*>::iterator,bool> ret;
	
	ret = m_currBSconn.insert(pair<int,basestation *>(servType,bs));

	if (ret.second == false) { // !OJO verificar que significa esto
		DBG_HIGH("For utID %d servType %d already connected to bsID %d", m_utID, servType, ret.first->second->bsID());
		exit(-1);
		// (ret.first)->second = bs;
		// DBG_INFO2("...new bsID %d", m_currBSconn.find(servType)->second->bsID());
	}
	DBG_INFO2("connectToBS: utID %d currBSconn size %d", m_utID, m_currBSconn.size());
}

void userTerminal::disconnectFromBS(int servType)
{
	currBSconn_iter_t iter;
	
	iter = m_currBSconn.find(servType);
	if (iter == m_currBSconn.end()) {
		DBG_ERR("utID %d servType %d no se encuentra en m_currBSconn", m_utID, servType);
		exit(-1);
	}
	DBG_INFO2("disconnectFromBS: utID %d servType %d from bsID %d", m_utID, servType, (*iter).second->bsID());
	m_currBSconn.erase(iter);

}

basestation *userTerminal::findServConnected(int servType)
{
	currBSconn_iter_t iter;
	
	iter = m_currBSconn.find(servType);
	if (iter == m_currBSconn.end()) {
		return NULL;
	}
	else {
		return iter->second;
	}
	
}
