#include "OBALCore.h"
#include "Precompiled.h"
#include "WindowsSystem.h"    //(Also includes BaseSystems.h)
#include "Graphics.h"
#include "Factory.h"
#include "InputManager.h"
#include "Physics.h"
//#include "BaseSystems.h"
#include <iostream>
//#include <Vector>

#define USE_RECIPIENTLIST 0

namespace OBALFramework
{
  //A global pointer to the core
  OBALCore* OBAL;


  /////////////////////////////////////////////////////////////////////////////
  /// *****CONSTRUCTOR-DESTRUCTOR***** ///
  /////////////////////////////////////////////////////////////////////////////
  
  OBALCore::OBALCore(const WCHAR* wndtitle, int screenwidth, int screenheight)
  {
    //This sends an string to be displayed in the debug output window
    OutputDebugStringA("CONSTRUCTING: OBALCore\n");
    _LastUpTime = 0;
    _GameActive = true;
    _windowTitle = wndtitle;
    _ScreenWidth = screenwidth;
    _ScreenHeight = screenheight;
    OBAL = this; //Set the global pointer

    _windowTitle = wndtitle;

    /*
#if (USE_RECIPIENTLIST)   // If a RecipientList is being used, create a new MessageRecipientList()
    //Create a new Recipient List for systems that want to Register for Certain messages
    _Recipients = new MessageRecipientList();
#endif // (USE_RECIPIENTLIST)
    */  ///DEPRECATED


    /////////////////////////////////////////////////////////
    //Add self to whatever recipient list *HERE, if necessary
    /////////////////////////////////////////////////////////

    // Create, add, and register all core engine systems
    CreateSystems();

  }
  OBALCore::~OBALCore()
  {
    OutputDebugStringA("DESTRUCTING: OBALCore\n");
    ///DELETION OF THE SYSTEMS IS CALLED VIA WINMAIN.CPP, WHO OWNS THE OBALCORE SYSTEM
    // Delete all dynamically allocated systems
    //DestroySystems();

    //All systems have been deleted at this point.  Delete the RecipientList
    if (_Recipients != NULL)
      delete _Recipients;

  }


  /////////////////////////////////////////////////////////////////////////////
  /// *****SYSTEM INITIALIZATIONS***** ///
  /////////////////////////////////////////////////////////////////////////////

  void OBALCore::CreateSystems()
  {
    /// Create all Singleton systems for the engine
    // WINDOWS
    WINDOWSSYSTEM = new WindowsSystem(_windowTitle, _ScreenWidth, _ScreenHeight);
    // GRAPHICS
    GRAPHICS = new Graphics();
    GRAPHICS->SetWindowProperties(WINDOWSSYSTEM->hWnd, _ScreenWidth, _ScreenHeight);
    // FACTORY
    FACTORY = new GameObjectFactory();
    // INPUT
    INPUTMGR = new InputManager();
    // PHYSICS
    PHYSICS = new Physics();
    // OTHER SYSTEMS TO BE ADDED...
    //FMOD = new FmodSoundSystem();
    //GAMESESSION = new GameSession();


    // Add the systems in the appropriate Update order
    AddSystem(WINDOWSSYSTEM);
    AddSystem(GRAPHICS);
    AddSystem(FACTORY);
    AddSystem(INPUTMGR);
    AddSystem(PHYSICS);
    // ...
    //AddSystem(FMOD);
    //AddSystem(GAMESESSION);


    // Register Appropriate Systems so they can recieve windows messages
    RegisterSystemForMessageBroadcast(GRAPHICS);
    RegisterSystemForMessageBroadcast(FACTORY);
    RegisterSystemForMessageBroadcast(PHYSICS);
  }

  // Initializes all of the systems that have been added to the OBALCore
  void OBALCore::Initialize()
  {
    OutputDebugStringA("INITIALIZING: OBALCore\n");
    
    for (unsigned int i = 0; i < _Systems.size(); ++i)
      _Systems[i]->RegisterAppropriateComponents();

    for (unsigned int j = 0; j < _Systems.size(); ++j)
      _Systems[j]->Initialize();

    // Activate and show the window for the game
    WINDOWSSYSTEM->ActivateWindow();

  }

  /////////////////////////////////////////////////////////////////////////////
  /// *****GAME LOOP***** ///
  /////////////////////////////////////////////////////////////////////////////

