#include "scenario.h"
#include <iterator>

using namespace std;

#define DBG_LEVEL 2
#include "debug.h"

scenario::scenario(void)
{
	DBG_IN("scenario::scenario");

	m_cfgSce = new configScenario;
	//m_accSel = new accessSelection(m_bsList);
	m_accSel = new accessSelection;
	m_ssCounter = 0;
	// m_accSel->getRATtypeList(*m_cfgSce);
	DBG_OUT("scenario::scenario");
}

scenario::~scenario(void)
{
	ut_iter_t utIter;
	bs_iter_t bsIter;
	DBG_IN("scenario::~scenario");

	delete(m_cfgSce);
	rng::instance()->endInstance();
	
	for (utIter = m_utList.begin(); utIter!=m_utList.end(); utIter++) {
		delete((*utIter).second);
	}
	m_utList.clear();

	for (bsIter = m_bsList.begin(); bsIter!=m_bsList.end(); bsIter++) {
		delete((*bsIter).second);
	}
	m_bsList.clear();
	
	delete(m_accSel);
	eventQueue::instance()->endInstance();
	
	statistics::instance()->endInstance();
	
	DBG_OUT("scenario::~scenario");
}

void scenario::onEvent(struct eventMessage *message)
{

	DBG_IN("scenario::onEvent");

	switch(message->code) {
		case SNAPSHOT_EVENT:
			//DBG_INFO2("EVENT snapshot at time %.2f",eventQueue::instance()->currentTime());
			makeSnapshots();
			break;
		case BS_LOADMONITORING_EVENT:
			//bsLoadMonitoring();
			break;
		default:
			DBG_ERR("Unrecognized code %d",message->code);
			break;
	}

	DBG_OUT("scenario::onEvent");	
}

void scenario::bsLoadMonitoring(void)
{
	bs_iter_t bsIter_;
	basestation *bs;
	
	for(bsIter_ = m_bsList.begin(); bsIter_ != m_bsList.end(); bsIter_++) {
		bs = bsIter_->second;
		statistics::instance()->statsInsert_bsLoad(bs->bsID(), bs->currLoad());
	}
	
}

void scenario::makeSnapshots(void)
{
	point_t po;
	char filenameSuffix[4];
	string currSnapshotsFilename;
	servState_t servState;
	servState_iter_t its;
	string UTSnapshotsFilename;
	FILE *UTSnapshotsFile;
	// string srvPhotoFilename;
	// FILE *srvPhotoFile;
	
	//UTSnapshotsFilename = "results/snapshots/" + m_cfgSce->ssfilePrefix() + "_UT_";
	UTSnapshotsFilename = "results/snapshots/snapshot_";
	//DBG_INFO2("Snapshot file = %s", UTSnapshotsFilename.c_str());
	// Generar nombre de fichero snapshot añadiendo m_ssCounter
	sprintf(filenameSuffix,"%04ld",++m_ssCounter);
	currSnapshotsFilename = UTSnapshotsFilename + string(filenameSuffix) + ".txt";
	//DBG_INFO2("Generating file snapshots ...");
	//DBG_INFO2("utID m_x m_y termTypeID userTypeID myOp serv");
	//DBG_INFO2("Generating file snapshots %s", currSnapshotsFile.c_str());

	UTSnapshotsFile = fopen(currSnapshotsFilename.c_str(), "w");

	// Temporal. Solo para comprobar que cada snapshot cae dentro de la linea inicio-fin de servicio
	// srvPhotoFilename = "srvPhoto_" + string(filenameSuffix) + ".txt";
	// srvPhotoFile = fopen(srvPhotoFilename.c_str(), "w");
		
	if (UTSnapshotsFile == NULL)
		DBG_ERR("Error abriendo file snapshot");

	for (int i=0; i < m_cfgSce->numOfUsers(); i++) {
		if ((m_utList[i]->getMovModel()) != MOVNONE) {
			//po = m_utList[i]->getcurrPosition(m_currStep);
			po = m_utList[i]->getcurrPosition(eventQueue::instance()->currentTime());
		}
		else {
			po = m_utList[i]->getIniPosition();
		}
		fprintf(UTSnapshotsFile, "%04d %07.2f %07.2f %02d %02d %02d", 
				m_utList[i]->utID(), po.m_x, po.m_y, m_utList[i]->termTypeID(),
				m_utList[i]->userTypeID(), m_utList[i]->myOperator());
		//DBG_INFO2("%04d %07.2f %07.2f", m_utList[i]->utID(), po.m_x, po.m_y);

		servState = m_utList[i]->getServicesState();
		for (int j=0; j< m_cfgSce->getnumOfServices(); j++) {
			its = servState.find(j);
			if (its!=servState.end()) {
				fprintf(UTSnapshotsFile, " %d", (*its).second.state ? 1 : 0);
				// Temporal. Imprime TipoServicio - tiempo en el que se toma el snapshot 
				// if (its->second) { 
					// fprintf(srvPhotoFile,"%2d %.2f\n", its->first, eventQueue::instance()->currentTime());
				// }
			}
			else {
				fprintf(UTSnapshotsFile, " %d", 0);
			}
		}
		fprintf(UTSnapshotsFile, "\n");
	}
	fclose(UTSnapshotsFile);
	// Relacionado con la seccion temporal
	// fclose(srvPhotoFile);

}

int scenario::makeConfiguration(configFile configFile_)
{
	int result;
	
	result = m_cfgSce->Configure(configFile_);
	return result;

}

void scenario::movementGenerator(void)
{
	DBG_INFO2("Generating movements for %d UTs", m_cfgSce->numOfUsers());
	for (int i=0; i < m_cfgSce->numOfUsers(); i++) {
		if ((m_utList[i]->getMovModel()) != MOVNONE) {
			//DBG_INFO2("utID=%d with model=%d", m_utList[i]->utID(), m_utList[i]->getMovModel());
			m_utList[i]->makeMovements(m_cfgSce->simulTime());
		}
		else {
			m_utList[i]->setUTmoveNone();
			DBG_INFO2("utID=%d with model=%d have been skipped", m_utList[i]->utID(), m_utList[i]->getMovModel());
		}
	}
}

