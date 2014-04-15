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

#include "EventHandler.hpp"
#include "EventBus.hpp"
#include "Player.hpp"

#include "PlayerMoveEvent.hpp"
#include "PlayerChatEvent.hpp"

#include <cstdio>
#include <string>
#include <cstdlib>



/**
 * \brief Simple example of an event handler class
 *
 * This snippet shows how to implement multiple EventHandlers in a single class
 */
class PlayerListener : public EventHandler<PlayerMoveEvent>, public EventHandler<PlayerChatEvent>
{
public:
	PlayerListener() { }

	virtual ~PlayerListener() { }


	/**
	 * \brief This event handler keeps the player inside a specific border area
	 *
	 * @param e The PlayerMoveEvent event
	 */
	virtual void onEvent(PlayerMoveEvent & e) override {

		// Ignore the event if it's already been canceled
		if (e.getCanceled()) {
			return;
		}

		Player & p = e.getPlayer();

		// Cancel the event if the new player position is outside of the border area
		if (std::abs(p.getX()) > BORDER_SIZE || std::abs(p.getZ()) > BORDER_SIZE) {
			e.setCanceled(true);
			printf("Canceled setting player %s position - outside of border\n", p.getName().c_str());
			return;
		}
	}


	/**
	 * This event handler prints out a debug message whenever a chat event is fired
	 *
	 * @param e The PlayerChatEvent event
	 */
	virtual void onEvent(PlayerChatEvent & e) override {

		// Ignore the event if it's already been canceled
		if (e.getCanceled()) {
			return;
		}

		printf("The player '%s' said: %s\n", e.getPlayer().getName().c_str(), e.getMessage().c_str());
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
		playerMoveReg = nullptr;
		playerChatReg = nullptr;
	}

	virtual ~EventBusDemo() { }


	/**
	 * Demo Function 1
	 *
	 * Registers an event listener on player1 and shows how events can be fired and canceled
	 */
	void Demo1() {

		// Two unique player objects
		Player player1("Player1");
		Player player2("Player2");

		// Declare a local PlayerMoveEvent and use the event bus to fire it
		// There are currently no listeners so this won't actually do anything
		PlayerMoveEvent e(*this, player1, 0, 0, 0);
		EventBus::FireEvent(e);

		// Create the player listener instance
		PlayerListener playerListener;

		// Register the player listener to handler PlayerMoveEvent events
		// Passing player1 as a second parameter means it will only listen for events from that object
		// The return value is a HandlerRegistration pointer that can be used to unregister the event handler
		playerMoveReg = EventBus::AddHandler<PlayerMoveEvent>(playerListener, player1);

		// The playerListener gets registered again, but this time as player chat event handler
		// The lack of a second parameter means that it will service ALL player chat events,
		// regardless of the source
		playerChatReg = EventBus::AddHandler<PlayerChatEvent>(playerListener);


		int x = 0;

		// This loop will attempt to increase the X position of player one
		// by 200 until it reaches 1000 or if the setPosition function fails.

		// The Player.setPosition() method fires a PlayerMoveEvent event internally
		//
		// The PlayerListener class has an event handler that will cancel the
		// PlayerMoveEvent if the X position is greater than 500
		while (x <= 1000) {
			printf("Changing player 1 X to %d\n", x);

			// This method will fail once X > 500 because of the event handler we registered
			if (setPlayerPostionWithEvent(player1, x, 0, 0) == true) {
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
		//
		// This shows how an event handler will handle data from one source while ignoring others
		while (x <= 1000) {
			printf("Changing player 2 X to %d\n", x);
			if (setPlayerPostionWithEvent(player2, x, 0, 0) == true) {
				x += 200;
			} else {
				printf("Setting player 2 position was canceled\n");
				break;
			}
		}


		// Here two chat player chat events are created for each player and fired.
		// Since the chat listener was registered without a source object, it will service
		// all chat events and print both messages
		//
		// The event handler will print out the player name with the message when the event is fired
		PlayerChatEvent chat1(*this, player1, "Hello I am Player 1!");
		EventBus::FireEvent(chat1);

		PlayerChatEvent chat2(*this, player2, "Hello I am Player 2!");
		EventBus::FireEvent(chat2);


		// The HandlerRegistration object can be used to unregister the event listener
		playerChatReg->removeHandler();


		// If a chat event is fired again, it will not be serviced since the handler has been unregistered
		PlayerChatEvent chat3(*this, player2, "This chat message will not be serviced");
		EventBus::FireEvent(chat3);


		// Clean up
		playerMoveReg->removeHandler();
		delete playerMoveReg;
		delete playerChatReg;
	}

private:
	HandlerRegistration* playerMoveReg;
	HandlerRegistration* playerChatReg;


	bool setPlayerPostionWithEvent(Player & player, int x, int y, int z) {

		int savedX = player.getX();
		int savedY = player.getY();
		int savedZ = player.getZ();

		player.setPosition(x, y, z);

		PlayerMoveEvent e(player, player, savedX, savedY, savedZ);
		EventBus::FireEvent(e);

		if (e.getCanceled()) {
			player.setPosition(savedX, savedY, savedZ);
			return false;
		}

		return true;
	}

};


int main()
{
	printf("* * * EventBus Demo Program * * * \n");

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


