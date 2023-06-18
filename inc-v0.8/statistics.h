#ifndef _INC_STATISTICS_H
#define _INC_STATISTICS_H

#include <map>
#include <vector>
#include <cstdlib>
#include "common.h"
#include "serviceType.h"
#include "userType.h"

using namespace std;

// m_prefOP: Cada vez que se admite una nueva llamada o ser realiza un HO, m_prefOP guarda un flag que indica si la nueva BS pertenece o no al operador preferido del UT
// m_result: Indica en que estado ha finalizado la llamada (successful, reject o drop)
// m_price: lista de precios a pagar por el usuario en diversos instantes de la duracion del servicio dependiendo de la BS y operador al cual se conecta.
//          Cada vez que inicia un nuevo servicio o se realiza un HO se añade una nueva entrada en este vector.
// m_currBS: Cada vez que se admite una nueva llamada o ser realiza un HO, m_currBS almacena el ID de la BS al cual se esta conectando
// m_tDrop: Si el servicio finaliza como DROP_CALL entonces m_tDrop guarda el tiempo en el cual ocurre ese evento
typedef struct {
	double m_tStart;
	double m_tEnd;
	vector<double> m_tHOs;
	vector<int> m_currBS; 
	vector<bool> m_prefOP; 
	vector<float> m_linkQuality;
	vector<float> m_price;
	double m_tDrop;
	int m_userType;
	int m_prefOperatorID; // No se utiliza por el momento
	int m_servType;
	int m_result;
} statsPerServ;

typedef struct {
	vector<float> lqVector;
	int servType;
	float duration;
	int numHOs;
	float rateOP;
	float income;
} servParamValues;

// key (int) = callID
typedef map<int, statsPerServ*> statsPerServ_t;
typedef statsPerServ_t::iterator statsPerServ_iter_t;

// key (int) = bsID, value (vector<float>) = valores de carga del BS
typedef map<int, vector<float> > statsBSload_t;
typedef statsBSload_t::iterator statsBSload_iter_t;

typedef map<int, int> statsHOsummary_t;
typedef statsHOsummary_t::iterator statsHOsummary_iter_t;

class statistics
{
  public:
	static statistics* instance();
	void endInstance(void);
	
	void initStatsPerServ(int callID);

	void statsInsert_tStart(int callID, double tStart);
	void statsInsert_tEnd(int callID, double tEnd);
	void statsInsert_tDrop(int callID, double tDrop);
	void statsInsert_userType(int callID, int userType);
	void statsInsert_servType(int callID, int servType);
	
	void statsInsert_tHOs(int callID, double tHO);
	void statsInsert_currBS(int callID, int currBS);
	void statsInsert_prefOP(int callID, bool value);
	void statsInsert_lq(int callID, float lq);
	void statsInsert_result(int callID, int result);
	void statsInsert_price(int callID, float price);
	
	void initStatsBSload(int bsID);
	void statsInsert_bsLoad(int bsID, float load);
	unsigned int getTotalServStats(void);
	//unsigned int getServiceSummaryStats(int paramStat);
	void getServiceSummaryStats(map<int, vector<int> >& summStats);
	void getUserSummaryStats(map<int, vector<int> >& summStats);
	//unsigned int getLQvectorStats(int index, vector<float> &lqVector, int& servType, float& duration, int& numHOS, float& rateOP);
	unsigned int getLQvectorStats(int index, servParamValues& sValues, map<int,serviceType *> servTypeList);
	void getHOperServStats(map<int, vector<int> >& HOstats, int numOfServs);
	void getHOperUserStats(map<int, vector<int> >& hoStats, int numTypeUsers);
	statsBSload_t getLoadStats(void);
	void getStateCallStats(int index, int& callID, int& stateCall, int& lqSize);
	void getPriceStats(map<int,float>& incomes, map<int,serviceType *> servTypeList);
	void getPricePerServStats(map<int,float>& incomes, map<int,serviceType *> servTypeList);
	void getIncomePerUserStats(map<int,float>& incomes);
	void getPayPerUserStats(map<int,float>& payment, userType_t userTypeList, serviceType_t servTypeList);
	
	inline int &aux1(void) {return m_aux1;}
	inline int &aux2(void) {return m_aux2;}
	
  private:
	static statistics *m_instance;
	statistics() {}                     // Private constructor
	~statistics() {}
	statistics(const statistics&);      // Prevent copy-construction
	statistics& operator=(const statistics&);  // Prevent assignment
	unsigned int m_rejectCall;
	unsigned int m_dropCall;
	unsigned int m_newCall;
	unsigned int m_handoverCall;
	
	statsPerServ_t m_statsPerServ;
	statsBSload_t m_statsBSload;
	statsHOsummary_t m_statsHOsummary;
	
	int m_aux1; // For debugging purposes
	int m_aux2; // For debugging purposes
};

#endif //  _INC_STATISTICS_H