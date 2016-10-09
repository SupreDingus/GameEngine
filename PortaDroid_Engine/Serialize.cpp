/* DEPRECATED */
#include "Serialize.h"

Serializer::Serializer(std::string fileName_)
{
  fileName = fileName_;
}

Serializer::~Serializer()
{
  if (fb.is_open())
    fb.close();
}

/*void Serializer::Send(Json::Value value)
{
  fb.open(fileName, std::ios::out);
  std::ostream os(&fb);
  writer.write(os, value);
}

template<typename T>
void Serializer::Send(T value)
{
  fb.open(fileName, std::ios::out);
  std::ostream os(&fb);
  writer.write(os, value);
}
*/

bool Serializer::Receive(Json::Value value)
{
  if (fb.open(fileName, std::ios::in))
  {
    std::istream is(&fb);
    reader.parse(is, value, false);

    return true;
  }
  return false;
}
