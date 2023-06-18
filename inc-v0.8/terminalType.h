#ifndef _INC_TERMINALTYPE_H
#define _INC_TERMINALTYPE_H

#include <vector>
#include <map>
using namespace std;

class terminalType;

typedef map<int,terminalType *> terminalType_t;
typedef terminalType_t::iterator terminalType_iter_t;
typedef vector<int> RATids_t;

class terminalType {
	public:
		terminalType();
		~terminalType();
		
		inline int &index(void) {return m_index;}
		inline float &termProb(void) {return m_prob;}
		inline void setRATids(RATids_t RATids_) {m_RATids = RATids_;}
		inline RATids_t getRATids(void) {return m_RATids;}
		
	private:
		int m_index;
		float m_prob;
		RATids_t m_RATids;
};

#endif //  _INC_TERMINALTYPE_H
