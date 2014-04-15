# C++11 EventBus

**A lightweight synchronous event framework for C++11**

## Overview
Like many other event frameworks, this project allows you to reduce inter-class dependencies and decouple your C++11 code by leveraging synchronous events and event handlers.

Unlike managed languages such as C# and Java, C++ is statically typed and doesn't support reflection or anonymous delegates making event systems slightly more difficult. This project attempts to create a simple and lightweight event framework that can easily be adapted to any C++11 project. The only requirements are a compiler that supports C++11 and access to the standard C++ libraries. It was inspired by the design of several public event APIs such as in Google's Web Toolkit and those found in popular game modding frameworks.

The files can be dropped into any existing project as is without any modification or can be easily modified to fit an existing design. One example of a common tweak would be to replace the included Object base class with another base class that is common to the project. The EventBus requires that all events be fired from an Object type which could be replaced by an already existing class in your project.

The EventBus does perform a few 'unsafe' operations by statically casting to and from void *. This was originally done using the boost::any library to avoid static casting but was changed to void * in the interest of making runtime speed faster and the code more portable. There are many projects that don't include Boost, and the casts are protected through the use of templates which remove the possibility of an incorrect cast.

## Source Files
**Core Files**
* */src/event/Event.hpp*
* */src/event/EventBus.cpp*
* */src/event/EventBus.hpp*
* */src/event/EventHandler.hpp*
* */src/event/HandlerRegistration.hpp*
* */src/event/Object.hpp*

**Example Files**

These are included for example only and can be deleted when using the framework.
* */src/Main.cpp*
* */src/Player.hpp*
* */src/event/PlayerChatEvent.hpp*
* */src/event/PlayerMoveEvent.hpp*

## Usage
### Firing an Event

The event bus is designed so that each event type is a unique class that inherits from the *Event* base class. This means that you will need to create the event classes that are specific to your application. There are two example event classes included in the project, *PlayerChatEvent* and *PlayerMoveEvent*. These are two events that you might commonly see in a game development framework. Below you can observe the syntax for creating and firing a player chat event.

```c++
Player player1("Player 1"); // Player instance with a name
PlayerChatEvent e(*this, player1, "This is a chat message"); // Create the event object
EventBus::FireEvent(e); // Fire the event
```

The PlayerChatEvent takes three parameters: the event sender or source, a reference to the player that is chatting, and then the message text. The act of firing an event is simply to declare an event object and pass it to the static *FireEvent* method. When *FireEvent* is called, it will service all the event handlers for that event type before returning. This synchronous event-based programming is extremely useful in decoupling inter-class dependencies in large code bases.

### Creating an Event Handler

For events to be useful there must be something listening for the events that get fired; that's where the *EventHandler* class comes in. To listen for a particular event, you must have a class that inherits from the *EventHandler* class and implements the virtual method for that event type. Below is a class that will listen for player chat events and print out the chat message with the player name.

```c++
class PlayerListener : public EventHandler<PlayerChatEvent>
{
public:
  virtual void onEvent(PlayerChatEvent & e) override {
    // Print out the name of the player and the chat message
    std::cout << "The player '" << e.getPlayer().getName() << "' said " << e.getMessage();
  }
};
```

The *PlayerListener* class inherits from *EventHandler* and uses the template parameter of the player chat event. This is the event type that it will listen for. *EventHandler* is a template class and must always be qualified with the type of event that is being targeted. This makes it possible for a single class to listen for multiple events. The class simply needs to inherit from *EventHandler* multiple times, each with a different template parameter.

All event handler functions must be virtual and called *onEvent* with a single parameter - a reference to the handled event type. The *override* keyword is new in C++11 and tells the compiler that it intends to override an existing virtual function. The compiler will throw an error if the function signature does not match an existing virtual function from an inherited class.

### Registering and Unregistering an Event Handler

The last part is to register the event handler with the event bus using the *AddHandler* method as you can see below.

```c++
// Create an instance of PlayerListener and register it with the event bus
PlayerListener pListener;
HandlerRegistration* reg = EventBus::AddHandler<PlayerChatEvent>(pListener);
```
    
Now the pListener object has been registered and the *onEvent* will be invoked every time a player chat event is fired. The template parameter is again necessary so that the same listener class can be registered as multiple event handlers; this removes any ambiguity as to which base class is being referenced.

The registered class can also be unregistered by using the returned *HandlerRegistration* object. Once a handler is unregistered, it will no longer recieve events of that type.

```c++
// Unregister the listener and delete the registration object
reg->removeHandler();
delete reg;
```

The *AddHandler* method also has an optional 2nd parameter that can specify a desired event source. Providing an event source during registration means that event handler will only be invoked when the event is fired from that specified source object.

### Creating a Custom Event

Creating new event classes is easy - just make a new class that inherits from the base *Event* class, implement the constructor and add custom fields and methods. This is what an empty event class looks like without any custom fields or methods.

```c++
class MyCustomEvent : public Event
{
public:
  MyCustomEvent(Object & sender) :
  Event(sender) {
  }
  
  virtual ~MyCustomEvent() { }
};
```

The constructor for the *Event* base class requires the event sender as a parameter, so at a minimum your event must have at least one parameter. Beyond this shell class, any custom fields or methods can be added as desired. 


## Conclusion

The above snippets are very top level and the source files, notably Main.cpp, should be examined for a more complete and in-depth example.

It is quite common to have many event classes in a large application; there are some game frameworks that have hundreds of individual event classes, each in its own file. Although it is easy to make new events, it is useful to consider whether some event types could share the same class if they have similar attributes. For example, a common example is to have an event that simply states that something has happened but doesn't have any data associated with it. In that case, there could be a single status event class with an enumerated list of possible status values. This event could then be used for every type of status flag in the system instead of creating an entirely new event type.


