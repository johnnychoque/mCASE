#ifndef _INC_OPERATORTYPE_H
#define _INC_OPERATORTYPE_H

#include <vector>
#include <map>
using namespace std;

class operatorType;

typedef map<int,operatorType *> operatorType_t;
typedef operatorType_t::iterator operatorType_iter_t;
typedef vector<int> numOfBSlist_t;
typedef vector<float> priceOfBSlist_t;

class operatorType {
	public:
		operatorType();
		~operatorType();
		
		inline int &index(void) {return m_index;}
		inline void setnumOfBS(numOfBSlist_t numOfBSlist_) {m_numOfBSlist = numOfBSlist_;}
		inline numOfBSlist_t getnumOfBS(void) {return m_numOfBSlist;}
		inline void setpriceOfBS(priceOfBSlist_t priceOfBSlist_) {m_priceOfBSlist = priceOfBSlist_;}
		inline priceOfBSlist_t getpriceOfBS(void) {return m_priceOfBSlist;}
		inline float &marketProb(void) {return m_marketProb;}
		float &bsPriceFactor(void) {return m_bsPriceFactor;}
		float &opPriceFactor(void) {return m_opPriceFactor;}

	private:
		int m_index;
		numOfBSlist_t m_numOfBSlist;
		priceOfBSlist_t m_priceOfBSlist;
		float m_marketProb;
		float m_bsPriceFactor;
		float m_opPriceFactor;		
};

#endif //  _INC_OPERATORTYPE_H