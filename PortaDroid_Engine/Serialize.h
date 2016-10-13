/* DEPRECATED */
#pragma once
#include <iostream>
#include <fstream>
#include "json\json.h"

class Serializer
{
public:
  Serializer(std::string fileName);
  ~Serializer();

  void Send(Json::Value val);
  bool Receive(Json::Value val);

  //template<typename T> void Send(T value);

private:
  //Styled stream writer, for writing stuff to files.
  Json::StyledStreamWriter writer;
  Json::Reader reader;

  //Hold the stream name here.
  std::filebuf fb;
  std::string fileName;
};
