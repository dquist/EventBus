/*
 * EventHandler.hpp
 *
 *  Created on: Oct 19, 2013
 *      Author: Dan
 */

#ifndef EVENTHANDLER_HPP_
#define EVENTHANDLER_HPP_

#include "Object.hpp"
#include <typeinfo>
#include "Event.hpp"

template <class T>
class EventHandler {
public:
	EventHandler() {

	}
	virtual ~EventHandler() { }

	virtual void onEvent(T*) = 0;

	void dispatch(Event* e) {
		onEvent((T*)e);
	}
};

#endif /* EVENTHANDLER_HPP_ */
