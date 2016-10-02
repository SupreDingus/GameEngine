#include "Precompiled.h"
#include "FilePath.h"

namespace OBALFramework
{
  FilePath::FilePath()
  {

  }
  FilePath::FilePath(std::string filename)
  {
    ConvertToLower(filename);

    std::size_t dirEnd = FullPath.find_last_of("\\");
    dirEnd = (dirEnd == std::string::npos) ? 0 : dirEnd + 1;

    std::size_t fileEnd = FullPath.find_last_of(".");
    fileEnd = (fileEnd == std::string::npos) ? FullPath.size() : fileEnd;

    //Extension may be nothing
    Extension = FullPath.substr(fileEnd);
    OutputDebugStringA("Extension = ");
    OutputDebugStringA(Extension.c_str());
    OutputDebugStringA(" : FileName = ");
    OutputDebugStringA(FileName.c_str());

    
    
    FileName = FullPath.substr(dirEnd, fileEnd - dirEnd);
    FileName += Extension;

    OutputDebugStringA("Extension = ");
    OutputDebugStringA(Extension.c_str());
    OutputDebugStringA(" : FileName = ");
    OutputDebugStringA(FileName.c_str());
    //FullDirectory = FullPath.substr(0, dirEnd);
  }
  FilePath::~FilePath()
  {
  }

  void FilePath::ConvertToLower(std::string lower)
  {
    for (unsigned int i = 0; i < lower.length(); ++i)
    {
      if (lower[i] == '/')
        FullPath += "\\";
      else
        FullPath += tolower(lower[i]);
    }
  }
}
