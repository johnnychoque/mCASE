#ifndef _INC_USERTYPE_H
#define _INC_USERTYPE_H

#include <vector>
#include <map>
#include <string>
using namespace std;

class userType;

typedef map<int,userType *> userType_t;
typedef userType_t::iterator userType_iter_t;
typedef vector<int> userServs_t;

class userType {
	public:
		userType();
		~userType();

		inline int &index(void) {return m_index;}
		inline float &userProb(void) {return m_prob;}
		inline void setuserServs(userServs_t userServs_) {m_userServs = userServs_;}
		inline userServs_t getuserServs(void) {return m_userServs;}
		inline float &userPrice(void) {return m_price;}
		
	private:
		int m_index;
		float m_prob;
		userServs_t m_userServs;
		float m_price;
};

#endif //  _INC_USERTYPE_H
