#include "DirectLight.h"
#include "Transform.h"
#include "Graphics.h"

namespace OBALFramework
{
  DirectLight::DirectLight()
  {
    OutputDebugStringA("Default 'DirectLight' Component Created.\n");
  }
  DirectLight::~DirectLight()
  {
    OutputDebugStringA("'DirectLight' Component being erased from GRAPHICS DirectLightList (destroying)\n");

    GRAPHICS->DirectLightList.erase(this);
  }

  void DirectLight::Initialize()
  {
    OutputDebugStringA("Initializing 'DirectLight' Component.\n");
    // Specify the ComponentTypeId
    TypeId = CT_DirectLight;

    // Get this Owner Object's Transform component (dependent)
    transform = GetOwner()->has(Transform);

    // Push this sprite on the Graphics system's spritelist for drawing in the future
    GRAPHICS->DirectLightList.push_back(this);
  }

  void DirectLight::SetColor(D3DXCOLOR color)
  {
    LightColor = color;
  }

}
