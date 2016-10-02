#include "Precompiled.h"
#include "TextSerialization.h"

namespace OBALFramework
{
  // This opens the file for reading and returns a bool saying if it was successful or not
  bool TextSerializer::Open(const std::string& file)
  {
    stream.open(file.c_str());
    return stream.is_open();
  }

  // Check if the stream still has data that needs to be read
  bool TextSerializer::IsGood()
  {
    return stream.good();
  }

  // If we've read an int from the file, then asssign the value to the parameter
  void TextSerializer::ReadInt(int& i)
  {
    stream >> i;
  }

  // If we've read a float from the file, then asssign the value to the parameter
  void TextSerializer::ReadFloat(float& f)
  {
    stream >> f;
  }

  // If we've read a string from the file, then asssign the value to the parameter
  void TextSerializer::ReadString(std::string& str)
  {
    stream >> str;
  }
}