void scenario::CreateMovementTraceFile(void)
{
	string movTraceFilename;
	FILE *movTraceFile;

	DBG_INFO2("Printing movements trace for %d UTs", m_cfgSce->numOfUsers());
	movTraceFilename = "results/movements.trc";
	movTraceFile = fopen(movTraceFilename.c_str(), "w");
	if (movTraceFile == NULL) {
		DBG_ERR("Error opening movement trace file");
	}
	
	for (int i=0; i < m_cfgSce->numOfUsers(); i++) {
		if ((m_utList[i]->getMovModel()) != MOVNONE) {
			//DBG_INFO2("utID=%d with model=%d", m_utList[i]->utID(), m_utList[i]->getMovModel());
			m_utList[i]->printMovementsToFile(movTraceFile);
		}
		else {
			DBG_INFO2("utID=%d with model=%d have been skipped", m_utList[i]->utID(), m_utList[i]->getMovModel());
		}
	}
	
	fclose(movTraceFile);	

}

void scenario::ReadMovementTraceFile(void)
{
	string movTraceFilename;
	FILE *movTraceFile;
	movData_t *mov_;
	int numParams;
	bool endLoop = false;
	
	movTraceFilename = "results/movements.trc";
	movTraceFile = fopen(movTraceFilename.c_str(), "r");
	if (movTraceFile == NULL) {
		DBG_ERR("Error opening movement trace file");
	}
	
	DBG_INFO2("Reading movements trace for %d UTs", m_cfgSce->numOfUsers());
	do {
		mov_ = new movData_t;
		numParams = fscanf(movTraceFile, "%d %d %lf %lf %lf %lf %lf %lf %lf", &(mov_->utID),
			&(mov_->movID), &(mov_->tStart), &(mov_->iniPos.m_x), &(mov_->iniPos.m_y),
			&(mov_->finPos.m_x), &(mov_->finPos.m_y), &(mov_->direction), &(mov_->speed));
		// DBG_INFO2("fpos %4d UTid %4d movID %4d",fpos, mov_->utID, mov_->movID);
		// if(m_utList.find(mov_->utID) == m_utList.end()) {
			// DBG_ERR("No end user stored %03d",mov_->utID);
		// }
		if (numParams == 9) {
			m_utList[mov_->utID]->insertMovement(mov_);
		}
		else {
			delete mov_;
			endLoop = true;
		}
	} while (!endLoop);
	
	fclose(movTraceFile);
}

void scenario::serviceGenerator(void)
{
	DBG_INFO2("Generating services for %d UTs", m_cfgSce->numOfUsers());
	for (int i=0; i < m_cfgSce->numOfUsers(); i++) {
		m_utList[i]->makeServices(m_cfgSce->simulTime());
	}
}

void scenario::CreateServiceTraceFile(void)
{
	string servTraceFilename;
	FILE *servTraceFile;

	DBG_INFO2("Printing services trace for %d UTs", m_cfgSce->numOfUsers());
	servTraceFilename = "results/services.trc";
	servTraceFile = fopen(servTraceFilename.c_str(), "a");
	if (servTraceFile == NULL) {
		DBG_ERR("Error opening service trace file");
	}
	
	for (int i=0; i < m_cfgSce->numOfUsers(); i++) {
		m_utList[i]->printServicesToFile(servTraceFile);
	}

	fclose(servTraceFile);	
}

void scenario::ReadServiceTraceFile(void)
{
	string servTraceFilename;
	FILE *servTraceFile;
	servData_t *serv_, *sData, *sDataPrev;
	int utID_;
	int numParams;
	bool endLoop = false;
	unsigned long int evID;
	bool initialization=true;
	int currServ;
	
	servTraceFilename = "results/services.trc";
	servTraceFile = fopen(servTraceFilename.c_str(), "r");
	if (servTraceFile == NULL) {
		DBG_ERR("Error opening service trace file");
	}
	
	DBG_INFO2("Reading services trace for %d UTs", m_cfgSce->numOfUsers());
	do {
		serv_ = new servData_t;
		numParams = fscanf(servTraceFile, "%d %d %d %lf %d", &utID_, &(serv_->callID),
			&(serv_->servType), &(serv_->tWhen), &(serv_->state));
		if (numParams == 5) {
			// DBG_INFO2("%4d %4d %4d %8.2f %4d", utID_, serv_->callID,
				// serv_->servType, serv_->tWhen, serv_->state);		
			evID = m_utList[utID_]->insertService(serv_);
			/* Proceso para colocar el ID del evento actual (evID) al registro servData previo. Asi cada registro de un servicio activo (1) 
			   tendra un campo que indique el ID del evento final correspondiente, para eliminarlo en caso que el servicio sea rechazado */
			sData = serv_;
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
			//DBG_INFO2(">s callID %d servType %d tWhen %.2f state %d endEvID %d", sDataPrev->callID, sDataPrev->servType, sDataPrev->tWhen, sDataPrev->state, sDataPrev->endEvID);
			sDataPrev = sData;			
		}
		else {
			delete serv_;
			endLoop = true;
		}		
	} while (!endLoop);
	
	fclose(servTraceFile);
}

void scenario::makeUTdeployment(void)
{
	int i;
	userTerminal *ut_;
	
	DBG_INFO2("Deploying UTs");	
	for (i=0; i < m_cfgSce->numOfUsers(); i++) {
		ut_ = new userTerminal(m_accSel);
		configureUT(i,ut_);
		m_utList.insert(pair<int,userTerminal*>(i,ut_));
	}
	
}

