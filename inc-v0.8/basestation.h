#ifndef _INC_BASESTATION_H
#define _INC_BASESTATION_H

#include <map>
#include <string>
#include "common.h"
using namespace std;

class basestation;

typedef map<int,basestation *> bs_t;
typedef bs_t::iterator bs_iter_t;

class basestation {
	public:
		basestation();
		~basestation();
		
		int &bsID(void) {return m_bsID;}
		int &bsTypeID(void) {return m_bsTypeID;}
		int &opTypeID(void) {return m_opTypeID;}
		inline double &posX(void) {return m_posX;}
		inline double &posY(void) {return m_posY;}
		point_t getPosition(void);
		inline float &capacity(void) {return m_capacity;}
		inline float &range(void) {return m_range;}
		inline float &currLoad(void) {return m_currLoad;}
		inline float &maxPrice(void) {return m_maxPrice;}
		inline float &currPrice(void) {return m_currPrice;}
		// inline float &pend(void) {return m_pend;}
		// inline float &inter(void) {return m_inter;}
		inline float &bsPriceFactor(void) {return m_bsPriceFactor;}
		inline float &opPriceFactor(void) {return m_opPriceFactor;}
		inline float &lowerThrLoad(void) {return m_lowerThrLoad;}
		inline float &upperThrLoad(void) {return m_upperThrLoad;}
		float getcurrPrice(float bsAvaiRelatLoad);

	private:
		//! m_bsID es el identificador del BS
		int m_bsID;
		//! m_bsTypeID define el tipo de RAT asiganado al BS y la minima distancia entre BSs del mismo tipo
		int m_bsTypeID;
		//! m_opTypeID es el identificador del operador al cual pertence el BS
		int m_opTypeID;
		//! m_posX, m_posY posicion de la BS definida durante la fase de despliegue
		double m_posX, m_posY;
		//! m_capacity maxima carga soportada por la BS. Depende de la RAT asignada durante la fase de despliegue
		float m_capacity;
		//! m_range radio que define el area de cobertura de la BS
		float m_range;
		//! m_currLoad carga actual que soporta la BS. Debe ser menor o igual que la capacidad del BS
		float m_currLoad;
		//! m_maxPrice precio maximo que oferta el operador para esta BS. Utilizado como referencia fija para el calculo de m_currPrice.
		float m_maxPrice;
		//! m_currPrice precio temporal que actual ofrece la BS en funcion de la carga libre que tenga.
		float m_currPrice;

		//! Parametros de la funcion lineal Precio(carga_libre). m_pend pendiente, m_inter interseccion con eje de ordenadas
		// float m_pend;
		// float m_inter;
		float m_bsPriceFactor;
		float m_opPriceFactor;
		float m_lowerThrLoad;
		float m_upperThrLoad;
};

#endif //  _INC_BASESTATION_H