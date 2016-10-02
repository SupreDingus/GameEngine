///////////////////////////////////////////////////////////////////////////////
//This is the base class for every System that the game engine uses.
//Basically, it virutualizes every common game loop function, such as
//Initialize and Update.
///////////////////////////////////////////////////////////////////////////////

#pragma once

//#include <iostream>
//#include <Vector>
#include "Message.h"
#include <string>

namespace OBALFramework
{
	///System is a pure virtual base class (which is to say, an interface) that is
	///the base class for all systems used by the game. 
  //The Systems class is used only for singleton classes (classes for systems with only 1 instance ever)
	class ISystem   /// Singleton Systems
	{
		public:
		// Systems can receive all message send to the Core.  (Borrowed from sample engine)
		virtual void ThrowMessage(Message* message) {};

		// Initialize the system.
		virtual void Initialize() {};

		// All systems are updated every game frame.
		virtual void Update(float timeslice) = 0;

		// All systems provide a string name for debugging.   (Borrowed from sample engine)
		virtual std::string GetName() = 0;

    // Each system should have "ComponentCreator.h" included so it can register components that are specific
    //  to it (i.e. RegisterComponent(Sprite) and RegisterComponent(Camera) in the constructor of "Graphics.cpp")
    // This is so each unique component type gets its own creator ONCE
    virtual void RegisterAppropriateComponents() {};

		// All systems need a virtual destructor to have their destructor called 
    virtual ~ISystem() {};
	};
}
