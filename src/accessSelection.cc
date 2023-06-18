#include "accessSelection.h"
#include "configScenario.h"
#include "userTerminal.h"
#include "geometry.h"
#include "statistics.h"

#include <cmath>

#define DBG_LEVEL 1
#include "debug.h"

bool compareDS(dataDS_t i,dataDS_t j)
{
	return (i.weight>j.weight);
}

accessSelection::accessSelection()
{
	DBG_IN("accessSelection::accessSelection");

	DBG_OUT("accessSelection::accessSelection");
}

accessSelection::~accessSelection()
{
	DBG_IN("accessSelection::~accessSelection");
	
	// Limpiar m_dsList
	m_dsList.clear();

	DBG_OUT("accessSelection::~accessSelection");
}

void accessSelection::getBSs(bs_t &bsList_)
{
	m_bsList = bsList_;
	
}

void accessSelection::getcfgSce(configScenario *&cfgSce_)
{
	m_cfgSce = cfgSce_;
}

/*
	Detecta las BSs cuya area de cobertura alcanza al UT. Las BSs detectadas
	son incluidas en m_dsList. Si el UT ya esta conectada a una BS, esta tambien 
	puede que se incluya en m_dsList. No se afirma que necesariamente la BS actual
	se incluye en el m_dsList porque para el caso de UT_CC_EVENT la BS actual es
	detectada como fuera de cobertura.
*/
void accessSelection::getDetectedSet(userTerminal *ut_)
{
	bs_iter_t it;
	basestation *bs_;
	dataDS_t val;
	bool wifi_;

	// Limpiar m_dsList
	m_dsList.clear();

	statistics::instance()->aux1()++;
	
	wifi_ = false;
	DBG_INFO2("getDetectedSet for utID %d with bsList size %d", ut_->utID(), m_bsList.size());
	DBG_INFO("# of Base Statiosn is %d", (signed) m_bsList.size());
	for (it=m_bsList.begin(); it!=m_bsList.end(); it++) {
		bs_ = it->second;
		//DBG_INFO(".... with bsID %d", bs_->bsID());		
		if (inCoverage(ut_, bs_)) {
			if(bs_->bsTypeID() == 0) {
				wifi_ = true;
			}
			DBG_INFO("utID %d is in coverage with bsID %d [%s]",ut_->utID(),bs_->bsID(),bs_->bsTypeID() == 0 ? "GSM" : "WiFi");
			val.bs = bs_;
			val.weight = 0.0;
			val.offeredPrice = 0.0;
			m_dsList.push_back(val);
		}
	}
	if(wifi_) {
		statistics::instance()->aux2()++;
	}
	
	DBG_INFO2("Detected set size %d", m_dsList.size());

}

/*
	Para cada BS incluida en m_dsList se calcula el precio actual que ofrece
	en funcion de la carga que actualmente soporta. Para el caso UT_MONITORING_EVENT
	la capacidad del BS no debe incluir la carga del servicio que esta cursando 
	porque el precio no debe variar mientras el servicio aun esta activo.
*/
void accessSelection::getValidatedSet(userTerminal *ut, servData_t *sData)
{
	dsIter_t it;
	basestation *bs_i, *currBS;
	int servType;
	
	servType = sData->servType;

	// Se fija el precio actual que ofrece cada BS del m_dsList en funcion de la carga actual que soporta
	for (it=m_dsList.begin(); it!=m_dsList.end(); it++) {
		bs_i = (*it).bs;
		currBS = ut->findServConnected(servType);
		// if (currBS == NULL) { // Si UT con servType no esta previamente conectado a alguna BS
			// DBG_ERR("El servicio debe estar conectado a un BS");
			// exit(-1);
		// }
		(*it).offeredPrice = setOfferPrice(ut, currBS, bs_i, sData);
	}

	// Se eliminan aquellas BSs cuyo precio actual supere el precio maximo del usuario	
	DBG_INFO2("m_dsList size %d", m_dsList.size());
	for (it=m_dsList.begin(); it!=m_dsList.end(); ) {
		bs_i = (*it).bs;
		DBG_INFO2("bsID %d offerPrice %.2f currLoad %.2f utID %d maxPrice %.2f",bs_i->bsID(),(*it).offeredPrice,
				bs_i->currLoad(), ut->utID(),ut->maxPrice());
		if ((*it).offeredPrice > ut->maxPrice()) {
			DBG_INFO2("Delete bs_i %d", bs_i->bsID());
			m_dsList.erase(it);		
		}
		else {
			it++;
		}
	}

}