void scenario::makeBSdeployment(void)
{
	int i,j;
	operatorType_iter_t opIter;
	vector<int> numOfBSlist;
	vector<float> prList;
	vector<float>::iterator prIter;
	operatorType_t opList;
	operatorType *operator_;
	
	DBG_INFO2("Deploying BSs");
	//DBG_INFO2("BSs no feasible deploy");
	//DBG_INFO2("OPtypeID BStypeID posX posy");
	opList = m_cfgSce->getOPtypeList(); // map<int,operatorType *>
	
	for (opIter=opList.begin(); opIter!=opList.end(); opIter++) { // recorre el mapa de tipos de operadores
		numOfBSlist = ((*opIter).second)->getnumOfBS(); // vector<int>
		operator_ = (*opIter).second; // operatorType *
		//DBG_INFO2("For operator type %d", operator_->index());
		for (i=0; i < (int) numOfBSlist.size(); i++) { // recorre el vector de tipos de BS(i) que contienen el numero de BSs
			//minDistance_ = m_cfgSce->getBSminDistance(i);
			//DBG_INFO2("  for BS type %d with minDistance %.2f", i, minDistance_);
			for (j=0; j < numOfBSlist[i]; j++) { // Para un mismo tipo de BS(i) recorre todas las BS.
				// i = id del tipo de BS
				// j = contador del numero de BSs de un mismo tipo
				// operator_ = operador actual
				configureBS(i, j, operator_);
			}
			//DBG_INFO2("Total for operator %d BS %d = %d bs.size=%d",operator_->index(), i, j, m_bsList.size());
		}
	}
	
}

void scenario::configureUT(int id_, userTerminal *ut_)
{
	movementType *movType_;
	
	DBG_IN("scenario::configureUT");

	ut_->utID() = id_;
	ut_->myOperator() = setOperatorID();
	// Importante! es necesario ejecutar setOperatorID antes de setTerminalTypeID
	ut_->termTypeID() = setTerminalTypeID(ut_);
	ut_->userTypeID() = setUserTypeID();
	movType_ = setMovementType();
	ut_->setMovementParams(movType_->model(), movType_->params());
	ut_->setAreaLimits(m_cfgSce->xMax(),m_cfgSce->yMax());
	ut_->pmStep() = m_cfgSce->pmStep();
	ut_->lqStep() = m_cfgSce->lqStep();
	ut_->maxPrice() = m_cfgSce->getUserPrice(ut_->userTypeID());
	
	ut_->setServiceParams(m_cfgSce->getuserTypeList(), m_cfgSce->getservTypeList());
	ut_->initialConfiguration();
	
	DBG_OUT("scenario::configureUT");
}

/*
	Para cada operador n-esimo, i representa la BS con id igual a i,
	y j representa la j-esima posicion de dicha BS del grupo de BS_i asignada a dicho operador.
	En el mCASE.cfg se corresponde con la siguiente seccion:
	[OPERATOR0]
	BS0=10
	BS1=0
	BS2=0
	En este caso i seria 0,1 y 2, mientras que, por ejemplo, para i=0 j tomaria valores entre 0 y 10.
*/
void scenario::configureBS(int i, int j, operatorType *operator_)
{
	basestation *bs_;
	basestation *currentBS_;
	bool safe_;
	bs_iter_t bsIter_;
	double currentDistance_;
	int try_=0;
	static int k=0;
	static int bsNum=0;
	point_t posBSnew;
	geometry gm;
	const int max_try=250;
	
	DBG_IN("scenario::configureBS");
	
	do {
		safe_ = true;
		posBSnew.m_x = rng::instance()->uniform(m_cfgSce->xMax());
		posBSnew.m_y = rng::instance()->uniform(m_cfgSce->yMax());
		//DBG_INFO2("try=%d",try_);
		for(bsIter_ = m_bsList.begin() ; bsIter_ != m_bsList.end() ; bsIter_++) { // BSs ya registradas
			currentBS_ = bsIter_->second;
			if(currentBS_->opTypeID() != operator_->index()) {
				continue;
			}
			if(currentBS_->bsTypeID() != i) {
				continue;
			}
			currentDistance_ = gm.distancePoints(posBSnew, currentBS_->getPosition());
			//DBG_INFO2("currX=%.2f currY=%.2f bsX=%.2f bsY=%.2f dist=%.2f",currentX_, currentY_, currentBS_->posX(), currentBS_->posY(), currentDistance_);
			if(currentDistance_ < m_cfgSce->getBSminDistance(i)) {
				safe_  = false;
				//DBG_INFO2("BREAK");
				break;
			}
		}
		try_++;
	} while ((!safe_) && (try_ < max_try));

	if (try_ >= max_try) {
		DBG_WARN("It wasn't feasible deploy BS %d of type %d from operator type %d tries %d", j, i, operator_->index(), try_);
		//DBG_INFO2("%02d %02d %7.2f %7.2f", operator_->index(), i, currentX_, currentY_);
	}
	
	if(safe_) {
		bs_ = new basestation;
		bs_->bsID() = bsNum++;
		bs_->bsTypeID() = i;
		bs_->opTypeID() = operator_->index();
		bs_->posX() = posBSnew.m_x;
		bs_->posY() = posBSnew.m_y;
		// Los siguientes parametros deben repetirse en ReadBSdeployFile()
		bs_->capacity() = m_cfgSce->getBScapacity(bs_->bsTypeID());
		bs_->range() = m_cfgSce->getBSrange(bs_->bsTypeID());
		priceParameters(bs_);
		// NOTA: Llamar a currPrice despues de priceParameters
		bs_->currPrice() = bs_->getcurrPrice(1);
		m_bsList.insert(pair<int,basestation*>(k++,bs_));
		//DBG_INFO2("    New BS %d BS id=%d OP id=%d bs.size=%d",bsNum-1,i,bs_->opTypeID(), m_bsList.size());	
	}
	
	DBG_OUT("scenario::configureBS");
}

void scenario::priceParameters(basestation *bs)
{
	// operatorType_iter_t opIter;
	// operatorType_t opList;
	// vector<float> prList;
	// vector<float>::iterator prIter;
	// float m, n;
	//float maxPrice = 0;
	
	// Calculo de parametros de la funcion lineal Precio(carga_libre). m pendiente, n interseccion con eje de ordenadas
	bs->maxPrice() = m_cfgSce->getBSprice(bs->bsTypeID(), bs->opTypeID());

	bs->bsPriceFactor() = m_cfgSce->getbsPriceFactor(bs->opTypeID());
	bs->opPriceFactor() = m_cfgSce->getopPriceFactor(bs->opTypeID());
	bs->lowerThrLoad() = m_cfgSce->lowerThrLoad();
	bs->upperThrLoad() = m_cfgSce->upperThrLoad();
	
	// m = ((1-bs->bsPriceFactor())*bs->price())/((bs->lowerThrLoad()-bs->upperThrLoad())*bs->capacity());
	// n = bs->price()*(1-((1-bs->bsPriceFactor())*bs->lowerThrLoad()/(bs->lowerThrLoad()-bs->upperThrLoad())));
	// bs->pend() = m;
	// bs->inter() = n;
	
	// Calculo del precio maximo de entre todos los operadores
	/** 
	opList = m_cfgSce->getOPtypeList(); // map<int,operatorType *>
	for (opIter=opList.begin(); opIter!=opList.end(); opIter++) {
		prList = ((*opIter).second)->getpriceOfBS(); // vector<float>
		for (prIter=prList.begin(); prIter!=prList.end(); prIter++) {
			if (*prIter > maxPrice) maxPrice = *prIter;
		}
	}
	bs->maxPrice() = maxPrice;
	*/
	//DBG_INFO2("Maximun price of BSs %.2f", bs->maxPrice());
	
}

