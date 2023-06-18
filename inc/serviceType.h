#ifndef _INC_SERVICETYPE_H
#define _INC_SERVICETYPE_H

#include <map>
#include <string>
using namespace std;

class serviceType;

typedef map<int,serviceType *> serviceType_t;
typedef serviceType_t::iterator serviceType_iter_t;

class serviceType {
	public:
		serviceType();
		~serviceType();
		
		inline int &index(void) {return m_index;}
		inline float &tia(void) {return m_tia;}
		inline float &ts(void) {return m_ts;}
		inline float &capacity(void) {return m_capacity;}
		inline bool &realtime(void) {return m_realtime;}

	private:
		int m_index;
		float m_tia;
		float m_ts;
		float m_capacity;
		bool m_realtime;
};

#endif //  _INC_SERVICETYPE_H
