C++11 EventBus
========

A lightweight synchronous event framework for C++11

Overview
--------
Like many other event frameworks, this project allows you to reduce dependencies and decouple your C++11 code by leveraging synchronous events and event handlers.

Unlike mananged languages such as C# and Java, C++ is statically typed and doesn't support reflection or anonymous delegates making event systems slightly more difficult. This project attempts to create a simple and lightweight event framework that can easily be adapted to any C++11 project. The only requirements are a compiler that supports C++11 and access to the standard C++ libraries. It was inspired by the design of several public event APIs such as Google's Web Toolkit and those found in popular game modding frameworks.

The files can be dropped into any existing project as is without any modification or can be easily modified to fit an existing design. One example of a common tweak maybe be replace the included Object base class with another base class that is common to the project. The EventBus requires that all events be fired from an Object type which could be replaced by an already existing class in your project.

The EventBus does perform a few 'unsafe' operations by statically casting to and from void *. This was originally done using the boost::any library to avoid static casting but was changed to void * in the interest of making runtime speed faster and the code more portable. There are many projects that don't include Boost, and the casts are protected through the use of templates which remove the possibility of an incorrect cast.

Usage
------

