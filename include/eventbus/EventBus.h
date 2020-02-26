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

#include "EventHandler.h"
#include "RoutedEvent.h"
#include "HandlerCollectionMap.h"


 /**
	* \brief An Event system that allows decoupling of code through synchronous events
	*
	*/
class EventBus final
{
public:
	/**
	 * \brief Default empty constructor
	 */
	EventBus() {}


	/**
	 * \brief Empty virtual destructor
	 */
	virtual ~EventBus() {}

	EventSubscription Add(const SubscriptionDescriptor& descriptor)
	{
		return _collectionMap.Add(descriptor);
	}

	template <typename TEvent>
	void Publish(TEvent& event)
	{
		_collectionMap.Dispatch(event);
	}

	template <typename TEvent>
	EventSubscription Subscribe(std::function<void(TEvent&)> handler, std::function<bool(TEvent&)> predicate)
	{
		const std::function<void(RoutedEvent&)> internalHandler = [=](RoutedEvent& e)
		{
			handler(dynamic_cast<TEvent&>(e));
		};

		const std::function<bool(RoutedEvent&)> internalPredicate = [=](RoutedEvent& e)
		{
			return predicate(dynamic_cast<TEvent&>(e));
		};

		const SubscriptionDescriptor descriptor(typeid(TEvent), internalHandler, internalPredicate);
		return Add(descriptor);
	}

	template <typename TEvent>
	EventSubscription Subscribe(std::function<void(TEvent&)> handler)
	{
		return Subscribe<TEvent>(handler, [](TEvent&) { return true; });
	}

	template <typename TEvent>
	EventSubscription Subscribe(EventHandler<TEvent>& handler)
	{
		return Subscribe<TEvent>([&handler](TEvent& e) {
			handler.On(dynamic_cast<TEvent&>(e));
		}, [](TEvent&) { return true; });
	}


private:
	HandlerCollectionMap _collectionMap;

};
