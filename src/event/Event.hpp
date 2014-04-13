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

#include "Object.hpp"
#include <typeindex>
#include <typeinfo>
#include <boost/any.hpp>
#include <vector>

#ifndef _SRC_EVENT_EVENT_HPP_
#define _SRC_EVENT_EVENT_HPP_

/**
 * \brief The base event class, all events inherit from this class
 */
class Event : public Object
{
public:
	Event(std::type_index typeIndex, Object* const sender) :
		typeIndex(typeIndex),
		sender(sender),
		canceled(false) {

		// The sender must not be null
		if (sender == nullptr) {
			throw std::runtime_error("Event parameter 'sender' was null.\n");
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



#endif /* _SRC_EVENT_EVENT_HPP_ */
