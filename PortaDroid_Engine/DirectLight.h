#pragma once

#include "EngineBases.h"
#include "GameObjectComposition.h"

namespace OBALFramework
{
  // Forward Declaration
  class Transform;

  class DirectLight : public GameComponent
  {
  public:
    DirectLight();
    ~DirectLight();

    void Initialize();

    void SetColor(D3DXCOLOR color);

    DirectLight * Next;
    DirectLight * Prev;
    int index;
    D3DLIGHT9 * Light;
    D3DXCOLOR LightColor;
    Transform * transform;
  };
}