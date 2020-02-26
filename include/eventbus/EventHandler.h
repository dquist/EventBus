/*
 * Copyright (c) 2020, Dan Quist
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

#pragma once

#include <typeinfo>
#include <type_traits>

#include "RoutedEvent.h"

 /**
  * \brief Base class of all classes that listen for events
  *
  * For a class to be an event listener, it needs to inherit from EventHandler
  * with the specific event type as the template parameter. A class can inherit from
  * multiple EventHandler base classes each using a different template parameter.
  */
template <class TEvent>
class EventHandler
{
public:

	/**
	 * \brief Default constructor that enforces the template type
	 */
	EventHandler()
	{
		// An error here indicates you're trying to implement EventHandler with a type that is not derived from Event
		static_assert(std::is_base_of<RoutedEvent, TEvent>::value, "EventHandler<TEvent>: TEvent must be a class derived from Event");
	}


	/**
	 * \brief Empty virtual destructor
	 */
	virtual ~EventHandler() {}


	/**
	 * \brief Pure virtual method for implementing the body of the listener
	 *
	 * @param e The event instance
	 */
	virtual void On(TEvent& e) = 0;
};

