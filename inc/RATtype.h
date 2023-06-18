#ifndef _INC_RATTYPE_H
#define _INC_RATTYPE_H

#include <map>
using namespace std;

class RATtype;

typedef map<int,RATtype *> RATtype_t;
typedef RATtype_t::iterator RATtype_iter_t;

class RATtype {
	public:
		RATtype();
		~RATtype();

		inline int &index(void) {return m_index;}
		inline double &range(void) {return m_range;}
		inline float &capacity(void) {return m_capacity;}

	private:
		int m_index;
		double m_range;
		float m_capacity;
};

#endif // _INC_RATTYPE_H
