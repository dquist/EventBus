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

#ifndef _SRC_PLAYER_HPP_
#define _SRC_PLAYER_HPP_

#include "Object.hpp"
//#include "PlayerMoveEvent.hpp"

#include <string>

class PlayerMoveEvent;

/**
 * \brief Demo class to showcase some of the features of the EventBus
 *
 * This is not part of the core functionality and can be modified or deleted as desired
 */
class Player : public Object
{
public:
	Player(std::string name) :
		name(name),
		posX(0),
		posY(0),
		posZ(0)
	{ }

	virtual ~Player() {

	}

	const std::string & getName() {
		return name;
	}

	void setPosition(int x, int y, int z) {
		posX = x;
		posY = y;
		posZ = z;
	}

	int getX() {
		return posX;
	}

	int getY() {
		return posY;
	}

	int getZ() {
		return posZ;
	}

private:
	std::string name;
	int posX;
	int posY;
	int posZ;

};


#endif /* _SRC_PLAYER_HPP_ */
