#pragma once

namespace OBALFramework
{
  class BaseSerializer
  {
    public:
   
      // This will open the inputted file. However, when used by a derived class this function must be overwritten (purely virtual)
      virtual bool Open(const std::string& file) = 0;
      // Checks if the stream/file still has data. (purely virtual) 
      virtual bool IsGood() = 0;
      virtual void ReadInt(int& i) = 0;
      virtual void ReadFloat(float& f) = 0;
      virtual void ReadString(std::string& str) = 0;
  };
  
  //Base case of serialization is that the object serializes itself.
  template<typename type>
  inline void StreamRead(BaseSerializer& stream, type& typeInstance)
  {
    typeInstance.Serialize(stream);
  }

  inline void StreamRead(BaseSerializer& stream, int& i)
  {
    stream.ReadInt(i);
  }

  inline void StreamRead(BaseSerializer& stream, float& f)
  {
    stream.ReadFloat(f);
  }

  inline void StreamRead(BaseSerializer& stream, std::string& str)
  {
    stream.ReadString(str);
  }
}