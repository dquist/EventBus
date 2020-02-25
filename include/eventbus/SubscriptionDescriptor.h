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

#include "RoutedEvent.h"

#include <functional>
#include <stdexcept>

 /**
   * A descriptor for a subscription to an event stream.
   */
class SubscriptionDescriptor final
{
public:
	/**
	 * Creates a new SubscriptionDescriptor instance
	 * @param typeInfo The routed event type
	 * @param handler A function that is called to handle a routed event
	 * @param predicate A function used to test whether the handler should be invoked
	 */
	explicit SubscriptionDescriptor(const type_info& typeInfo, const std::function<void(RoutedEvent&)>& handler, const std::function<bool(RoutedEvent&)>& predicate) :
		_typeInfo(typeInfo),
		_handler(handler),
		_predicate(predicate)
	{
	}

	~SubscriptionDescriptor() = default;

	SubscriptionDescriptor(SubscriptionDescriptor&& other) = delete;
	SubscriptionDescriptor(const SubscriptionDescriptor& other) = delete;
	SubscriptionDescriptor& operator=(const SubscriptionDescriptor& other) = delete;
	SubscriptionDescriptor& operator=(SubscriptionDescriptor&& other) = delete;

	[[nodiscard]]
	const type_info& GetType() const
	{
		return _typeInfo;
	}

	/// Dispatches an event to the 
	template <typename TEvent>
	void Dispatch(TEvent& event)
	{
		if (typeid(event) != _typeInfo)
			throw std::runtime_error("The dispatched event type must match the event descriptor.");

		if (_predicate(event))
			_handler(event);
	}

private:
	const type_info& _typeInfo;
	const std::function<void(RoutedEvent&)>& _handler;
	const std::function<bool(RoutedEvent&)>& _predicate;
};