/**
	Al inicio de la simulacion un servicio puede tener el estado activo pero aun no haber sido conectado a ninguna BS
*/
float accessSelection::setOfferPrice(userTerminal *ut, basestation *currBS, basestation *bs_i, servData_t *sData)
{
	dsIter_t it;
	//float bsAvaiRelatLoad;
	servState_t servState;
	//float auxPrice_;
	float offerPrice_;
	
	servState = ut->getServicesState();
	//bsAvaiRelatLoad = getBSAvailableRelativeLoad(bs_i);

	if (currBS != NULL) { // Servicio ya esta conectado a una BS
		if (currBS->opTypeID() == bs_i->opTypeID()) { // BSs que pertenecen al mismo operador
			if (currBS->bsTypeID() == bs_i->bsTypeID()) { // BSs del mismo tipo
				// El usuario mantiene el mismo precio siempre y cuando el precio de la nueva BS sea mayor que la del usuario
				//auxPrice_ = bs_i->getcurrPrice(bsAvaiRelatLoad, ut->myOperator() == bs_i->opTypeID() ? true : false);
				offerPrice_ = sData->userPrice > bs_i->currPrice() ? bs_i->currPrice() : sData->userPrice; 
			}
			else { // BSs con diferente tipo
				//offerPrice_ = bs_i->getcurrPrice(bsAvaiRelatLoad, ut->myOperator() == bs_i->opTypeID() ? true : false); // sameOP = true		
				offerPrice_ = bs_i->currPrice();
				
			}
		}
		else { // BSs que pertenecen a distinto operador
			if (ut->myOperator() == bs_i->opTypeID()) {
				//offerPrice_ = bs_i->getcurrPrice(bsAvaiRelatLoad, true); // sameOP = true
				offerPrice_ = bs_i->currPrice();
				
			}
			else {
				//offerPrice_ = bs_i->getcurrPrice(bsAvaiRelatLoad, false); // sameOP = false
				offerPrice_ = bs_i->currPrice()*(1+bs_i->opPriceFactor());
			}
		}
	}
	else { // servType is IDLE
		if (ut->myOperator() == bs_i->opTypeID()) {
			//offerPrice_ = bs_i->getcurrPrice(bsAvaiRelatLoad, true); // sameOP = true
			offerPrice_ = bs_i->currPrice();
			
		}
		else {
			//offerPrice_ = bs_i->getcurrPrice(bsAvaiRelatLoad, false); // sameOP = false	
			offerPrice_ = bs_i->currPrice()*(1+bs_i->opPriceFactor());
			
		}
	}
	
	return offerPrice_;

}	
	
