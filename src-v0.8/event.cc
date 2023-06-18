#include "event.h"
#include <iostream>

#define DBG_LEVEL 1
#include "debug.h"

unsigned long int event::global_id = 0;
eventQueue *eventQueue::m_instance = 0;

eventQueue *eventQueue::instance()
{
	if(m_instance == 0) {
		m_instance = new eventQueue;
	}
	return m_instance;
}

void eventQueue::endInstance()
{
	if (m_instance != 0) {
		clear();
		delete m_instance;
	}

}

unsigned long int eventQueue::insertEvent(unsigned char code_, double delta_t_, 
							bool periodic_, void *data_, eventHandler *evhandler_)
{
	event *ev_;
	evqIter_t iter;
	
	ev_ = new event;
	ev_->code = code_;
	ev_->delta_t = delta_t_;
	ev_->periodic = periodic_;
	ev_->data = data_;
	ev_->evhandler = evhandler_;
	add(ev_);
	// if (ev_->code == UT_SERVICE_EVENT) {
		// for (iter = event_queue.begin(); iter != event_queue.end(); iter++) {
			// ev = (*iter).second;
			// if (ev->code == UT_SERVICE_EVENT) {
				// sData = (servData_t *)ev->data;
				// DBG_INFO(">e callID %d servType %d tWhen %.2f state %d evID %lu endEvID %d", sData->callID, sData->servType, sData->tWhen, sData->state, ev->id, sData->endEvID);
			// }
		// }
	// }
	//DBG_INFO("insertEvent evID %lu OK", ev_->id)
	return (ev_->id);
}

void eventQueue::deleteEvent(unsigned long int id_)
{
	event* ev_;
	evqIter_t evqIter;
	bool found = false;
		
	for (evqIter = event_queue.begin(); evqIter!=event_queue.end(); evqIter++) {
		ev_ = (*evqIter).second;
		if (ev_->id == id_) {
			delete ev_;
			event_queue.erase(evqIter);
			//DBG_WARN("eventQueue::deleteEvent evID %lu deleted", id_);
			found = true;
			break;		
		}
	}

	if (!found) {
		DBG_ERR("Event with id %lu not found. Please fix it!", id_);
		exit(-1);
	}
	//DBG_WARN("deleteEvent: evID %lu deleted", id_);
}

void eventQueue::add(event *ev_)
{
	ev_->expire_t = tsim + ev_->delta_t;
	event_queue.insert( pair<double,event*>(ev_->expire_t,ev_) );
}

void eventQueue::run(double totalTsim)
{
	struct eventMessage *message;
	event* ev_;
	evqIter_t evqIter;
	//unsigned int xx=0;
	DBG_IN("eventQueue::run");
	
	while (!event_queue.empty() && (tsim<totalTsim)) {
		evqIter = event_queue.begin(); // Next event to process.
		ev_ = (*evqIter).second; // pointer to the event
		//event* ev_ = event_queue.top(); // Next event to process.
		event_queue.erase(evqIter); // Remove event from event_queue.
		//DBG_WARN("eventQueue::run evID %lu deleted %u", ev_->id, xx++);
		DBG_WARN("eventQueue::run evID %lu deleted", ev_->id);
		tsim = ev_->expire_t; // Update current time.
		
		// Dispatch event
		message = new struct eventMessage;
		//std::cout << "ev_->code = " << ev_->code << std::endl;
		message->code = ev_->code;
		message->data = ev_->data;
		message->id = ev_->id;
		ev_->evhandler->onEvent(message); // Execute the event.
		delete message;		

		if (ev_->periodic) {
			add(ev_);
		}
		else {
			delete ev_; // This event is history!
		}
	}

	DBG_OUT("eventQueue::run");
}

void eventQueue::clear()
{
	//event* ev_;
	evqIter_t evqIter;
		
	for (evqIter = event_queue.begin(); evqIter!=event_queue.end(); evqIter++) {
		DBG_WARN("Deleting evID %lu code %d",(*evqIter).second->id, (*evqIter).second->code);
		delete ((*evqIter).second);
	}
	event_queue.clear();
	
	// while (!event_queue.empty()) {
		// ev_ = event_queue.top();
		// //std::cout << "Deleting event id=" << ev_->id << std::endl;
		// delete ev_;
		// event_queue.pop();
	// }
	tsim = 0;
}

void eventQueue::printEvents()
{
	evqIter_t evqIter;
	event* ev;
		
	for (evqIter = event_queue.begin(); evqIter!=event_queue.end(); evqIter++) {
		ev = (*evqIter).second;
		if (ev->code == UT_SERVICE_EVENT) {
			servData_t *sData;
			sData = (servData_t *)ev->data;
			DBG_INFO("Event id %6lu expire %8.2f state %d endEvID %lu", ev->id, ev->expire_t, sData->state, sData->endEvID);
		}
		else {
			//DBG_INFO("Event id %6lu expire %8.2f", ev->id, ev->expire_t);		
		}
	}
}