void scenario::CreateBSdeployFile(void)
{
	bs_iter_t it;
	string BSdeployFilename;
	FILE *BSdeployFile;

	BSdeployFilename = "results/BSs_list.txt";
	BSdeployFile = fopen(BSdeployFilename.c_str(), "w");
	if (BSdeployFile == NULL) {
		DBG_ERR("Error opening file BS list for writing");
	}
	DBG_INFO2("Generating BSs file list %s", BSdeployFilename.c_str());
	for (it=m_bsList.begin(); it!=m_bsList.end(); it++) {
		fprintf(BSdeployFile, "%04d %07.2f %07.2f %02d %02d\n",
		(*it).second->bsID(), (*it).second->posX(), (*it).second->posY(),
		(*it).second->bsTypeID(), (*it).second->opTypeID());
	}
	fclose(BSdeployFile);
}

void scenario::ReadBSdeployFile(string BSdeployFilename)
{
	bs_iter_t it;
	//string BSdeployFilename;
	FILE *BSdeployFile;
	basestation *bs_;
	int k=0;
	int numParams;
	bool endLoop = false;	

	//BSdeployFilename = "results/BSs_list.txt";
	BSdeployFile = fopen(BSdeployFilename.c_str(), "r");
	if (BSdeployFile == NULL) {
		DBG_ERR("Error opening file BS list for reading %s", BSdeployFilename.c_str());
		exit(-1);
	}
	DBG_INFO2("Reading BSs file list %s", BSdeployFilename.c_str());
	do {
		bs_ = new basestation;
		numParams = fscanf(BSdeployFile, "%d %lf %lf %d %d",
		&(bs_->bsID()), &(bs_->posX()), &(bs_->posY()),
		&(bs_->bsTypeID()), &(bs_->opTypeID()));
		if (numParams == 5) {
			bs_->capacity() = m_cfgSce->getBScapacity(bs_->bsTypeID());
			bs_->range() = m_cfgSce->getBSrange(bs_->bsTypeID());
			priceParameters(bs_);
			// NOTA: Llamar a currPrice despues de priceParameters
			bs_->currPrice() = bs_->getcurrPrice(1);
			m_bsList.insert(pair<int,basestation*>(k++,bs_));
		}
		else {
			delete bs_;
			endLoop = true;
		}		
	} while (!endLoop);
	
	fclose(BSdeployFile);
}

/*
	Crea el archivo que contiene los parametros de despliegue de las BS para el caso
	de Snapshot = ON
*/
void scenario::CreateSS_BSdeployFile(void)
{
	bs_iter_t it;
	string BSdeployFilename;
	FILE *BSdeployFile;

	BSdeployFilename = "results/snapshots/" + m_cfgSce->ssfilePrefix() + "_BS.txt";
	BSdeployFile = fopen(BSdeployFilename.c_str(), "w");
	if (BSdeployFile == NULL) {
		DBG_ERR("Error opening file BS deployment");
	}
	DBG_INFO2("Generating BSs file %s", BSdeployFilename.c_str());
	for (it=m_bsList.begin(); it!=m_bsList.end(); it++) {
		fprintf(BSdeployFile, "%04d %07.2f %07.2f %02d %02d\n",
		(*it).second->bsID(), (*it).second->posX(), (*it).second->posY(),
		(*it).second->bsTypeID(), (*it).second->opTypeID());
	}
	fclose(BSdeployFile);
}

/*
   Aunque posteriormente en ReadUTdeployFile() no se necesite la posicion iniial del UT
   (pos.m_x, pos.m_y), esta funcion lo imprime por si se necesita dicha info para dibujar
   el despliegue inicial de los UTs.
*/
void scenario::CreateUTdeployFile(void)
{
	ut_iter_t it;
	string UTdeployFilename;
	FILE *UTdeployFile;
	userTerminal *ut_;
	point_t pos;
	
	UTdeployFilename = "results/UTs_list.txt";
	UTdeployFile = fopen(UTdeployFilename.c_str(), "w");
	if (UTdeployFile == NULL) {
		DBG_ERR("Error opening file UT list");
	}
	DBG_INFO2("Generating UTs list file %s", UTdeployFilename.c_str());
	for (it=m_utList.begin(); it!=m_utList.end(); it++) {
		ut_ = (*it).second;
		pos = ut_->getIniPosition();
		fprintf(UTdeployFile, "%04d %07.2f %07.2f %02d %02d %02d %2d\n",
		ut_->utID(), pos.m_x, pos.m_y,	ut_->termTypeID(),
		ut_->userTypeID(), ut_->myOperator(), ut_->getMovModel());
	}
	fclose(UTdeployFile);
}

