/*
 * Object.hpp
 *
 *  Created on: Oct 19, 2013
 *      Author: Dan
 */

#ifndef HANDLER_REGISTRATION_HPP_
#define HANDLER_REGISTRATION_HPP_

#include "Object.hpp"

class HandlerRegistration : public Object {
public:
	virtual ~HandlerRegistration() { }

	virtual void removeHandler() = 0;
};

#endif /* HANDLER_REGISTRATION_HPP_ */