basestation* accessSelection::getCandidateSet(userTerminal *ut, float resources, servData_t *sData)
{
	dsIter_t it;
	basestation *bs, *result;
	float totalWeight, bsAvailableLoad, offerPrice;
	bool connected = false;
	int servType;

	servType = sData->servType;
	
	DBG_INFO2("getCandidateSet recursos solicitados %.2f", resources);
	for (it=m_dsList.begin(); it!=m_dsList.end(); it++) {
		bs = (*it).bs;
		totalWeight = m_cfgSce->qualityWeight()*LinkQualityConstraint(ut,bs) +
					  m_cfgSce->operatorWeight()*OperadorConstraint(ut,bs) +
					  m_cfgSce->loadWeight()*LoadConstraint(bs) +
					  m_cfgSce->priceWeight()*PriceConstraint(ut,bs,(*it).offeredPrice);

		DBG_INFO("[%02d] qualityWeight %.2f LinkQualityConstraint %.2f", bs->bsID(), m_cfgSce->qualityWeight(), LinkQualityConstraint(ut,bs));
		DBG_INFO("[%02d] operatorWeight %.2f OperadorConstraint %.2f", bs->bsID(), m_cfgSce->operatorWeight(), OperadorConstraint(ut,bs));
		DBG_INFO("[%02d] loadWeight %.2f LoadConstraint %.2f", bs->bsID(), m_cfgSce->loadWeight(), LoadConstraint(bs));
		DBG_INFO("[%02d] priceWeight %.2f PriceConstraint %.2f", bs->bsID(), m_cfgSce->priceWeight(), PriceConstraint(ut,bs,(*it).offeredPrice));

		if (ut->findServConnected(servType) != NULL) { // UT ya esta conectado ha alguna BS
			totalWeight = totalWeight + m_cfgSce->handoverWeight()*HandoverConstraint(ut,bs,servType);
			DBG_INFO("[%02d] handoverWeight %.2f HandoverConstraint %.2f", bs->bsID(), m_cfgSce->handoverWeight(), HandoverConstraint(ut,bs,servType));
		}
		(*it).weight = totalWeight;
	}
	
	sort(m_dsList.begin(), m_dsList.end(), compareDS);
	// for (it=m_dsList.begin(); it!=m_dsList.end(); ) {
		// it_ini = it;
		// it_next = it++;
		// if ((*it_ini).weight == (*it_next).weight) {
			
	// }

	for (it=m_dsList.begin(); it!=m_dsList.end(); it++) {
		bs = (*it).bs;
		DBG_INFO("\tbsID %d [%s] overall weight %.2f price %.2f loadAva %.2f currLoad %.2f capacity %.2f", bs->bsID(), bs->bsTypeID() == 0 ? "GSM" : "WiFi", (*it).weight, (*it).offeredPrice, getBSAvailableLoad(bs), bs->currLoad(), bs->capacity());
	}
	
	// Busqueda de una BS del DS que soporte los recursos solicitados. Puede que dentro del DS tambien se encuentra la BS actual
	for (it=m_dsList.begin(); it!=m_dsList.end(); it++) {
		bs = (*it).bs;
		offerPrice = (*it).offeredPrice;
		bsAvailableLoad = getBSAvailableLoad(bs);
		// Detectar si ya esta conectado a una BS
		if (ut->findServConnected(servType) != NULL) { // Si UT con servType esta previamente conectado a alguna BS
			if (ut->findServConnected(servType)->bsID() == bs->bsID()) { // Si el id de dicha BS es igual al actual id del m_dsList
				DBG_WARN("utID %d with servType %d connected to same previous bsID %d", ut->utID(), servType, bs->bsID());
				connected = true;
				break;
			}
		}

		DBG_INFO("\tbsID %d peso %.2f loadAva %.2f currLoad %.2f capacity %.2f", bs->bsID(), (*it).weight, bsAvailableLoad, bs->currLoad(), bs->capacity());
		if (bsAvailableLoad >= resources) {
			connected = true;
			break;			
		}
	}

	if (connected) {
		result = bs;
		sData->userPrice = offerPrice;
	}
	else {
		result = NULL;
	}
	
	return result;
}

void accessSelection::assignLoadBS(basestation *bs, float resources, servData_t *sData)
{
	float bsAvailableLoad;
	
	DBG_INFO("assignLoadBS: bsID %d currLoad %.2f capacity %.2f", bs->bsID(), bs->currLoad(), bs->capacity());
	bsAvailableLoad = getBSAvailableLoad(bs);
	if (bsAvailableLoad < resources) {
		DBG_ERR("bsID %d bsAvailableLoad %.2f cannot be less than resources %.2f", bs->bsID(), bsAvailableLoad, resources);
		exit(-1);
	}
	bs->currLoad() = bs->currLoad() + resources;
	// Guarda el precio asignado al usuario por la BS
	//sData->userPrice = bs->currPrice();
	bs->currPrice() = bs->getcurrPrice(getBSAvailableRelativeLoad(bs));
	statistics::instance()->statsInsert_price(sData->callID, bs->currPrice());
	DBG_INFO("\tafter assign currLoad %.2f", bs->currLoad());
}

float accessSelection::getBSAvailableLoad(basestation *bs)
{
	float bsAvailableLoad;
	
	bsAvailableLoad = bs->capacity() - bs->currLoad();
	if (bsAvailableLoad < 0) {
		DBG_ERR("bsID %d bsAvailableLoad %.2f cannot be negative", bs->bsID(), bsAvailableLoad);
		exit(-1);
	}

	return bsAvailableLoad;
}

