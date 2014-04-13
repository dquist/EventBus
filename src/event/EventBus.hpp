/*
 * Copyright (c) 2014, Dan Quist
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef _SRC_EVENT_EVENT_BUS_HPP_
#define _SRC_EVENT_EVENT_BUS_HPP_

#include "Object.hpp"
#include "EventHandler.hpp"
#include "Event.hpp"
#include "HandlerRegistration.hpp"

#include <list>
#include <functional>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include "boost/any.hpp"
#include "boost/function.hpp"



/**
 * \brief An Event system that allows decoupling of code through synchronous events
 *
 *
 */
class EventBus : public Object {
public:
	EventBus() {

	}

	virtual ~EventBus() {

	}

	static EventBus* GetInstance() {
		if (instance == nullptr) {
			instance = new EventBus();
		}

		return instance;
	}

	template <class T>
	static HandlerRegistration* const AddHandler(EventHandler<T>* handler) {
		return AddHandler<T>(handler, nullptr);
	}

	template <class T>
	static HandlerRegistration* const AddHandler(EventHandler<T>* handler, Object* sender) {
		EventBus* instance = GetInstance();

		EventPairs* eventPairs = instance->handlers[typeid(T)];

		if (eventPairs == nullptr) {
			eventPairs = new EventPairs();
			instance->handlers[typeid(T)] = eventPairs;
		}


		EventPair* pair = new EventPair(handler, eventPairs, sender);

		eventPairs->push_back(pair);

		return pair;
	}


	static void FireEvent(Event & e) {
		EventBus* instance = GetInstance();

		EventPairs* eventpairs = instance->handlers[e.getType()];

		if (eventpairs == nullptr) {
			return; // No registered handlers for this event type
		}

		for (auto & pair : *eventpairs) {
			if ((pair->getSender() == nullptr) || (pair->getSender() == e.getSender())) {
				e.handleEvent(pair->getHandler());
			}
		}
	}

private:
	static EventBus* instance;

	class EventPair : public HandlerRegistration
	{
	public:
		typedef std::list<EventPair*> EventPairs;

		EventPair(const boost::any handler, EventPairs * const registrations, Object * const sender ) :
			handler(handler),
			eventPairs(registrations),
			sender(sender),
			registered(true)
		{ }

		boost::any getHandler() { return handler; }
		Object* const getSender() { return sender; }

		virtual void removeHandler() {
			if (registered) {
				eventPairs->remove(this);
				registered = false;
			}
		}

	private:
		const boost::any handler;
		EventPairs* const eventPairs;
		Object* const sender;

		bool registered;
	};

	typedef std::list<EventPair*> EventPairs;
	typedef std::unordered_map<std::type_index, std::list<EventPair*>*> TypeMap;

	TypeMap handlers;

};

#endif /* _SRC_EVENT_EVENT_BUS_HPP_ */
