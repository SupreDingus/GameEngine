#include "Precompiled.h"
#include "InputManager.h"
#include "WindowsSystem.h"

#define INDICATEREGISTERDKEYS 0
#define INDICATEUNREGISTERDKEYS 0

namespace OBALFramework
{
  //Our global pointer to Graphics.
  InputManager * INPUTMGR = NULL;


  /////////////////////////////////////////////////////////////////////////////
  /// *****CONSTRUCTOR-DESTRUCTOR***** ///
  /////////////////////////////////////////////////////////////////////////////
  InputManager::InputManager()
  {
    INPUTMGR = this;
    // OBAL will tell this system to initialize
  }
  InputManager::~InputManager()
  {
    _DeleteKeyStates();
  }


  /////////////////////////////////////////////////////////////////////////////
  /// *****SYSTEM INITIALIZATIONS***** ///
  /////////////////////////////////////////////////////////////////////////////
  void InputManager::Initialize()
  {
    /*'A' Key*/
    AddKeyStateObject(KEY_A);
    /*'S' Key*/
    AddKeyStateObject(KEY_S);
    /*'D' Key*/
    AddKeyStateObject(KEY_D);
    /*'W' Key*/
    AddKeyStateObject(KEY_W);
    /*'SPACE' Key*/
    AddKeyStateObject(VK_SPACE);
    /*'LSHIFT' Key*/
    AddKeyStateObject(VK_LSHIFT);
    /*'RSHIFT' Key*/
    AddKeyStateObject(VK_RSHIFT);
    /*'LCONTROL' Key*/
    AddKeyStateObject(VK_LCONTROL);
    /*'RSHIFT' Key*/
    AddKeyStateObject(VK_RCONTROL);
    /*'TAB' Key*/
    AddKeyStateObject(VK_TAB);
    /*'ESC' Key*/
    AddKeyStateObject(VK_ESCAPE);
    /*'UP' Key*/
    AddKeyStateObject(VK_UP);
    /*'DOWN' Key*/
    AddKeyStateObject(VK_DOWN);
    /*'LEFT' Key*/
    AddKeyStateObject(VK_LEFT);
    /*'RIGHT' Key*/
    AddKeyStateObject(VK_RIGHT);
    
  }


  /////////////////////////////////////////////////////////////////////////////
  /// *****SYSTEM UPDATE***** ///
  /////////////////////////////////////////////////////////////////////////////
  ///  THIS CAN BE OPTIMIZED.  THIS CHECKS EACH KEY EVERY FRAME FOR IF IT PRESSED OR RELEASED NEEDS TO BE FALSIFIED
  void InputManager::Update(float dt)
  {
    KeyStateMap::iterator it = _InputtedKeys.begin();
    for (; it != _InputtedKeys.end(); ++it)
    {
      // If Pressed is true, Down is true as well.
      // Set Pressed to false so any executions dependent upon one-time key press actions are only activated once.
      if (it->second->Pressed == true)
      {
        it->second->Pressed = false;
      }
      // Similarly, do the same with Released, for Up is true already
      if (it->second->Released == true)
      {
        it->second->Released = false;
      }
    }

  }


  /////////////////////////////////////////////////////////////////////////////
  /// *****SYSTEM MESSAGING***** ///
  /////////////////////////////////////////////////////////////////////////////
  void InputManager::ThrowMessage(Message * message)
  {
    OutputDebugStringA("MESSAGE THROWN TO THIS RECIPIENT: Input Manager\n");


    // If any key is pressed, check to see if it has a KeyState object created for it.
    // Set the pressed booleans.
    if (message->MessageId == Mid::Key)
    {
      OutputDebugStringA("KEY PRESSED: Input Manager\n");

      // The user has pressed a key.
      Message_Key * pressedkey = (Message_Key*)message;

      // Iterate through all existing KeyState Objects
      KeyStateMap::iterator it = _InputtedKeys.begin();
      for (; it != _InputtedKeys.end(); ++it)
      {
        // Check to make sure the key exists first and that it hasn't been held down
        if (it->first == pressedkey->key && it->second->Down == false)
        {
          // The Key is no longer down
          it->second->Down = true;
          it->second->Pressed = true;
          // The Key is unpressed now
          it->second->Released = false;
          it->second->Up = false;
          return;
        }
      }

      
#if(INDICATEUNREGISTERDKEYS == 1)
      // The key doesn't exist if the iterator gets to the end() of the _InputtedKeys map
      // Thus, a new KeyState Object must be created.
      ///  NOTE:KeyState Object are dynamically created, meaning they aren't created until the first time
      ///       the key is pressed.
      WINDOWSSYSTEM->Request_ERRORMessageBox("A key was pressed, but there was no KeyState Object for it.","KeyState Object Doesn't Exist.");
#endif

    }
    // If a key is released, set the existing key's release booleans.  Throw an error message if this message
    // is recieved without an existing KeyState object
    if (message->MessageId == Mid::KeyUp)
    {
      OutputDebugStringA("KEY RELEASED: Input Manager\n");
      // The user has pressed a key, we may respond by creating
      // a specific object based on the key pressed.
      Message_KeyUp * uppedkey = (Message_KeyUp*)message;

      KeyStateMap::iterator it = _InputtedKeys.begin();
      for (; it != _InputtedKeys.end(); ++it)
      {
        if (it->first == uppedkey->keyup)
        {
          // The Key is no longer down
          it->second->Down = false;
          it->second->Pressed = false;
          // The Key is unpressed now
          it->second->Released = true;
          it->second->Up = true;
          return;
        }
      }

      /*
      // When the KeyState is found, this should not be reached
      // If it is, there is a missing KeyState Object
      WINDOWSSYSTEM->Request_ERRORMessageBox("A key was released, but there was no KeyState Object for it.", "Key Release With No KeyState");
      */
    }
  }


