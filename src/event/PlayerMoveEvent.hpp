/*
 * BaseEvent.hpp
 *
 *  Created on: Oct 19, 2013
 *      Author: Dan
 */

#ifndef _PLAYER_MOVE_EVENT_HPP_
#define _PLAYER_MOVE_EVENT_HPP_

#include "Event.hpp"
#include <string>

class Player;

class PlayerMoveEvent : public Event
{
public:
	PlayerMoveEvent(Object* sender, Player * player, int oldX, int oldY, int oldZ) :
	Event(typeid(PlayerMoveEvent), sender),
	player(player),
	oldX(oldX),
	oldY(oldY),
	oldZ(oldZ) {
	}

	virtual ~PlayerMoveEvent() {

	}


	virtual void handleEvent(boost::any handler) override {
		boost::any_cast<EventHandler<PlayerMoveEvent>*>(handler)->dispatch(this);
	}

	Player * getPlayer() {
		return player;
	}

	int getOldX() {
		return oldX;
	}

	int getOldY() {
		return oldY;
	}

	int getOldZ() {
		return oldZ;
	}

private:
	Player * player;

	int oldX;
	int oldY;
	int oldZ;

};



#endif /* _PLAYER_MOVE_EVENT_HPP_ */
