#pragma once

#include "GameObjectComposition.h"
#include "VMath.h"

namespace OBALFramework
{
  //Forward Declaration
  class Transform;

  class Camera : public GameComponent
  {
  public:
    Camera();
    ~Camera();

    void Initialize();

    void Update();


    Vec3 CameraLookAt;       // the look-at position
    Vec3 CameraUpDirection;  //the up direction

    float FieldofView = 45.0f;   // The field of view for the camera in DEGREES
    float NearPlane = 0.5f;    // The closest the camera will render
    float FarPlane = 1000.0f;     // The farthest the camera will render
    float OrthoProjSize = 20.0f;      // Size of the Orthographic Projection

    //Used by GRAPHICS for camera position when getting view and projection matricies
    Transform * transform;
  };
}