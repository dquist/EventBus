/*
 * BaseEvent.hpp
 *
 *  Created on: Oct 19, 2013
 *      Author: Dan
 */


#include <string>

#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include "Object.hpp"
#include "PlayerMoveEvent.hpp"

class Player : public Object
{
public:
	Player(std::string name) :
		name(name),
		posX(0),
		posY(0),
		posZ(0)
	{

	}

	virtual ~Player() {

	}

	const std::string & getName() {
		return name;
	}

	bool setPosition(int x, int y, int z) {

		int savedX = posX;
		int savedY = posY;
		int savedZ = posZ;

		posX = x;
		posY = y;
		posZ = z;

		PlayerMoveEvent e(this, this, savedX, savedY, savedZ);
		EventBus::FireEvent(e);

		if (e.getCanceled()) {
			posX = savedX;
			posY = savedY;
			posZ = savedZ;
			return false;
		}

		return true;
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


#endif /* _PLAYER_HPP_ */
