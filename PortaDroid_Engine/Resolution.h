// ---------------------------------------------------------------------------
// Project Name   :
// File Name      : Resolution.h
// Author         : Sarah Janzen
// Creation Date  : 2016/09/10
// Purpose        : Header file for Resolution.cpp
// History        :
// -9/10/16  : created
// © Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
// ---------------------------------------------------------------------------
#pragma once
#include "Collision.h"
#include <string>

#define MAXCONTACTS 1024

namespace OBALFramework
{
  //A Set of contacts that need to be resolved.
  class ContactSet
  {
  public:
    BodyContact *GetNextContact();
    void ResolveContacts(float dt);
    void Reset();

  private:
    friend class Physics;
    static const int MaxContacts = MAXCONTACTS;
    BodyContact contactArray[MaxContacts];
    unsigned NumberOfContacts;
    void ResolveVelocities(float dt);
    void ResolvePositions(float dt);
  };

  void ResloveContactVelocity(BodyContact& c, float dt);
}