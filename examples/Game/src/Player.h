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

#include <string>
#include <utility>

class PlayerMoveEvent;

/**
 * \brief Demo class to showcase some of the features of the EventBus
 *
 * This is not part of the core functionality and can be modified or deleted as desired
 */
class Player
{
public:
	Player(std::string name) :
		name(std::move(name)),
		posX(0),
		posY(0),
		posZ(0)
	{
	}

	virtual ~Player()
	{

	}

	[[nodiscard]]
	const std::string& GetName() const
	{
		return name;
	}

	void SetPosition(const int x, const int y, const int z)
	{
		posX = x;
		posY = y;
		posZ = z;
	}

	[[nodiscard]]
	int GetX() const
	{
		return posX;
	}

	[[nodiscard]]
	int GetY() const
	{
		return posY;
	}

	[[nodiscard]]
	int GetZ() const
	{
		return posZ;
	}

private:
	std::string name;
	int posX;
	int posY;
	int posZ;

};
