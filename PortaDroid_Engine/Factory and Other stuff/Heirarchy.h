#pragma once

#include "Sprite.h"
#include "Containers.h"
#include "Camera.h"

namespace OBALFramework
{
  /*
  struct HeirID
  {
    Sprite* Child;
    int ID;
  };
  */

  class Heirarchy
  {
  public:
    Heirarchy();
    ~Heirarchy();

    void Update();

    void AddChild(Sprite * child);
    void RemoveChild(Sprite child);

    void EmptyChildren();

    Camera * Owner;

    std::vector<Sprite*> Parents;
    std::vector<Sprite*> Children;
  };
}