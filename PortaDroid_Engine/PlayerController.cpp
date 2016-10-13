#include "PlayerController.h"

namespace OBALFramework
{
  PlayerController::PlayerController() { }
  PlayerController::~PlayerController() { }

  void PlayerController::Initialize()
  {
    player = FACTORY->CreateSprite();
    FACTORY->GiveComponentToObject("Transform", player);
    FACTORY->GiveComponentToObject("Body", player);

    trans = player->has(Transform);
    //body = GetOwner()->has(Body);
    //body->IsStatic = false;

    TypeId = CT_PlayerController;
  }

  void PlayerController::Update(float dt)
  {
    //Move the player left and right first.
    //Check key presses.
    if (INPUTMGR->KeyIsDown(KEY_L))
    {
      trans->Position.x += 1.0f * dt;
    }
    else if (INPUTMGR->KeyIsDown(KEY_J))
    {
      trans->Position.x -= 1.0f * dt;
    }
    
    if (INPUTMGR->KeyIsDown(KEY_I))
    {
      trans->Position.y += 1.0f * dt;
    }
    else if (INPUTMGR->KeyIsDown(KEY_K))
    {
      trans->Position.y -= 1.0f * dt;
    }
  }
}
