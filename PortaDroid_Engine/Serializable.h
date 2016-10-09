#pragma once

#include "json\json.h"

class Serializeable
{
public:
  virtual ~Serializeable() { };
  virtual void Serialize(Json::Value& val) = 0;
  virtual void Deserialize(Json::Value& val) = 0;
};
