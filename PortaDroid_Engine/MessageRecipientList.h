#pragma once

#include "EngineBases.h"

namespace OBALFramework
{
  class MessageRecipientList
  {
  public:
    MessageRecipientList();
    ~MessageRecipientList();

    void AddRecipient(ISystem* system, MSGID id);

    //private:    (CONSIDER THIS BEING PUBLIC!!!)
    std::vector<ISystem*> None_Recipients;
    std::vector<ISystem*> All_Recipients;
    std::vector<ISystem*> Quit_Recipients;
    std::vector<ISystem*> Collision_Recipients;
    std::vector<ISystem*> DebugInfo_Recipients;
    std::vector<ISystem*> CharKey_Recipients;
    std::vector<ISystem*> HeldKey_Recipients;
    std::vector<ISystem*> MouseButton_Recipients;
    std::vector<ISystem*> MouseMove_Recipients;
    std::vector<ISystem*> FileDrop_Recipients;
  };

  extern MessageRecipientList * MSGRECIPIENTS;
}