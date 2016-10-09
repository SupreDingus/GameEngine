#include "Camera.h"
#include "Transform.h"
#include "Graphics.h"

namespace OBALFramework
{
  Camera::Camera()
  {
    OutputDebugStringA("Default 'Camera' Component Created.\n");
    CameraLookAt = Vec3(0.0f, 0.0f, 0.0f);
    CameraUpDirection = Vec3(0.0f, 1.0f, 0.0f);

    FieldofView = 45.0f;
    NearPlane = 0.5f;
    FarPlane = 1000.0f;
    OrthoProjSize = 18.0f;
  }
  Camera::~Camera()
  {
    // Detach from the Graphics System
    if (GRAPHICS->CurrentCamera == GetOwner())
    {
      OutputDebugStringA("'Camera' Component being unassigned to GRAPHICS->CurrentCamera.\n");

      GRAPHICS->CurrentCamera = NULL;
    }
  }

  void Camera::Initialize()
  {
    OutputDebugStringA("Initializing 'Camera' Component.\n");
    // Specify the ComponentTypeId
    TypeId = CT_Camera;

    // Get this Owner Object's Transform component (dependent)
    transform = GetOwner()->has(Transform);

    //transform = GetOwner()->has(Transform);
    OutputDebugStringA("Camera Initialized\n");
  }

  void Camera::Update()
  {
  }
}
