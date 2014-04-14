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

#ifndef _SRC_EVENT_EVENT_HPP_
#define _SRC_EVENT_EVENT_HPP_

#include "Object.hpp"

#include <typeindex>
#include <typeinfo>
#include <vector>
#include <stdexcept>

/**
 * \brief The base event class, all events inherit from this class
 */
class Event : public Object
{
public:
	/**
	 * \brief Default constructor
	 *
	 * @param typeIndex The type ID of the inherited class
	 * @param sender The sender of the event
	 */
	Event(Object* const sender) :
		sender(sender),
		canceled(false) {

		// The sender must not be null
		if (sender == nullptr) {
			throw std::runtime_error("Event parameter 'sender' was null.\n");
		}
	}


	/**
	 * \brief Empty virtual destructor
	 */
	virtual ~Event() { }


	/**
	 * \brief Gets the source object for this event
	 *
	 * @return The event sender
	 */
	Object* const getSender() {
		return sender;
	}


	/**
	 * \brief Gets whether the event has been canceled
	 *
	 * @return true if the event is canceled
	 */
	bool getCanceled() {
		return canceled;
	}


	/**
	 * \brief Sets the canceled status for the event
	 *
	 * @param canceled Whether the even is canceled or not
	 */
	void setCanceled(bool canceled) {
		this->canceled = canceled;
	}

private:
	Object* const sender;
	bool canceled;

};

#endif /* _SRC_EVENT_EVENT_HPP_ */
