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

    float FieldofView;        // The field of view for the camera in DEGREES
    float NearPlane;          // The closest the camera will render
    float FarPlane;           // The farthest the camera will render
    float OrthoProjSize;      // Size of the Orthographic Projection

    //Used by GRAPHICS for camera position when getting view and projection matricies
    Transform * transform;
  };
}