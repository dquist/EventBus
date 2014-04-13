/*
 * main.cpp
 *
 *  Created on: Oct 19, 2013
 *      Author: Dan
 */

#include "EventHandler.hpp"
#include "EventBus.hpp"
#include "PlayerMoveEvent.hpp"
#include "Player.hpp"

#include <cstdio>
#include <string>
#include <cstdlib>



class PlayerListener : public EventHandler<PlayerMoveEvent>
{
public:
	PlayerListener() {

	}

	virtual ~PlayerListener() { }

	/**
	 * This event handler keeps the player inside a specific border area
	 *
	 * @param e The PlayerMoveEvent event
	 */
	virtual void onEvent(PlayerMoveEvent* e) override {
		Player* p = e->getPlayer();

		// Cancel the event if the new player position is outside of the border area
		if (std::abs(p->getX()) > BORDER_SIZE || std::abs(p->getZ()) > BORDER_SIZE) {
			e->setCanceled(true);
			printf("Canceled setting player %s position - outside of border\n", p->getName().c_str());
			return;
		}
	}

private:
	static const int BORDER_SIZE = 500;

};



/**
 * \brief Demo class showing off some functionality of the EventBus
 */
class EventBusDemo : public Object
{
public:
	EventBusDemo() {
		playerListener = nullptr;
		playerListenerReg = nullptr;
	}

	virtual ~EventBusDemo() { }



	/**
	 * Demo Function 1
	 *
	 * Registers an event listener on player1 and shows how it can cancel events
	 */
	void Demo1() {

		// Two unique player objects
		Player player1("Player1");
		Player player2("Player2");

		// Create the player listener instance
		playerListener = new PlayerListener();

		// Register the player listener to listen for PlayerMoveEvent events
		// Passing player1 as a second parameter means it will only listen for events from that object
		// The return value is a HandlerRegistration pointer that can be used to unregister the event listener
		// Alternatively, you could have it listen for ALL players by omitting the second parameter
		playerListenerReg = EventBus::AddHandler<PlayerMoveEvent>(playerListener, &player1);

		int x = 0;

		// This loop will attempt to increase the X position of player one
		// by 200 until it reaches 1000 or if the setPosition function fails.
		//
		// The PlayerListener class has an event handler that will cancel the
		// PlayerMoveEvent if the X position is greater than 500
		while (x <= 1000) {
			printf("Changing player 1 X to %d\n", x);
			if (player1.setPosition(x, 0, 0) == true) {
				x += 200;
			} else {
				printf("Setting player 1 position was canceled\n");
				break;
			}
		}

		x = 0;

		// This loop does the same thing as the loop above, just with player2.
		// Since we only registered the PlayerListener to player1, the bounds
		// checking will have no effect for this loop
		while (x <= 1000) {
			printf("Changing player 2 X to %d\n", x);
			if (player2.setPosition(x, 0, 0) == true) {
				x += 200;
			} else {
				printf("Setting player 2 position was canceled\n");
				break;
			}
		}

		// Clean up
		playerListenerReg->removeHandler();
		delete playerListenerReg;
		delete playerListener;
	}

private:
	PlayerListener* playerListener;
	HandlerRegistration* playerListenerReg;

};


int main()
{
	printf("Hello world!\n");

	try
	{
		EventBusDemo demo;
		demo.Demo1();
	}
	catch (std::runtime_error & e)
	{
		printf("Runtime exception: %s\n", e.what());
	}
}


