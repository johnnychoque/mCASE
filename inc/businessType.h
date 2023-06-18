#ifndef _INC_BUSINESSTYPE_H
#define _INC_BUSINESSTYPE_H

#include <map>
#include <vector>
#include <string>
using namespace std;

class businessType;

typedef map<int,businessType *> businessType_t;
typedef businessType_t::iterator businessType_iter_t;
typedef vector<int> OPids_t;

class businessType {
	public:
		businessType();
		~businessType();

		inline int &index(void) {return m_index;}
		inline int &typeOfBM(void) {return m_type;}
		inline void setOPids(OPids_t OPids_) {m_OPids = OPids_;}
		inline void getOPids(OPids_t &OPids_) {OPids_ = m_OPids;}
		
	private:
		int m_index;
		int m_type;
		OPids_t m_OPids;
};

#endif // _INC_BUSINESSTYPE_H