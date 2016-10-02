#include "Transform.h"

namespace OBALFramework
{
  Transform::Transform()
  {
    OutputDebugStringA("Default 'Transform' Component Created.\n");
    Position = Vec3(0,0,0);
    Rotation = Vec3(0.0f, 0.0f, 0.0f);
    Scale = Vec3(1.0f,1.0f,1.0f);
  }
  Transform::Transform(FLOAT x, FLOAT y, FLOAT z, Vec3 rotation, FLOAT scalex, FLOAT scaley, FLOAT scalez)
  {
    OutputDebugStringA("Conversion 'Transform' Component Created.\n");
    Position = Vec3(x, y, z);
    Rotation = rotation;
    Scale = Vec3(scalex, scaley, scalez);

  }
  Transform::~Transform()
  {
    OutputDebugStringA("'Transform' Component Destroyed.\n");
  }

  void Transform::Initialize()
  {
    OutputDebugStringA("Initializing 'Transform' Component.\n");
    // Specify the ComponentTypeId
    TypeId = CT_Transform;
  }


  void Transform::UpdateTransform()
  {
    LastPos = Position;
  }
}