  /////////////////////////////////////////////////////////////////////////////
  /// *****COMPONENT REGISTRATION***** ///
  /////////////////////////////////////////////////////////////////////////////
  void InputManager::RegisterAppropriateComponents()
  {
  }


  /////////////////////////////////////////////////////////////////////////////
  /// *****KEY STATE ADDING***** ///
  /////////////////////////////////////////////////////////////////////////////
  void InputManager::AddKeyStateObject(unsigned int key)
  {
    KeyState * newkeystate;
    if (_InputtedKeys[key] != NULL)
    {
      WINDOWSSYSTEM->Request_ERRORMessageBox("A KeyState for the passed key already exists.","KeyState Exists");
      return;
    }
    else
    {
       newkeystate = new KeyState(key);
       // Add the new keystate object to the Input Manager's map of inputted keys
       _InputtedKeys[newkeystate->Key] = newkeystate;

#if (INDICATEREGISTERDKEYS == 1)
       WINDOWSSYSTEM->Request_OKMessageBox("Created a New KeyState for  key.", "KeyState Created");
#endif
    }

  }


  /////////////////////////////////////////////////////////////////////////////
  /// *****KEY STATE CHECKING***** ///
  /////////////////////////////////////////////////////////////////////////////
  // May need to set 'Enabled' conditional for these
  bool InputManager::KeyIsPressed(unsigned int inquirykey)
  {
    KeyStateMap::iterator it = _InputtedKeys.begin();
    for (; it != _InputtedKeys.end(); ++it)
    {
      if (it->first == inquirykey)
      {
        // If Pressed is true, Down is true as well.
        // Set Pressed to false so any executions dependent upon one-time key press actions are only activated once.
        if (it->second->Pressed == true)
          return true;
      }
    }

    return false;
  }
  bool InputManager::KeyIsReleased(unsigned int inquirykey)
  {
    KeyStateMap::iterator it = _InputtedKeys.begin();
    for (; it != _InputtedKeys.end(); ++it)
    {
      if (it->first == inquirykey)
      {
        // If Pressed is true, Down is true as well.
        // Set Pressed to false so any executions dependent upon one-time key press actions are only activated once.
        if (it->second->Released == true)
          return true;
      }
    }

    return false;
  }
  bool InputManager::KeyIsDown(unsigned int inquirykey)
  {
    KeyStateMap::iterator it = _InputtedKeys.begin();
    for (; it != _InputtedKeys.end(); ++it)
    {
      if (it->first == inquirykey)
      {
        // If Pressed is true, Down is true as well.
        // Set Pressed to false so any executions dependent upon one-time key press actions are only activated once.
        if (it->second->Down == true)
          return true;
      }
    }

    return false;
  }
  bool InputManager::KeyIsUp(unsigned int inquirykey)
  {
    KeyStateMap::iterator it = _InputtedKeys.begin();
    for (; it != _InputtedKeys.end(); ++it)
    {
      if (it->first == inquirykey)
      {
        // If Pressed is true, Down is true as well.
        // Set Pressed to false so any executions dependent upon one-time key press actions are only activated once.
        if (it->second->Up == true)
          return true;
        else
          return false;
      }
    }

    // If we've iterated through the entire map and haven't found the keystate...
    if (it == _InputtedKeys.end())
    {
      if (it->first != inquirykey)
      {
        WINDOWSSYSTEM->Request_ERRORMessageBox("You are asking for a KeyIsUp inquiry for a KeyState that hasn't been created yet.","KeyState doesn't exist");
      }
    }

    return false;
  }


  /////////////////////////////////////////////////////////////////////////////
  /// *****CLEANUP***** ///
  /////////////////////////////////////////////////////////////////////////////
  void InputManager::_DeleteKeyStates()
  {
    KeyStateMap::iterator it = _InputtedKeys.begin();
    for (; it != _InputtedKeys.end(); ++it)
    {
      OutputDebugStringA("Destroying Keys State Object");
      delete it->second;
    }

    _InputtedKeys.clear();
  }

}