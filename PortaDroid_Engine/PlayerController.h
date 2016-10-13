#pragma once

#include "Transform.h"
#include "Physics.h"
#include "VMath.h"
#include "GameObjectComposition.h"
#include "InputManager.h"
#include "Body.h"

namespace OBALFramework
{
  class PlayerController : public GameComponent
  {
  public:
    PlayerController();
    ~PlayerController(); //These won't have anything, since nothing needs to be set.

    void Initialize();
    void Update(float dt);

  private:
    GOC* player;
    Transform* trans;
    Body* body;
  };
}
