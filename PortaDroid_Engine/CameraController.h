#pragma once
#include "Component.h"


namespace OBALFramework
{
  class Transform;

  class CameraController : public GameComponent
  {
  public:
    CameraController();
    ~CameraController();

    void Initialize();

    void Update(float dt);

    bool Enabled = true;

  private:
    Transform * transform;
  };
}