#include <string>
#include <iostream>
using namespace std;

#define DBG_LEVEL 3
#define MAX_PATH 200

#include "common.h"
#include "configFile.h"
#include "debug.h"
#include "scenario.h"

int main(int argc, char *argv[])
{
	unsigned int pos;
	char path_[MAX_PATH];
	string confFile_;
	string fileName_;
	int result;
	
	configFile configFile_;
	scenario *scenario_;
	
	DBG_IN("main");

	if(argc != 2) {
		DBG_INFO("Multi-Constraint Access Selection in heterogeneous Environment: mCASE Simulator");
		DBG_INFO("Execute simulator from root dirtectory. Format command:");
		DBG_INFO("bin/mcase configs/configurationFile.cfg");
		DBG_OUT("main");
		exit(-1);
	}
	
	/* Obtaining the configuration File - stored in configs/ */
	fileName_ = argv[1];
	getcwd(path_,MAX_PATH);
	confFile_ = path_;

	if((pos = confFile_.find("bin")) != string::npos) {
		DBG_INFO("Run mcase outside bin directory...");
		DBG_INFO("bin/mcase configs/configurationFile.cfg");
		DBG_OUT("main");
		exit(-1);
	}

	if((pos = fileName_.find("configs")) == string::npos) {
		DBG_INFO("Configuration directory is missing ...");
		DBG_INFO("bin/mcase configs/configurationFile.cfg");
		DBG_OUT("main");
		exit(-1);
	}

	confFile_ += "/" + fileName_;
	
	DBG_INFO2("Opening configuration File: %s",confFile_.c_str());
	
	if(configFile_.loadConfig(confFile_) == -1) {
		DBG_ERR("Could not open Configuration File %s", confFile_.c_str());
		DBG_OUT("main");
		exit(-1);
	}

	scenario_ = new scenario;
	result = scenario_->makeConfiguration(configFile_);

	if (result==ResultCode_ERROR) {
		DBG_ERR("ResultCode_ERROR! system halted %d", result);
		DBG_OUT("main");
		exit(-1);
	}	

	switch (scenario_->traceFileSetup())
	{
		case MAKE_TRACES:
			// Obtener trazas para replicar escenario en otra simulacion
			DBG_INFO("Simulation and creating traces");
			scenario_->makeUTdeployment();
			scenario_->makeBSdeployment();
			scenario_->CreateBSdeployFile();
			scenario_->CreateUTdeployFile();
			scenario_->movementGenerator();
			scenario_->CreateMovementTraceFile();
			scenario_->serviceGenerator();
			scenario_->CreateServiceTraceFile();
			break;
		case LOAD_TRACES:
			// Replicando escenario
			DBG_INFO("Simulation using traces already created");
			scenario_->ReadBSdeployFile();
			scenario_->ReadUTdeployFile();
			scenario_->ReadMovementTraceFile();
			scenario_->ReadServiceTraceFile();
			break;
		case NO_TRACES:
			// Simulacion pura, sin trazas
			DBG_INFO("Only simulation, without traces");
			scenario_->makeUTdeployment();
			//scenario_->makeBSdeployment();
			//scenario_->CreateBSdeployFile();
			scenario_->ReadBSdeployFile("results/BSs_list.txt");
			scenario_->movementGenerator();
			scenario_->serviceGenerator();
			scenario_->setLoadMonitoringEvents();
			break;
		default:
			//Configuracion sin utilidad, evitarlo!
			DBG_ERR("There is some problem with Trace File section... Please FIX");
			DBG_OUT("main");
			exit(-1);
			break;
	}
	
	if (scenario_->IsSnapshotSetup()) {
		scenario_->CreateSS_BSdeployFile();
		scenario_->setSnapshotEvents();
		// Falta definir un flag que evite entrar al Detected Set para no ejecutar toda la simulacion solo para obtener fotos. Se uso m_accSel->IsSnapshotSetup() en requestResourcesToBS() de userTerminal.cc pero generaba errores.
	}

	//eventQueue::instance()->printEvents();
	DBG_INFO("Start simulation ...");
	scenario_->startSimulation();
	if (!scenario_->IsSnapshotSetup()) {
		scenario_->dumpStatistics();
	}
	configFile_.flushConfig();
	delete scenario_;
	
	return ResultCode_OK;
	
	DBG_OUT("main");
}
