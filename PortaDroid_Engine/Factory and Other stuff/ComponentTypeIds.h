#pragma once

namespace OBALFramework
{
  enum ComponentTypeId
  {
    // Invalid component id
    CT_None = 0,
    
    // Basic Components
    CT_Transform,
    CT_Camera,
    CT_Sprite,

    // Max
    CT_MaxComponents
  };
}