  //This method loops every frame tells all systems update
  //Once a QUIT message has been sent to the OBALCore, it will stop
  void OBALCore::GameLoop()
  {
    //Initialize the last time variable so our first frame
    //is "zero" seconds (and not some huge unknown number)
    _LastUpTime = timeGetTime();

    // While the game is not being terminated...
    while (_GameActive)
    {
      //Get the current time in milliseconds
      unsigned currenttime = timeGetTime();
      //Convert it to the time passed since the last frame (in seconds)
      float dt = (currenttime - _LastUpTime) / 1000.0f;
      //Update when the last update started
      _LastUpTime = currenttime;

      //Update every system.  
      //Tell each one how much time has passed since the last update
      for (unsigned i = 0; i < _Systems.size(); ++i)
        _Systems[i]->Update(dt);
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  /// *****SYSTEMS HANDLING***** ///
  /////////////////////////////////////////////////////////////////////////////

  /// Adds a system to the OBALCore's Systems Vector
  void OBALCore::AddSystem(ISystem * system)
  {
    //Add a system to the core to be updated
    _Systems.push_back(system);
  }


  /// Destorys all systems the engine is holding in reverse order to 
  /// minimize dependency problems between systems
  void OBALCore::DestroySystems()
  {
    // Delete systems in their appropriate order
    // ...
    delete PHYSICS;   // Delete Last
    delete INPUTMGR;
    delete FACTORY;
    delete GRAPHICS;
    delete WINDOWSSYSTEM;   // Delete Last
    // ...
  }


  /////////////////////////////////////////////////////////////////////////////
  /// *****SYSTEMS MESSAGING AND BROADCASTING***** ///
  /////////////////////////////////////////////////////////////////////////////

  void OBALCore::RegisterSystemForMessageBroadcast(ISystem * system)
  {
    _MessageRecipients.push_back(system);
  }

  void OBALCore::BroadcastMessage(Message* message)
  {
    // The message that tells the game to quit
    if (message->MessageId == Mid::Quit)
      _GameActive = false;
    // Check for other messages as needed
    // ...


    //Send the message to every system--each
    //system can figure out whether it cares
    //about a given message or not
    for (unsigned i = 0; i < _Systems.size(); ++i)
      _Systems[i]->ThrowMessage(message);
  }



  /*
  /// Registers the passed system to the Recipient Vector that it wants
  /// to be registered into
  void OBALCore::RegisterRecipient(ISystem * system, MGID id)
  {
    Recipients->AddRecipient(system, id);
  }

  /// Sends the message to the appropriate Recipient Vector
  void OBALCore::BroadcastRecipientMessage(Message* message)
  {
  OutputDebugStringA("BROADCASTING MESSAGE: OBALCore\n");
  //Used to point to the vector that points to the registered recipients
  //for "message"
  std::vector<ISystem*> * RList = 0;

  OutputDebugStringA("PICKING ");
  // Sort through and find what code to run for the message given
  switch (message->MessageId)
  {
  case Mid::None:
  {
  OutputDebugStringA("None RECIPIENTS: OBALCore\n");
  //Pick the vector for the None message Recipients
  RList = &_Recipients->None_Recipients;
  break;
  }

  case Mid::All:
  {
  OutputDebugStringA("All RECIPIENTS: OBALCore\n");
  //Pick the vector for the None message Recipients
  RList = &_Recipients->All_Recipients;
  break;
  }

  case Mid::Quit: //A character key was pressed
  {
  OutputDebugStringA("Quit RECIPIENTS: OBALCore\n");
  //A quit message has been passed
  //The game will now commence shutting down
  _GameActive = false;

  //Pick the vector for the Quit message Recipients
  RList = &_Recipients->Quit_Recipients;
  break;
  }

  case Mid::Collision:
  {
  OutputDebugStringA("Collision RECIPIENTS: OBALCore\n");
  //Pick the vector for the None message Recipients
  RList = &_Recipients->Collision_Recipients;
  break;
  }

  case Mid::ToggleDebugInfo:
  {
  OutputDebugStringA("ToggleDebugInfo RECIPIENTS: OBALCore\n");
  //Pick the vector for the None message Recipients
  RList = &_Recipients->DebugInfo_Recipients;
  break;
  }

  case Mid::CharacterKey:
  {
  OutputDebugStringA("CharacterKey RECIPIENTS: OBALCore\n");
  //Pick the vector for the None message Recipients
  RList = &_Recipients->CharKey_Recipients;
  break;
  }

  case Mid::HeldKey:
  {
  OutputDebugStringA("HeldKey RECIPIENTS: OBALCore\n");
  //Pick the vector for the None message Recipients
  RList = &_Recipients->HeldKey_Recipients;
  break;
  }

  case Mid::MouseButton:
  {
  OutputDebugStringA("MouseButton RECIPIENTS: OBALCore\n");
  //Pick the vector for the None message Recipients
  RList = &_Recipients->MouseButton_Recipients;
  break;
  }

  case Mid::MouseMove:
  {
  OutputDebugStringA("MouseMove RECIPIENTS: OBALCore\n");
  //Pick the vector for the None message Recipients
  RList = &_Recipients->MouseMove_Recipients;
  break;
  }

  case Mid::FileDrop:
  {
  OutputDebugStringA("FileDrop RECIPIENTS: OBALCore\n");
  //Pick the vector for the None message Recipients
  RList = &_Recipients->FileDrop_Recipients;
  break;
  }
  } /// END switch (message->MessageId)

  OutputDebugStringA("THROWING TO REGISTERED RECIPIENTS OF MESSAGE: OBALCore\n");
  //Send "message" to all Registered Recipients for this message
  if ((*RList).size() > 0)
  {
  for (unsigned i = 0; i < (*RList).size(); ++i)
  (*RList)[i]->ThrowMessage(message);
  }

  }
  */

}