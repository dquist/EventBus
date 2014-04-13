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

#ifndef _SRC_EVENT_EVENT_HANDLER_HPP_
#define _SRC_EVENT_EVENT_HANDLER_HPP_

#include "Object.hpp"

#include <typeinfo>

// Forward declare the Event class
class Event;

/**
 * \brief Base class of all classes that listen for events
 *
 * For a class to be an event listener, it needs to inherit from EventHandler
 * with the specific event type as the template parameter. A class can inherit from
 * multiple EventHandler base classes each using a different template parameter.
 */
template <class T>
class EventHandler {
public:

	/**
	 * \brief Empty default constructor
	 */
	EventHandler() { }


	/**
	 * \brief Empty virtual destructor
	 */
	virtual ~EventHandler() { }


	/**
	 * \brief Pure virtual method for implementing the body of the listener
	 *
	 * @param The event instance
	 */
	virtual void onEvent(T*) = 0;


	/**
	 * \brief Dispatches a generic event to the appropriate listener method
	 *
	 * This method is called by the EventBus and dispatches to the correct method by
	 * dynamic casting the event parameter to the template type for this handler.
	 *
	 * @param e The event to dispatch
	 */
	void dispatch(Event* e) {
		onEvent(dynamic_cast<T*>(e));
	}
};

#endif /* _SRC_EVENT_EVENT_HANDLER_HPP_ */