float accessSelection::getBSAvailableRelativeLoad(basestation *bs)
{

	if (bs->capacity() == 0) {
		DBG_ERR("Capacity of bsID %d cannot be zero", bs->bsID());
		exit(-1);
	}
	
	return  getBSAvailableLoad(bs)/bs->capacity();
}

void accessSelection::assignResourcesToBS(userTerminal *ut, basestation *bs, float resources, servData_t *sData, unsigned char code)
{
	int servType;
	basestation *prevBS;
	int pmSetup, lqSetup;
	double currTime;
	
	servType = sData->servType;
	pmSetup = m_cfgSce->pmSetup();
	lqSetup = m_cfgSce->lqSetup();
	
	if (bs != NULL) { // There is Active Set (AS)
		prevBS = ut->findServConnected(servType);
		if (prevBS != NULL) { // service already connected
			if (prevBS->bsID() != bs->bsID()) { // previous BS != Active Set => HO call!!
				//DBG_WARN("HandOver callID %d", sData->callID);			
				releaseResourcesOfBS(ut, resources, sData, true);
				ut->cleanCCevent(sData, code);
				// Asignar recursos solicitados en la BS
				assignLoadBS(bs, resources, sData);
				// connect UT with servType to BS
				ut->connectToBS(servType,bs);				
				DBG_INFO2("Detecting CC event for new service");
				ut->addCCevent(sData, bs);				
				statistics::instance()->statsInsert_currBS(sData->callID, bs->bsID());
				if (bs->opTypeID() == ut->myOperator()) {
					statistics::instance()->statsInsert_prefOP(sData->callID, true);
				}
				else {
					statistics::instance()->statsInsert_prefOP(sData->callID, false);
				}
				//DBG_WARN("HO utID %d with servType %d connected to bsID %d", ut->utID(), servType, bs->bsID());
				// Update statistics HO
				currTime = eventQueue::instance()->currentTime();
				statistics::instance()->statsInsert_tHOs(sData->callID, currTime);
			}
			else { // previous BS = Active Set => Drop call!!
				if (code == UT_CC_EVENT) {
					DBG_WARN("CC event Drop call for utID %d servType %d bsID %d callID %d", ut->utID(), servType, bs->bsID(), sData->callID);
					releaseResourcesOfBS(ut, resources, sData);
					// Eliminar fin de servicio. Inicio de servicio es eliminado por el gestor de eventos.
					eventQueue::instance()->deleteEvent(sData->endEvID);								
					ut->cleanServState(servType);
					// Update statistics dropCall
					statistics::instance()->statsInsert_result(sData->callID, DROP_CALL);
					statistics::instance()->statsInsert_tDrop(sData->callID, eventQueue::instance()->currentTime());
					if (pmSetup == MONITORING_ON) {
						DBG_INFO2("clean PM events");
						ut->cleanPMevents(sData);
					}
					if (lqSetup == LINKQUALITY_ON) {
						DBG_INFO2("clean LQ events");
						ut->cleanLQevents(sData);
					}
					ut->cleanCCevent(sData, code);
				}
			}
		}
		else { // new service
			//DBG_WARN("New call callID %d", sData->callID);
			// Asignar recursos solicitados en la BS
			assignLoadBS(bs, resources, sData);
			// connect UT with servType to BS
			ut->connectToBS(servType,bs);
			//DBG_WARN("newServ utID %d with servType %d connected to bsID %d", ut->utID(), servType, bs->bsID());
			statistics::instance()->statsInsert_currBS(sData->callID, bs->bsID());
			if (bs->opTypeID() == ut->myOperator()) {
				statistics::instance()->statsInsert_prefOP(sData->callID, true);
			}
			else {
				statistics::instance()->statsInsert_prefOP(sData->callID, false);
			}
			// Update statistics newCall
			if (pmSetup == MONITORING_ON) {
				DBG_INFO2("set PM events");
				ut->addPMevents(sData);
			}
			if (lqSetup == LINKQUALITY_ON) {
				DBG_INFO2("set LQ events");
				ut->addLQevents(sData);
			}
			DBG_INFO2("Detecting CC event for new service");
			ut->addCCevent(sData, bs);
		}
	}
	else { // There isn't Active Set (AS) 
		if (ut->findServConnected(servType) != NULL) { // service already connected => Drop call
			DBG_WARN("No AS. Drop call for utID %d servType %d callID %d", ut->utID(), servType, sData->callID);
			releaseResourcesOfBS(ut, resources, sData);
			// Update statistics dropCall
			statistics::instance()->statsInsert_result(sData->callID, DROP_CALL);
			statistics::instance()->statsInsert_tDrop(sData->callID, eventQueue::instance()->currentTime());
			if (pmSetup == MONITORING_ON) {
				DBG_INFO2("clean PM events");
				ut->cleanPMevents(sData);
			}
			if (lqSetup == LINKQUALITY_ON) {
				DBG_INFO2("clean LQ events");
				ut->cleanLQevents(sData);
			}
			ut->cleanCCevent(sData, code);
		}
		else { // service not connected yet => Reject call
			//DBG_WARN("Reject call for utID %d servType %d state %d callID %d", ut->utID(), servType, sData->state, sData->callID);
			// Update statistics rejectCall
			statistics::instance()->statsInsert_result(sData->callID, REJECT_CALL);
		}
		// Eliminar fin de servicio. Inicio de servicio es eliminado por el gestor de eventos.
		DBG_INFO2("Deleting event %lu from accessselection",sData->endEvID);
		eventQueue::instance()->deleteEvent(sData->endEvID);			
		ut->cleanServState(servType);
	}

	//DBG_INFO("Fin AccessSelecction for callID %d", sData->callID);
}