/* 
   WARNING! Durante la creacion de UTs en makeDeployment() se definen varios parametros 
   relacionados con la creacion de movimientos y servicios (setMovementParams(), setAreaLimits(),
   setServiceParams(), initialConfiguration()) que no son necesarios volverlos a crear porque
   las trazas ya estan generadas. Uno de ellos es la posicion inicial del UT (pos.m_x, pos.m_y),
   el cual esta almacenada indirectamente en m_movReg.
*/
void scenario::ReadUTdeployFile(void)
{
	string UTdeployFilename;
	FILE *UTdeployFile;
	userTerminal *ut_;
	point_t pos;
	int k=0;
	int movModel_;
	int numParams;
	bool endLoop = false;
	movementType *movType_;
	
	UTdeployFilename = "results/UTs_list.txt";
	UTdeployFile = fopen(UTdeployFilename.c_str(), "r");
	if (UTdeployFile == NULL) {
		DBG_ERR("Error opening file UT list for reading");
	}
	DBG_INFO2("Reading UTs list file %s", UTdeployFilename.c_str());

	do {
		ut_ = new userTerminal(m_accSel);
		numParams = fscanf(UTdeployFile, "%d %lf %lf %d %d %d %d",
			&(ut_->utID()), &pos.m_x, &pos.m_y, &(ut_->termTypeID()),
			&(ut_->userTypeID()), &(ut_->myOperator()), &movModel_);
		movType_ = getMovementType(movModel_);
		ut_->setMovementParams(movType_->model(), movType_->params());
		if (numParams == 7) {
			ut_->setServiceParams(m_cfgSce->getuserTypeList(), m_cfgSce->getservTypeList());
			m_utList.insert(pair<int,userTerminal*>(k++,ut_));
		}
		else {
			delete ut_;
			endLoop = true;
		}		
	} while (!endLoop);
	
	fclose(UTdeployFile);

}

// Solucion temporal
/*
int scenario::setTerminalTypeID(userTerminal *ut_)
{
	return 0;
}
*/

int scenario::setTerminalTypeID(userTerminal *ut_)
{
	double prob_;
	terminalType_iter_t iter;
	float threshold;
	bool found = false;
	terminalType_t termList;
	int val, i, tries = 20;

	DBG_IN("scenario::setTerminalID");
	
	for (i=0; i < tries; i++) {
		threshold = 0.0;
		prob_ = rng::instance()->uniform();
		termList = m_cfgSce->gettermTypeList();
		// Como prob_ siempre sera un valor menor que 1, rng genera valores entre [0,1>,
		//   y threshold acumula como maxino un valor igual a 1, entonces en algun momento
		//   del bucle "for" la variable "found" sera TRUE.
		for (iter=termList.begin(); iter!=termList.end(); iter++) {
			threshold = threshold + ((*iter).second)->termProb();
			//DBG_INFO2("TERM prob_ = %.2f threshold = %.2f", prob_, threshold);
			if (prob_ < threshold) {
				found = true;
				break;
			}
		}

		if (found) {
			val = ((*iter).second)->index();
			if (validateTermTypeID(val, ut_)) {
				DBG_INFO("utID %d set terminalTypeID to %d", ut_->utID(), val);
				break;
			}
		}
		else {
			DBG_ERR("setTerminalTypeID: ID with prob_ %.2f not found", prob_);
			exit(-1);
		}
	}
	
	if (!validateTermTypeID(val, ut_)) {
		DBG_ERR("setTerminalTypeID: TermTypeID not validated after %d tries", tries);
		exit(-1);
	}
	
	DBG_OUT("scenario::setTerminalID");
	return val;
}


bool scenario::validateTermTypeID(int termTypeID, userTerminal *ut_)
{
	terminalType_t termList;
	RATids_t utRATids;
	RATids_t::iterator utIter;
	operatorType_t opList;
	vector<int> numOfBSlist;
	bool result=false;
	int bsTypeID;

	
	termList = m_cfgSce->gettermTypeList();
	opList = m_cfgSce->getOPtypeList(); // map<int,operatorType *>
	numOfBSlist = opList[ut_->myOperator()]->getnumOfBS();
	utRATids = termList[termTypeID]->getRATids();
	
	for (utIter=utRATids.begin(); utIter != utRATids.end(); utIter++) {
		for (bsTypeID=0; bsTypeID < (int) numOfBSlist.size(); bsTypeID++) {
			if (numOfBSlist[bsTypeID] > 0) {
				// Se considera que el ratTypeID se corresponde con bsTypeID, es decir ratTypeID=0 es asignado a bsTypeID=0
				if (bsTypeID == *utIter) {
					result = true;
					break;
				}
			}
		}
	}
	
	return result;	
}

int scenario::setUserTypeID(void)
{
	double prob_;
	userType_iter_t iter;
	float threshold = 0.0;
	bool found = false;
	userType_t list;
	int val;

	DBG_IN("scenario::setUserID");
	
	prob_ = rng::instance()->uniform();
	list = m_cfgSce->getuserTypeList();
	for (iter=list.begin(); iter!=list.end(); iter++) {
		threshold = threshold + ((*iter).second)->userProb();
		//DBG_INFO2("USER prob_ = %.2f threshold = %.2f", prob_, threshold);
		if (prob_ < threshold) {
			found = true;
			break;
		}
	}
	
	if (found) {
		val = ((*iter).second)->index();
		//DBG_INFO2("Set user ID to %d", val);
	}
	else {
		DBG_ERR("ResultCode_ERROR!");
		val = ResultCode_ERROR;
	}

	DBG_OUT("scenario::setUserID");	
	return val;
}

int scenario::setOperatorID(void)
{
	double prob_;
	operatorType_iter_t iter;
	float threshold = 0.0;
	bool found = false;
	operatorType_t list;
	int val;

	DBG_IN("scenario::setOperatorID");
	
	prob_ = rng::instance()->uniform();
	list = m_cfgSce->getOPtypeList();
	for (iter=list.begin(); iter!=list.end(); iter++) {
		threshold = threshold + ((*iter).second)->marketProb();
		//DBG_INFO2("MARKET prob_ = %.2f threshold = %.2f", prob_, threshold);
		if (prob_ < threshold) {
			found = true;
			break;
		}
	}
	
	if (found) {
		val = ((*iter).second)->index();
		//DBG_INFO2("Set operator ID to %d", val);
	}
	else {
		DBG_ERR("ResultCode_ERROR!");
		val = ResultCode_ERROR;
	}

	DBG_OUT("scenario::setOperatorID");	
	return val;
}

