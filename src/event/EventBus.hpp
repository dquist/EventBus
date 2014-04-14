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
#include <typeinfo>
#include <unordered_map>


/**
 * \brief An Event system that allows decoupling of code through synchronous events
 *
 */
class EventBus : public Object {
public:
	/**
	 * \brief Default empty constructor
	 */
	EventBus() { }


	/**
	 * \brief Empty virtual destructor
	 */
	virtual ~EventBus() { }


	/**
	 * \brief Returns the EventBus singleton instance
	 *
	 * Creates a new instance of the EventBus if hasn't already been created
	 *
	 * @return The singleton instance
	 */
	static EventBus* const GetInstance() {
		if (instance == nullptr) {
			instance = new EventBus();
		}

		return instance;
	}


	/**
	 * \brief Registers a new event handler to the EventBus with a source specifier
	 *
	 * The template parameter is the specific type of event that is being added. Since a class can
	 * potentially inherit multiple event handlers, the template specifier will remove any ambiguity
	 * as to which handler pointer is being referenced.
	 *
	 * @param handler The event handler class
	 * @param sender The source sender object
	 * @return An EventRegistration pointer which can be used to unregister the event handler
	 */
	template <class T>
	static HandlerRegistration* const AddHandler(EventHandler<T>* const handler, Object* const sender) {
		EventBus* instance = GetInstance();

		// Fetch the list of event pairs unique to this event type
		Registrations* registrations = instance->handlers[typeid(T)];

		// Create a new collection instance for this type if it hasn't been created yet
		if (registrations == nullptr) {
			registrations = new Registrations();
			instance->handlers[typeid(T)] = registrations;
		}

		// Create a new EventPair instance for this registration.
		// This will group the handler, sender, and registration object into the same class
		EventRegistration* registration = new EventRegistration(static_cast<void*>(handler), registrations, sender);

		// Add the registration object to the collection
		registrations->push_back(registration);

		return registration;
	}


	/**
	 * \brief Registers a new event handler to the EventBus with no source specified
	 *
	 * @param handler The event handler class
	 * @return An EventRegistration pointer which can be used to unregister the event handler
	 */
	template <class T>
	static HandlerRegistration* const AddHandler(EventHandler<T>* const handler) {
		return AddHandler<T>(handler, nullptr);
	}


	/**
	 * \brief Fires an event
	 *
	 * @param e The event to fire
	 */
	static void FireEvent(Event & e) {
		EventBus* instance = GetInstance();

		Registrations* registrations = instance->handlers[typeid(e)];

		// If the registrations list is null, then no handlers have been registered for this event
		if (registrations == nullptr) {
			return;
		}

		// Iterate through all the registered handlers and dispatch to each one if the sender
		// matches the source or if the sender is not specified
		for (auto & reg : *registrations) {
			if ((reg->getSender() == nullptr) || (reg->getSender() == e.getSender())) {

				// This is where some magic happens. The void * handler is statically cast to an event handler
				// of generic type Event and dispatched. The dispatch function will then do a dynamic
				// cast to the correct event type so the matching onEvent method can be called
				static_cast<EventHandler<Event>*>(reg->getHandler())->dispatch(&e);
			}
		}
	}


private:
	// Singleton class instance
	static EventBus* instance;


	/**
	 * \brief Registration class private to EventBus for registered event handlers
	 */
	class EventRegistration : public HandlerRegistration
	{
	public:
		typedef std::list<EventRegistration*> Registrations;


		/**
		 * \brief Represents a registration object for a registered event handler
		 *
		 * This object is stored in a collection with other handlers for the event type.
		 *
		 * @param handler The event handler
		 * @param registrations The handler collection for this event type
		 * @param sender The registered sender object
		 */
		EventRegistration(void * const handler, Registrations * const registrations, Object * const sender ) :
			handler(handler),
			registrations(registrations),
			sender(sender),
			registered(true)
		{ }


		/**
		 * \brief Empty virtual destructor
		 */
		virtual ~EventRegistration() { }


		/**
		 * \brief Gets the event handler for this registration
		 *
		 * @return The event handler
		 */
		void * const getHandler() {
			return handler;
		}


		/**
		 * \brief Gets the sender object for this registration
		 *
		 * @return The registered sender object
		 */
		Object* const getSender() {
			return sender;
		}


		/**
		 * \brief Removes an event handler from the registration collection
		 *
		 * The event handler will no longer receive events for this event type
		 */
		virtual void removeHandler() {
			if (registered) {
				registrations->remove(this);
				registered = false;
			}
		}

	private:
		void * const handler;
		Registrations* const registrations;
		Object* const sender;

		bool registered;
	};

	typedef std::list<EventRegistration*> Registrations;
	typedef std::unordered_map<std::type_index, std::list<EventRegistration*>*> TypeMap;

	TypeMap handlers;

};

#endif /* _SRC_EVENT_EVENT_BUS_HPP_ */
