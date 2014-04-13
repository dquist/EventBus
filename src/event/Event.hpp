/*
 * BaseEvent.hpp
 *
 *  Created on: Oct 19, 2013
 *      Author: Dan
 */

#include "Object.hpp"
#include <typeindex>
#include <typeinfo>
#include <boost/any.hpp>
#include <vector>

#ifndef BASEEVENT_HPP_
#define BASEEVENT_HPP_

class Event : public Object
{
public:
	Event(std::type_index typeIndex, Object* const sender) :
		typeIndex(typeIndex),
		sender(sender),
		canceled(false) {
		if (sender == nullptr) {
			throw std::runtime_error("Event parameter 'sender' was null. \r\n");
		}
	}

	virtual ~Event() { }

	Object* const getSender() {
		return sender;
	}

	bool getCanceled() {
		return canceled;
	}


	void setCanceled(bool canceled) {
		this->canceled = canceled;
	}

	virtual std::type_index getType() {
		return typeIndex;
	}

	virtual void handleEvent(boost::any handler) = 0;

private:
	std::type_index typeIndex;
	Object* const sender;
	bool canceled;

};



#endif /* BASEEVENT_HPP_ */
