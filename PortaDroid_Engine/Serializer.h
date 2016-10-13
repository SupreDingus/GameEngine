#pragma once
#include "Serializable.h"
#include <iostream>
#include <fstream>

class Serializer
{
public:
  static bool Serialize(Serializeable* obj, std::string fileName);
  static bool Deserialize(Serializeable* obj, std::string fileName);

private:
  Serializer() { };
};
