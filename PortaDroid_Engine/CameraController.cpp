#include "CameraController.h"
#include "InputManager.h"
//#include "KeyDefines.h"
#include "Transform.h"

namespace OBALFramework
{
  CameraController::CameraController()
  {

  }
  CameraController::~CameraController()
  {
  }

  void CameraController::Initialize()
  {
    OutputDebugStringA("Initializing 'CameraController' Component.\n");
    // Specify the ComponentTypeId
    TypeId = CT_CameraController;

    // Get this Owner Object's Transform component (dependent)
    transform = GetOwner()->has(Transform);
  }

  void CameraController::Update(float dt)
  {
    // If the controller is disabled, do nothing
    if (Enabled == false)
      return;
    
    // Create a vector which will be added to the owner's translation if any of the designated buttons are pressed.
    Vec3 movement = Vec3(0,0,0);

    // If the Up button is down...
    if (INPUTMGR->KeyIsDown(KEY_W))
    {
      movement += Vec3(0,1,0);
    }
    // If the Down button is down...
    if (INPUTMGR->KeyIsDown(KEY_S))
    {
      movement += Vec3(0, -1, 0);
    }
    // If the Left button is down...
    if (INPUTMGR->KeyIsDown(KEY_A))
    {
      movement += Vec3(-1, 0, 0);
    }
    // If the Right button is down...
    if (INPUTMGR->KeyIsDown(KEY_D))
    {
      movement += Vec3(1, 0, 0);
    }
    // If the Forward button is down...
    if (INPUTMGR->KeyIsDown(VK_LSHIFT))
    {
      movement += Vec3(0, 0, -1);
    }
    // If the Backward button is down...
    if (INPUTMGR->KeyIsDown(VK_LCONTROL))
    {
      movement += Vec3(0, 0, 1);
    }

    // If any movement button was pressed, apply it to the owner
    if (movement != Vec3(0,0,0))
    {
      // Apply movement to the camera
      transform->Position += movement;
    }
  }

}
