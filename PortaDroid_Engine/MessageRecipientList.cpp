#include "MessageRecipientList.h"

namespace OBALFramework
{
  MessageRecipientList * MSGRECIPIENTS;


  MessageRecipientList::MessageRecipientList()
  { MSGRECIPIENTS = this; }
  MessageRecipientList::~MessageRecipientList()
  {}


  void MessageRecipientList::AddRecipient(ISystem * system, MSGID id)
  {
    switch (id)
    {
    case Mid::None:
    {
      // Add the system to the None Message Recipient Vector
      None_Recipients.push_back(system);
      break;
    }
    case Mid::All:
    {
      // Add the system to the None Message Recipient Vector
      All_Recipients.push_back(system);
      break;
    }
    case Mid::Quit: //
    {
      // Add the system to the Quit Message Recipient Vector
      Quit_Recipients.push_back(system);
      break;
    }
    case Mid::Collision: //
    {
      // Add the system to the Collision Message Recipient Vector
      Collision_Recipients.push_back(system);
      break;
    }
    case Mid::ToggleDebugInfo: //
    {
      // Add the system to the DebugInfo Message Recipient Vector
      DebugInfo_Recipients.push_back(system);
      break;
    }
    case Mid::CharacterKey: //
    {
      // Add the system to the CharacterKey Message Recipient Vector
      CharKey_Recipients.push_back(system);
      break;
    }
    /*
    case Mid::HeldKey: //
    {
      // Add the system to the CharacterKey Message Recipient Vector
      HeldKey_Recipients.push_back(system);
      break;
    }
    */
    case Mid::MouseButton: //
    {
      // Add the system to the MouseButton Message Recipient Vector
      MouseButton_Recipients.push_back(system);
      break;
    }
    case Mid::MouseMove: //
    {
      // Add the system to the MouseMove Message Recipient Vector
      MouseMove_Recipients.push_back(system);
      break;
    }
    case Mid::FileDrop: //
    {
      // Add the system to the FileDrop Message Recipient Vector
      FileDrop_Recipients.push_back(system);
      break;
    }
    } /// END Switch (id)
  }



}