movementType *scenario::setMovementType(void)
{
	double prob_;
	movType_iter_t iter;
	float threshold = 0.0;
	bool found = false;
	movType_t list;
	//int val;
	movementType *val;

	DBG_IN("scenario::setOperatorID");
	
	prob_ = rng::instance()->uniform();
	list = m_cfgSce->getmovTypeList();
	for (iter=list.begin(); iter!=list.end(); iter++) {
		threshold = threshold + ((*iter).second)->movProb();
		//DBG_INFO2("MOVEMENT prob_ = %.2f threshold = %.2f", prob_, threshold);
		if (prob_ < threshold) {
			found = true;
			break;
		}
	}
	
	if (found) {
		//val = ((*iter).second)->index();
		val = ((*iter).second);
		//DBG_INFO2("Set movement ID to %d", val);
	}
	else {
		DBG_ERR("ResultCode_ERROR!");
		val = NULL;
	}

	DBG_OUT("scenario::setMovementID");	
	return val;
}

movementType *scenario::getMovementType(int model)
{
	movType_iter_t iter;
	movType_t list;
	movementType *val;
	bool found = false;	
	
	list = m_cfgSce->getmovTypeList();
	for (iter=list.begin(); iter!=list.end(); iter++) {
		if ( ((*iter).second)->model() == model) {
			found = true;
			break;
		}
	}
	if (found) {
		val = ((*iter).second);
	}
	else {
		DBG_ERR("Movement type for model %d not found!", model);
		exit(-1);

	}	

	return val;
}

void scenario::setLoadMonitoringEvents(void)
{
	double stepTime;
	bs_iter_t bsIter_;
	basestation *bs;
		
	stepTime = m_cfgSce->ldStep() * m_cfgSce->simulTime();
	eventQueue::instance()->insertEvent(BS_LOADMONITORING_EVENT,stepTime,true,NULL,this);

	for (bsIter_ = m_bsList.begin(); bsIter_ != m_bsList.end(); bsIter_++) {
		bs = bsIter_->second;
		statistics::instance()->initStatsBSload(bs->bsID());
	}

}

void scenario::setSnapshotEvents(void)
{
	eventQueue::instance()->insertEvent(SNAPSHOT_EVENT,m_cfgSce->stepTime(),true,NULL,this);
}

void scenario::startSimulation(void)
{
	m_accSel->getBSs(m_bsList);
	m_accSel->getcfgSce(m_cfgSce);
	// m_accSel->CreateBSdeployFilefromAC(); // solo para prueba
	// m_accSel->getRATtypeList(*m_cfgSce); // solo para prueba
	
	eventQueue::instance()->run(m_cfgSce->simulTime());

}

bool scenario::IsSnapshotSetup(void)
{
	return m_cfgSce->snapshotSetup();
}

int scenario::traceFileSetup(void)
{
	return m_cfgSce->traceFileSetup();
}

void scenario::dumpStatistics(void)
{
	//dumpServiceSummaryStats();
	dumpUserSummaryStats();
	//dumpLinkQualityStats();
	//dumpHOperServStats();
	dumpHOperUserStats();
	dumpLoadStats();
	//dumpListOfParams();
	dumpPriceStats();
	//dumpPricePerServStats();
	dumpPricePerUserStats();
	
	//printf("Percentage of times with WiFi = %.6f\n",((float) statistics::instance()->aux2() )/(1.0 *  statistics::instance()->aux1() ));
}

void scenario::dumpServiceSummaryStats(void)
{
	string serviceStatsFilename;
	FILE *serviceStatsFile;
	//unsigned int totServ, i;
	//int stateCall, callID, lqSize;
	map<int, vector<int> > summStats;
	map<int, vector<int> >::iterator iter;
	char usersfilePrefix[5];
	
	sprintf(usersfilePrefix,"%03d",m_cfgSce->numOfUsers());
	serviceStatsFilename = "results/stats/serviceStats.out";
	serviceStatsFile = fopen(serviceStatsFilename.c_str(), "w");
	if (serviceStatsFile == NULL) {
		DBG_ERR("Error opening file serviceStats");
		exit(-1);
	}
	DBG_INFO2("Generating service summary stats file %s", serviceStatsFilename.c_str());
	
	statistics::instance()->getServiceSummaryStats(summStats);
	fprintf(serviceStatsFile, "#servTypeID initiatedServ successfulServ rejectedServ droppedServ\n");
	for (iter=summStats.begin(); iter!=summStats.end(); iter++) {
		fprintf(serviceStatsFile, "%5d %5d %5d %5d %5d\n", (*iter).first,
			(*iter).second[0], (*iter).second[1], (*iter).second[2], (*iter).second[3]);
	}
	
	// Solo para verificar
	// totServ = statistics::instance()->getTotalServStats();
	// fprintf(serviceStatsFile,"\ntotal servicios %d\n",totServ);
	// fprintf(serviceStatsFile,"\ncallID state lqSize\n");
	// for (i = 0; i<totServ; i++) {
		// statistics::instance()->getStateCallStats(i, callID, stateCall, lqSize);
		// fprintf(serviceStatsFile,"%5d %3d %3d\n", callID, stateCall, lqSize);
	// }
	fclose(serviceStatsFile);
}

void scenario::dumpUserSummaryStats(void)
{
	string userStatsFilename;
	FILE *userStatsFile;
	//unsigned int totServ, i;
	//int stateCall, callID, lqSize;
	map<int, vector<int> > summStats;
	map<int, vector<int> >::iterator iter;
	char usersfilePrefix[5];
	
	sprintf(usersfilePrefix,"%03d",m_cfgSce->numOfUsers());
	userStatsFilename = "results/stats/userStats.out";
	userStatsFile = fopen(userStatsFilename.c_str(), "w");
	if (userStatsFile == NULL) {
		DBG_ERR("Error opening file userStats");
		exit(-1);
	}
	DBG_INFO2("Generating user summary stats file %s", userStatsFilename.c_str());
	
	statistics::instance()->getUserSummaryStats(summStats);
	fprintf(userStatsFile, "#userTypeID initiatedServ successfulServ rejectedServ droppedServ\n");
	for (iter=summStats.begin(); iter!=summStats.end(); iter++) {
		fprintf(userStatsFile, "%5d %5d %5d %5d %5d\n", (*iter).first,
			(*iter).second[0], (*iter).second[1], (*iter).second[2], (*iter).second[3]);
	}
	
	// Solo para verificar
	// totServ = statistics::instance()->getTotalServStats();
	// fprintf(userStatsFile,"\ntotal servicios %d\n",totServ);
	// fprintf(userStatsFile,"\ncallID state lqSize\n");
	// for (i = 0; i<totServ; i++) {
		// statistics::instance()->getStateCallStats(i, callID, stateCall, lqSize);
		// fprintf(userStatsFile,"%5d %3d %3d\n", callID, stateCall, lqSize);
	// }
	fclose(userStatsFile);
}

