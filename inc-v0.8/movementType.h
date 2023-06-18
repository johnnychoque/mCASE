#ifndef _INC_MOVEMENTTYPE_H
#define _INC_MOVEMENTTYPE_H

#include <map>
#include <string>
#include "common.h"

using namespace std;

class movementType;

typedef map<int,movementType *> movType_t;
typedef movType_t::iterator movType_iter_t;

class movementType {
	public:
		movementType();
		~movementType();
		
		inline int &index(void) {return m_index;}
		inline int &model(void) {return m_model;}
		inline float &movProb(void) {return m_movProb;}
		inline struct movParams &params(void) {return m_params;}
		
	private:
		int m_index;
		int m_model;
		float m_movProb;
		struct movParams m_params;
};

#endif //  _INC_SERVICETYPE_H
