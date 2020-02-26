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

#include <unordered_map>
#include "EventSubscription.h"
#include <typeindex>

 /**
	  * An unordered map of event types to event subscription collections
	  */
class HandlerCollectionMap final : public std::unordered_map<std::type_index, HandlerCollection*>
{
public:
	HandlerCollectionMap() = default;
	~HandlerCollectionMap() = default;
	HandlerCollectionMap(HandlerCollectionMap&& other) = delete;
	HandlerCollectionMap(const HandlerCollectionMap& other) = delete;
	HandlerCollectionMap& operator=(const HandlerCollectionMap& other) = delete;
	HandlerCollectionMap& operator=(HandlerCollectionMap&& other) = delete;

	template <typename TEvent>
	void Dispatch(TEvent& event)
	{
		const auto collection = (*this)[typeid(TEvent)];
		if (collection != nullptr)
			collection->Dispatch(event);

	}

	EventSubscription Add(const SubscriptionDescriptor& descriptor)
	{
		const auto& type = descriptor.GetType();
		auto collection = (*this)[type];
		if (collection == nullptr)
		{
			collection = new HandlerCollection();
			(*this)[type] = collection;
		}

		const EventSubscription subscription(*collection, descriptor);
		collection->push_back(descriptor);
		return subscription;
	}
};
