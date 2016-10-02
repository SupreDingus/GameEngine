#pragma once
#include "Serialization.h"
#include <fstream>

namespace OBALFramework
{
  // This is the interface to serialize text files
  class TextSerializer : public BaseSerializer
  {
    public:
      std::ifstream stream;
      virtual bool Open(const std::string& file);
      virtual bool IsGood();
      virtual void ReadInt(int& i);
      virtual void ReadFloat(float& i);
      virtual void ReadString(std::string& str);
  };
}