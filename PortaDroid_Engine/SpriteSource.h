#pragma once

#include <string>
#include "VMath.h"

namespace OBALFramework
{
  class SpriteSource
  {
  public:
    //SpriteSource();
    SpriteSource(std::string name, IDirect3DSurface9* surfacesource, unsigned int framesperrow, 
                  unsigned int numofrows, unsigned int framerate, unsigned int sourcesizex, 
                  unsigned int sourcesizey);
    ~SpriteSource();

    void Intialize();

    //Create a new Surface object
    std::string SurfaceName;          //Name of the sprite's surface asset
    IDirect3DSurface9 * SurfaceSource;
    unsigned int FramesPerRow = 1;
    unsigned int NumberofRows = 1;
    unsigned int FrameRate = 12;
    unsigned int SourceSizeX;     // The backbuffersprite's full width
    unsigned int SourceSizeY;     // The backbuffersprite's full height
    unsigned int OriginX;     // The backbuffersprite's center width
    unsigned int OriginY;     // The backbuffersprite's center height
  };
}