void scenario::dumpLinkQualityStats(void)
{
	string lqStatsFilename;
	FILE *lqStatsFile;
	unsigned int totServ, stateCall, i;
	//vector<float> lqVector;
	vector<float>::iterator lqIter;
	//int servType, numHOs;
	//float duration, rateOP;
	servParamValues sValues;
	char usersfilePrefix[5];
	
	sprintf(usersfilePrefix,"%03d",m_cfgSce->numOfUsers());
	lqStatsFilename = "results/stats/linkQualityStats.out";
	lqStatsFile = fopen(lqStatsFilename.c_str(), "w");
	if (lqStatsFile == NULL) {
		DBG_ERR("Error opening file link quality Stats");
		exit(-1);
	}
	DBG_INFO2("Generating link quality stats file %s", lqStatsFilename.c_str());
	totServ = statistics::instance()->getTotalServStats();
	fprintf(lqStatsFile, "#servTypeID  income duration  numHOs  rateOP  LQvalues ...\n");
	for (i = 0; i<totServ; i++) {
		//stateCall = statistics::instance()->getLQvectorStats(i, lqVector, servType, duration, numHOs, rateOP);
		stateCall = statistics::instance()->getLQvectorStats(i, sValues, m_cfgSce->getservTypeList());
		if (stateCall == SUCCESS_CALL) {
			fprintf(lqStatsFile, "%2d  %08.2f  %08.2f %3d %6.2f   ", sValues.servType, sValues.income, sValues.duration, sValues.numHOs, sValues.rateOP);
			for (lqIter=sValues.lqVector.begin(); lqIter!=sValues.lqVector.end(); lqIter++) {
				fprintf(lqStatsFile, "%4.2f ", *lqIter);
			}
			fprintf(lqStatsFile, "\n");
		}
		sValues.lqVector.clear();
	}
	fclose(lqStatsFile);
}

void scenario::dumpHOperServStats(void)
{
	string hoStatsFilename;
	FILE *hoStatsFile;
	map<int, vector<int> > HOstats;
	map<int, vector<int> >::iterator hoIter;
	int i;
	char usersfilePrefix[5];
	
	sprintf(usersfilePrefix,"%03d",m_cfgSce->numOfUsers());
	hoStatsFilename = "results/stats/hoPerServStats.out";
	hoStatsFile = fopen(hoStatsFilename.c_str(), "w");
	if (hoStatsFile == NULL) {
		DBG_ERR("Error opening file handover stats");
		exit(-1);
	}
	DBG_INFO2("Generating handover stats file %s", hoStatsFilename.c_str());
	statistics::instance()->getHOperServStats(HOstats, m_cfgSce->getnumOfServices());
	fprintf(hoStatsFile, "#numHOs numServType0 numServType1 ...\n");
	for (hoIter=HOstats.begin(); hoIter!=HOstats.end(); hoIter++) {
		fprintf(hoStatsFile, "%4d ",(*hoIter).first);
		for (i=0; i < m_cfgSce->getnumOfServices(); i++) {
			fprintf(hoStatsFile, "%4d ",(*hoIter).second[i]);
		}
		fprintf(hoStatsFile, "\n");
	}
	fclose(hoStatsFile);
}


void scenario::dumpHOperUserStats(void)
{
	string hoStatsFilename;
	FILE *hoStatsFile;
	map<int, vector<int> > hoStats;
	map<int, vector<int> >::iterator hoIter;
	int i;
	char usersfilePrefix[5];
	
	sprintf(usersfilePrefix,"%03d",m_cfgSce->numOfUsers());
	hoStatsFilename = "results/stats/hoPerUserStats.out";
	hoStatsFile = fopen(hoStatsFilename.c_str(), "w");
	if (hoStatsFile == NULL) {
		DBG_ERR("Error opening file handover stats");
		exit(-1);
	}
	DBG_INFO2("Generating handover stats file %s", hoStatsFilename.c_str());
	statistics::instance()->getHOperUserStats(hoStats, m_cfgSce->getnumOfTypeUsers());
	fprintf(hoStatsFile, "#numHOs numUserType0 numUserType1 ...\n");
	for (hoIter=hoStats.begin(); hoIter!=hoStats.end(); hoIter++) {
		fprintf(hoStatsFile, "%4d ",(*hoIter).first);
		for (i=0; i < m_cfgSce->getnumOfTypeUsers(); i++) {
			fprintf(hoStatsFile, "%4d ",(*hoIter).second[i]);
		}
		fprintf(hoStatsFile, "\n");
	}
	fclose(hoStatsFile);
}

void scenario::dumpLoadStats(void)
{
	string ldStatsFilename;
	FILE *ldStatsFile;
	map<int, vector<float> > statsBSload;
	map<int, vector<float> >::iterator loadIter;
	vector<float> loadValues;
	vector<float>::iterator valIter;
	int bsID;
	char usersfilePrefix[5];
	
	sprintf(usersfilePrefix,"%03d",m_cfgSce->numOfUsers());
	ldStatsFilename = "results/stats/loadStats.out";
	ldStatsFile = fopen(ldStatsFilename.c_str(), "w");
	if (ldStatsFile == NULL) {
		DBG_ERR("Error opening load stats file");
		exit(-1);
	}
	DBG_INFO2("Generating load stats file %s", ldStatsFilename.c_str());
	statsBSload = statistics::instance()->getLoadStats();
	fprintf(ldStatsFile,"#bsID loadBS_values ...\n");
	for (loadIter=statsBSload.begin(); loadIter!=statsBSload.end(); loadIter++) {
		bsID = (*loadIter).first;
		loadValues = (*loadIter).second;
		fprintf(ldStatsFile,"%3d ",bsID);
		for (valIter=loadValues.begin(); valIter!=loadValues.end(); valIter++) {
			fprintf(ldStatsFile,"%3.0f ",*valIter);
		}
		fprintf(ldStatsFile,"\n");
	}
	fclose(ldStatsFile);
}