void accessSelection::releaseResourcesOfBS(userTerminal *ut, float resources, servData_t *sData, bool fromHO_)
{
	basestation *bs;
	int servType;
	
	servType = sData->servType;
	
	//DBG_INFO("releaseResourcesToBS for utID %d with servType %d", ut->utID(), servType);
	bs = ut->findServConnected(servType);
	if (bs != NULL) {
		ut->disconnectFromBS(servType);
		bs->currLoad() = bs->currLoad() - resources;
		bs->currPrice() = bs->getcurrPrice(getBSAvailableRelativeLoad(bs));
		if(!fromHO_) {
			ut->cleanServState(servType);		
		}
		DBG_INFO2("Release %.2f resources to bsID %d currLoad %.2f",resources,bs->bsID(), bs->currLoad());		
	}
	else {
		DBG_ERR("Servicio %d no se encuentra en m_currBSconn", servType);
		exit(-1);	
	}

}

double accessSelection::LinkQualityConstraint(userTerminal *ut, basestation *bs)
{
	double distance, bsRadio, currTime;
	geometry gm;
	
	currTime = eventQueue::instance()->currentTime();
	//DBG_INFO("LinkQualityConstraint: currTime %f", currTime);
	distance = gm.distancePoints(ut->getcurrPosition(currTime),bs->getPosition());
	bsRadio = bs->range();
	if (bsRadio == 0) {
		DBG_ERR("Radio of bsID %d no deberia ser 0", bs->bsID());
		exit(-1);
	}
	
	return (1-distance/bsRadio);
}

float accessSelection::HandoverConstraint(userTerminal *ut, basestation *bs, int servType)
{
	float value;
	
	if (ut->findServConnected(servType)->bsID() != bs->bsID()) {
		value = 0;
	}
	else { // utID = bsID
		value = -1.0*log10(1.0-m_cfgSce->hoDiscount());;
	}

	return value;
}

float accessSelection::OperadorConstraint(userTerminal *ut, basestation *bs)
{
	float value;
	
	if (bs->opTypeID() != ut->myOperator()) {
		value = 0;
	}
	else { // bs->opTypeID() == ut->myOperator()
		value = 1;
	}

	return value;
}

float accessSelection::LoadConstraint(basestation *bs)
{
	float result;

	if (bs->capacity() == 0) {
		DBG_ERR("Capacidad de bsID %d no deberia ser 0", bs->bsID());
		exit(-1);
	}
		
	if (bs->currLoad() <= bs->capacity()) {
		result = 1-pow((bs->currLoad()/bs->capacity()),2);
	}
	else {
		DBG_ERR("BS current load %.2f should not be greater than capacity %.2f", 
		bs->currLoad(), bs->capacity());
		exit(-1);
	}

	return result;

}

