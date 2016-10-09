#include "Serializer.h"

bool Serializer::Serialize(Serializeable * obj, std::string fileName)
{
  //Check to see if we have an object.
  if (!obj)
    return false;

  std::ofstream out;
  Json::Value val;
  Json::StyledStreamWriter writer;

  //Call the object's serialize.
  obj->Serialize(val);

  //Write out to the file.
  out.open(fileName);
  if (out.is_open())
  {
    writer.write(out, val);
    out.close();

    return true;
  }
  return false;
}

bool Serializer::Deserialize(Serializeable * obj, std::string fileName)
{
  //Check for the object.
  if (!obj)
    return false;

  std::ifstream in;
  Json::Value val;
  Json::Reader reader;

  //Open the input file.
  in.open(fileName, std::ifstream::in);
  if (in.is_open())
  {
    //Read in, give to object.
    reader.parse(in, val);
    obj->Deserialize(val);

    return true;
  }

  return false;
}