void scenario::dumpListOfParams(void)
{
	bs_iter_t bsIter_;
	basestation *bs;
	string paramsListFilename;
	FILE *paramsListFile;
	char usersfilePrefix[5];
	
	sprintf(usersfilePrefix,"%03d",m_cfgSce->numOfUsers());
	paramsListFilename = "results/stats/paramsList.out";
	paramsListFile = fopen(paramsListFilename.c_str(), "w");
	if (paramsListFile == NULL) {
		DBG_ERR("Error opening params list file");
		exit(-1);
	}
	DBG_INFO2("Generating params list file %s", paramsListFilename.c_str());
	
	fprintf(paramsListFile,"#bsID opTypeID capacity bsTypeID\n");
	for (bsIter_ = m_bsList.begin(); bsIter_ != m_bsList.end(); bsIter_++) {
		bs = bsIter_->second;
		fprintf(paramsListFile,"%4d %4d %6.2f %3d\n",bs->bsID(),bs->opTypeID(),bs->capacity(),bs->bsTypeID());
	}
	fclose(paramsListFile);
}

void scenario::dumpPriceStats(void)
{
	bs_iter_t bsIter_;
	basestation *bs;
	string priceListFilename;
	FILE *priceListFile;
	map<int,float> incomes;
	float relativeIncome;
	char usersfilePrefix[5];
	
	sprintf(usersfilePrefix,"%03d",m_cfgSce->numOfUsers());
	priceListFilename = "results/stats/priceList.out";
	priceListFile = fopen(priceListFilename.c_str(), "w");
	if (priceListFile == NULL) {
		DBG_ERR("Error opening price list file");
		exit(-1);
	}
	DBG_INFO2("Generating price list file %s", priceListFilename.c_str());
	
	statistics::instance()->getPriceStats(incomes,m_cfgSce->getservTypeList());
	fprintf(priceListFile,"#bsID bsTypeID opTypeID relativeIncome\n");
	for (bsIter_ = m_bsList.begin(); bsIter_ != m_bsList.end(); bsIter_++) {
		bs = bsIter_->second;
		relativeIncome = incomes[bs->bsID()]/m_cfgSce->simulTime();
		fprintf(priceListFile,"%4d %4d %4d   %09.2f\n",bs->bsID(),bs->bsTypeID(),bs->opTypeID(),relativeIncome);
	}

	fclose(priceListFile);
}

void scenario::dumpPricePerServStats(void)
{
	string priceStatsFilename;
	FILE *priceStatsFile;
	map<int,float> incomes;
	map<int,float>::iterator iter;
	float relativeIncome, totPrice;
	map<int, vector<int> > summStats;
	map<int, vector<int> >::iterator iterSumm;
	const int successfulService = 1;
	int servTypeID;
	char usersfilePrefix[5];
	
	sprintf(usersfilePrefix,"%03d",m_cfgSce->numOfUsers());
	priceStatsFilename = "results/stats/pricePerServStats.out";
	priceStatsFile = fopen(priceStatsFilename.c_str(), "w");
	if (priceStatsFile == NULL) {
		DBG_ERR("Error opening pricePerServStats file");
		exit(-1);
	}
	DBG_INFO2("Generating price per services file %s", priceStatsFilename.c_str());
	
	statistics::instance()->getPricePerServStats(incomes,m_cfgSce->getservTypeList());
	fprintf(priceStatsFile,"#servTypeID relativeIncome\n");
	for (iter = incomes.begin(); iter!=incomes.end(); iter++) {
		servTypeID = (*iter).first;
		totPrice = (*iter).second;
		statistics::instance()->getServiceSummaryStats(summStats);
		iterSumm = summStats.find(servTypeID);
		if (iterSumm == summStats.end()) {
			DBG_ERR("servTypeID %d no se encuentra en summStats!!", servTypeID);
			exit(-1);
		}
		relativeIncome = totPrice/(*iterSumm).second[successfulService];
		fprintf(priceStatsFile,"%4d  %09.2f\n", servTypeID,relativeIncome);
	}
	
	fclose(priceStatsFile);
}

void scenario::dumpPricePerUserStats(void)
{
	string priceStatsFilename;
	FILE *priceStatsFile;
	map<int,float> payment;
	map<int,float>::iterator iter;
	float averagePrice, totalPayment;
	map<int, vector<int> > summStats;
	map<int, vector<int> >::iterator iterSumm;
	const int successfulService = 1;
	const int servTypeID = 0; // Se asume que solo hay un tipo de servicio
	int userTypeID;
	char usersfilePrefix[5];
	
	sprintf(usersfilePrefix,"%03d",m_cfgSce->numOfUsers());
	priceStatsFilename = "results/stats/pricePerUserStats.out";
	priceStatsFile = fopen(priceStatsFilename.c_str(), "w");
	if (priceStatsFile == NULL) {
		DBG_ERR("Error opening pricePerServStats file");
		exit(-1);
	}
	DBG_INFO2("Generating price per services file %s", priceStatsFilename.c_str());
	
	statistics::instance()->getPayPerUserStats(payment,m_cfgSce->getuserTypeList(), m_cfgSce->getservTypeList());
	fprintf(priceStatsFile,"#userTypeID averagePrice\n");
	statistics::instance()->getUserSummaryStats(summStats);
	for (iter = payment.begin(); iter!=payment.end(); iter++) {
		userTypeID = (*iter).first;
		totalPayment = (*iter).second;
		iterSumm = summStats.find(userTypeID);
		if (iterSumm == summStats.end()) {
			DBG_ERR("userTypeID %d no se encuentra en summStats!!", servTypeID);
			exit(-1);
		}
		if ((*iterSumm).second[successfulService] > 0) {
			averagePrice = totalPayment/(*iterSumm).second[successfulService];
		}
		else {
			averagePrice = 0;
		}
		fprintf(priceStatsFile,"%4d  %09.2f\n", userTypeID,averagePrice);
		//DBG_INFO("userType %d totalPayment %.2f successCall %d", userTypeID, totalPayment,(*iterSumm).second[successfulService]);
	}
	
	fclose(priceStatsFile);
}
