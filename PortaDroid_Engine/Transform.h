#pragma once

#include "VMath.h"
#include "GameObjectComposition.h"

namespace OBALFramework
{
  class Transform : public GameComponent
  {
  public:
    Transform();
    Transform(FLOAT x, FLOAT y, FLOAT z, Vec3 rotation, FLOAT scalex, FLOAT scaley, FLOAT scalez);
    ~Transform();
    
    void Initialize();

    void UpdateTransform();


    Vec3 Position;
    Vec3 Rotation;
    Quat QuatRotation;
    Vec3 Scale;

    Vec3 LastPos = Vec3(0,0,0);
  };
}