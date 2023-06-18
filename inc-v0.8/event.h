#ifndef _INC_EVENT_H
#define _INC_EVENT_H

#include <queue>
#include <deque>
#include <map>
#include <cstdlib>
#include "common.h"
using namespace std;

class eventHandler;
class eventQueue;

/**
 * \brief This structure is passed to an event costumer
 * 
 * The structure keeps the code of the event that has happened, as well as the information
 * that the "client" is willing to receive
 */
struct eventMessage {
	unsigned char code;			/**< Code of the event */
	unsigned long int id;	/**< Id of the event */ 
	void *data;					/**< Information that the client is expecting */
};

/*!
  \brief Event Handler Class

  An abstract Base class of Events that can be used to derive new events. All Event classes
  need to define the onEvent() function which is called when the event expires. An event has an
  execution time and an id.
*/
class eventHandler {
  public:
	eventHandler() {}
	virtual ~eventHandler(){}
	//! interface que sera ejecutada cuando un evento expire
	virtual void onEvent(struct eventMessage *message) = 0;
};

class event {
  public:
	friend class eventQueue;
	friend struct compareEventTimes;

	//! Schedule an event at time \c delta_time from now
	event() {
		expire_t = 0; // Will be set correctly upon calling eventQueue::add().
		id = global_id++;
	}
	//! Destructor
	virtual ~event(){}
  private:
	eventHandler *evhandler;
	//! Code identifica el tipo de evento de una determinada clase 
	unsigned char code;
	//! delta_t es el tiempo relativo respecto al instante en el que se crea el evento.
	//! The event will occur in 'delta_time' time units from now!
	double delta_t;
	//! periodic define si un evento se repite constantemente hasta que sea eliminado
	bool periodic;
	//! data almacena cualquier parametro que el evento necesite cuando sea ejecutado
	void *data;	
	
	//! expire_t es el tiempo absoluto del evento dentro de la simulación. Indica el instante en el que se ejecuta el evento
	double expire_t;
	//! Numero de identifiación unico del evento
	unsigned long int id;
	//! Guarda la numeracion sucesiva de los eventos
	static unsigned long int global_id;	
};

//! Compare to events, Returns true if expire time of event1 is larger than the expire time of event2
//! caso contrario retorna true si el ID del event1 es mayor que el ID del event2
struct compareEventTimes {
	bool operator()(double expire_t1, double expire_t2) {
		return (expire_t1 < expire_t2); // Different expire times. Regular comparison.
	}
};

/*!
  \brief Event Queue class

  A class for storing and executing events. Events can be added to the queue and when the start() is
  called all events will be executed. Observe that Events need to be created before they are added to the
  queue by calling an appropriate constructor. However, expired events are destroyed automatically (the destructor is called).
*/
class eventQueue {
  public:
	static eventQueue *instance();
	void endInstance(void);
	//! Add event to Queue
	unsigned long int insertEvent(unsigned char code_, double delta_t_, 
					bool periodic_, void *data_, eventHandler *evhandler_);
	//! Delete event with specific id
	void deleteEvent(unsigned long int id_);
	//! Start executing events
	void run(double totalTsim);
	//! Remove all events	
	void clear();
	void printEvents();
	inline double currentTime(void) {return tsim;}

  private:
	static eventQueue *m_instance;
	eventQueue(void) { tsim=0; };
	~eventQueue(void) {};
	eventQueue(const eventQueue&);                 // Prevent copy-construction
	eventQueue& operator=(const eventQueue&);      // Prevent assignment
	
	//typedef std::deque<event*, std::allocator< event* > >::iterator event_iter;
	//! tsim almacena el tiempo actual de la simulacion
	double tsim;
	void add(event *ev_);
	// std::priority_queue < event*,
	// std::deque<event*, std::allocator<event*> >,
	// compareEventTimes > event_queue; // Queue for the Events.
	typedef multimap < double, event*, compareEventTimes > event_queue_t; // Queue for the Events.
	typedef event_queue_t::iterator evqIter_t;
	event_queue_t event_queue;
	
};

#endif // _INC_EVENT_H