/*
float accessSelection::PriceConstraint(userTerminal *ut, basestation *bs, float offerPrice)
{
	float result;
	
	if (offerPrice <= ut->maxPrice()) {
		if ((offerPrice >= 0) && (offerPrice <= bs->bsPriceFactor()*bs->maxPrice())) {
			result = 1;
		} else if ((offerPrice > bs->bsPriceFactor()*bs->maxPrice()) && (offerPrice <= 1)) {
			result = -1.0*log10(offerPrice);
		} else {
			DBG_ERR("BS current price %.2f is out of range [0,1]", offerPrice);
			exit(-1);		
		}
	}
	else {
		DBG_ERR("bsID %d current price %.2f should no be greater than maximun user price %.2f",
		bs->bsID(), offerPrice, ut->maxPrice());
		exit(-1);
	}
	
	// if(std::isnan(result)) {
		// DBG_ERR("IsNAN pr");
		// exit(-1);
		// result = 1.0;
	// }
	
	return result;
}
*/

float accessSelection::PriceConstraint(userTerminal *ut, basestation *bs, float offerPrice)
{
	float result;
	
	// DBG_INFO("offerPrice %f bsPriceFactor %f bsmaxPrice %f utmaxPrice %f",
			// offerPrice,bs->bsPriceFactor(),bs->maxPrice(),ut->maxPrice());
	// DBG_INFO("First condition %d",(offerPrice >= (bs->bsPriceFactor()*bs->maxPrice())));
	// DBG_INFO("Difference first condition %f",(offerPrice - (bs->bsPriceFactor()*bs->maxPrice()))*1e6);
	// DBG_INFO("Difference first condition truncate %f",(offerPrice - truncate(bs->bsPriceFactor()*bs->maxPrice(),4))*1e6);
	// DBG_INFO("Second condition %d",(offerPrice <= ut->maxPrice()));
	// DBG_INFO("truncate %f",truncate(bs->bsPriceFactor()*bs->maxPrice(),4)*1e6);
	// valor = truncate(bs->bsPriceFactor()*bs->maxPrice(),4);
	//if ((offerPrice >= truncate(bs->bsPriceFactor()*bs->maxPrice(),4)) && (offerPrice <= ut->maxPrice())) {
	
	if ( FLOAT_EQ(offerPrice,bs->bsPriceFactor()*bs->maxPrice()) || FLOAT_EQ(offerPrice,ut->maxPrice()) ||
		((offerPrice > bs->bsPriceFactor()*bs->maxPrice()) && (offerPrice <= ut->maxPrice())) ) {
		result = -1.0*log10(offerPrice);
	} else {
		DBG_ERR("BS current price %.2f is out of range", offerPrice);
		exit(-1);
	}
	
	// if(std::isnan(result)) {
		// DBG_ERR("IsNAN pr");
		// exit(-1);
		// result = 1.0;
	// }
	
	return result;
}

bool accessSelection::inCoverage(userTerminal *ut, basestation *bs)
{
	bool result = false;
	double currTime;
	geometry gm;
	float distance;
	
	DBG_IN("accessSelection::inCoverage");
	
	currTime = eventQueue::instance()->currentTime();
	//DBG_INFO("inCoverage: currTime %f", currTime);
	// Nota: m_cfgSce->compareRATids() puede ser reemplazado guardando los RATids del UT y el RATid del BS en sus respectivas clases
	//DBG_INFO("....Coverage ut_type %d bs_type %d", ut->termTypeID(),bs->bsTypeID());
	if (m_cfgSce->compareRATids(ut->termTypeID(),bs->bsTypeID())) {
		distance = gm.distancePoints(ut->getcurrPosition(currTime),bs->getPosition());
		DBG_INFO("Current distance is %.3f; bs coverage is %.3f",distance,bs->range());
		if (truncate(distance,3) < bs->range()) {
			result = true;
			DBG_INFO("TRUE");
		}
	}
	
	DBG_OUT("accessSelection::inCoverage");
	return result;
}

float accessSelection::truncate(float f, unsigned int n) 
{ 
	float d = pow(10,n); 
	return ((long)(d * f)) / d; 
}

bool accessSelection::IsSnapshotSetup(void)
{
	return m_cfgSce->snapshotSetup();
}