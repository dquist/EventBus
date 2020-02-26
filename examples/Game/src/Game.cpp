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

#include "eventbus/EventBus.h"
#include "Player.h"

#include "PlayerMoveEvent.h"
#include "PlayerChatEvent.h"

#include <cstdio>
#include <string>
#include <cstdlib>


bool setPlayerPositionWithEvent(EventBus& eventBus, Player& player, const int x, const int y, const int z)
{
	const int oldX = player.GetX();
	const int oldY = player.GetY();
	const int oldZ = player.GetZ();

	player.SetPosition(x, y, z);

	PlayerMoveEvent e(player, oldX, oldY, oldZ);
	eventBus.Publish(e);

	if (e.IsCanceled())
	{
		player.SetPosition(oldX, oldY, oldZ);
		return false;
	}

	return true;
}


int main()
{
	EventBus eventBus;

	// Two unique player objects
	Player player1("Player1");
	Player player2("Player2");

	// Declare a local PlayerMoveEvent and use the event bus to fire it
	// There are currently no listeners so this won't actually do anything
	PlayerMoveEvent e(player1, 0, 0, 0);
	eventBus.Publish(e);

	static const int BORDER_SIZE = 500;

	eventBus.Subscribe<PlayerMoveEvent>([](PlayerMoveEvent& e)
	{
		// Ignore the event if it's already been canceled
		if (e.IsCanceled())
		{
			return;
		}

		auto& p = e.getPlayer();

		// Cancel the event if the new player position is outside of the border area
		if (std::abs(p.GetX()) > BORDER_SIZE || std::abs(p.GetZ()) > BORDER_SIZE)
		{
			e.SetCanceled(true);
			printf("Canceled setting player %s position - outside of border\n", p.GetName().c_str());
			return;
		}
	}, [](PlayerMoveEvent& e) { return !e.IsCanceled(); });

	eventBus.Subscribe<PlayerChatEvent>([](PlayerChatEvent& e)
	{
		printf("The player '%s' said: %s\n", e.GetPlayer().GetName().c_str(), e.GetMessage().c_str());
	}, [](PlayerChatEvent& e) { return !e.IsCanceled(); });


	int x = 0;

	// This loop will attempt to increase the X position of player one
	// by 200 until it reaches 1000 or if the SetPosition function fails.

	// The Player.SetPosition() method fires a PlayerMoveEvent event internally
	//
	// The PlayerListener class has an event handler that will cancel the
	// PlayerMoveEvent if the X position is greater than 500
	while (x <= 1000)
	{
		printf("Changing player 1 X to %d\n", x);

		// This method will fail once X > 500 because of the event handler we registered
		if (setPlayerPositionWithEvent(eventBus, player1, x, 0, 0) == true)
		{
			x += 200;
		}
		else
		{
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
	while (x <= 1000)
	{
		printf("Changing player 2 X to %d\n", x);
		if (setPlayerPositionWithEvent(eventBus, player2, x, 0, 0) == true)
		{
			x += 200;
		}
		else
		{
			printf("Setting player 2 position was canceled\n");
			break;
		}
	}


	// Here two chat player chat events are created for each player and fired.
	// Since the chat listener was registered without a source object, it will service
	// all chat events and print both messages
	//
	// The event handler will print out the player name with the message when the event is fired
	PlayerChatEvent chat1(player1, "Hello I am Player 1!");
	eventBus.Publish(chat1);

	PlayerChatEvent chat2(player2, "Hello I am Player 2!");
	eventBus.Publish(chat2);

	// If a chat event is fired again, it will not be serviced since the handler has been unregistered
	PlayerChatEvent chat3(player2, "This chat message will not be serviced");
	eventBus.Publish(chat3);
}


