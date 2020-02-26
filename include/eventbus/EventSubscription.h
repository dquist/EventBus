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

#include "HandlerCollection.h"
#include "SubscriptionDescriptor.h"

 /**
  * The abstract base class for all routed events.
  */
class EventSubscription final
{
public:
	EventSubscription(HandlerCollection& collection, const SubscriptionDescriptor& descriptor) :
		_collection(collection),
		_descriptor(descriptor)
	{
	}

	~EventSubscription() = default;
	EventSubscription(EventSubscription&& other) = delete;
	EventSubscription(const EventSubscription& other) = default;
	EventSubscription& operator=(const EventSubscription& other) = delete;
	EventSubscription& operator=(EventSubscription&& other) = delete;

	/// Unregisters the event subscription
	void Unsubscribe() const
	{
		//_collection.remove(_descriptor); // TODO
	}

private:
	HandlerCollection& _collection;
	const SubscriptionDescriptor& _descriptor;
};
