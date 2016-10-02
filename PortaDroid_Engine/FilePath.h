#pragma once



namespace OBALFramework
{
  class FilePath
  {
  public:
    FilePath();
    FilePath(std::string filename);
    ~FilePath();

    void ConvertToLower(std::string lower);

    std::string FullPath;
    std::string Extension;
    std::string FileName;

  };
}