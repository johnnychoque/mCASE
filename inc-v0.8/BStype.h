#ifndef _INC_BSTYPE_H
#define _INC_BSTYPE_H

#include <map>
#include <vector>
using namespace std;

class BStype;

typedef map<int,BStype *> BStype_t;
typedef BStype_t::iterator BStype_iter_t;

class BStype {
	public:
		BStype();
		~BStype();

		inline int &index(void) {return m_index;}
		inline int &RATid(void) {return m_RATid;}
		inline double &minDistance(void) {return m_minDistance;}

	private:
		int m_index;
		int m_RATid;
		double m_minDistance;
};

#endif // _INC_BSTYPE_H