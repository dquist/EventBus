# C++11 EventBus

A lightweight synchronous event framework for C++11

## Overview
Like many other event frameworks, this project allows you to reduce dependencies and decouple your C++11 code by leveraging synchronous events and event handlers.

Unlike mananged languages such as C# and Java, C++ is statically typed and doesn't support reflection or anonymous delegates making event systems slightly more difficult. This project attempts to create a simple and lightweight event framework that can easily be adapted to any C++11 project. The only requirements are a compiler that supports C++11 and access to the standard C++ libraries. It was inspired by the design of several public event APIs such as in Google's Web Toolkit and those found in popular game modding frameworks.

The files can be dropped into any existing project as is without any modification or can be easily modified to fit an existing design. One example of a common tweak maybe be replace the included Object base class with another base class that is common to the project. The EventBus requires that all events be fired from an Object type which could be replaced by an already existing class in your project.

The EventBus does perform a few 'unsafe' operations by statically casting to and from void *. This was originally done using the boost::any library to avoid static casting but was changed to void * in the interest of making runtime speed faster and the code more portable. There are many projects that don't include Boost, and the casts are protected through the use of templates which remove the possibility of an incorrect cast.

## Source Files
**Core Files**
* /src/event/Event.hpp
* /src/event/EventBus.cpp
* /src/event/EventBus.hpp
* /src/event/EventHandler.hpp
* /src/event/HandlerRegistration.hpp
* /src/event/Object.hpp

**Example Files**

These are included for example only and can be deleted when using the framework.
* /src/Main.cpp
* /src/Player.hpp
* /src/event/PlayerChatEvent.hpp
* /src/event/PlayerMoveEvent.hpp

## Usage
### Firing an Event

The event bus is designed so that each event type is a unique class that inherits from the *Event* base class. This means that each application will need to create the event classes that are desired. There are two example event classes included in the project, *PlayerChatEvent* and *PlayerMoveEvent*. These are two events that you might commonly see in game development framework. Below you can observe the syntax for creating and firing a player chat event.

    Player player1("Player 1", 0, 0, 0); // Player instance with a name and default position
    PlayerChatEvent e(this, &player1, "This is a chat message"); // Create the event object
    EventBus::FireEvent(e); // Fire the event
    
The PlayerChatEvent takes three parameters: the event sender or source, a reference to the player that is chatting, and then the message text. The act of firing an event is simply to declare an event object and pass it to the static *FireEvent* method. When *FireEvent* is called, it will service all the event handlers for that event type before returning. This synchronous event-based programming is extremely useful in decoupling inter-class dependencies in large code bases

### Creating an Event Handler

For events to be useful there must be something listening for the events that get fired; that's where the EventHandler class comes in. To listen for a particular event, you must have a class that inherits from the EventHandler class and implements the virtual method for that event type. Below is a class that can handle player chat events.

    class PlayerListener : public EventHandler<PlayerChatEvent>
    {
    public:
      virtual void onEvent(PlayerChatEvent* e) override {
        std::cout << "The player " << e->getPlayer()->getName() << " said " << e->getMessage();
    }

The first point is that the *PlayerListener* class inherits from *EventHandler* and uses the template parameter of the player chat event which is the event type that it will listen for. *EventHandler* is a template class and must always be qualified with the type of event that is being targeted. This makes it possible for a single class to listen for multiple events. The class simply needs to inherit from *EventHandler* multiple times, each with a different template parameter.

The second point is the virtual function. All event handler functions must be called *onEvent* with a single parameter that is a pointer to the event type. The *override* keyword is introduced in C++11 and tells the compiler that you are intending to override an existing virtual function and will throw and error if the function signature does not match an existing virtual function.

### Registering and Unregistering an Event Handler


### Creating a New Event
