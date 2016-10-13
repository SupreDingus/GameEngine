#pragma once

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "GameObjectComposition.h"
#include "Factory.h"
#include "Transform.h"

namespace OBALFramework
{
  class LevelGen
  {
  public:
    LevelGen();
    ~LevelGen();

    void CreateEmptyLevel();
    void LoadLevel(std::string file);
  };
}
