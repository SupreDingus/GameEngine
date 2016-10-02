#include "SpriteSource.h"

namespace OBALFramework
{
  /*
  SpriteSource::SpriteSource()
  {
    SurfaceName = "No SpriteSource Name";
    SurfaceSource = NULL;
    Intialize();
  }
  */
  SpriteSource::SpriteSource(std::string name, IDirect3DSurface9* surfacesource, unsigned int framesperrow, 
                              unsigned int numofrows, unsigned int framerate, unsigned int sourcesizex, 
                              unsigned int sourcesizey)
  {
    SurfaceName = name;
    SurfaceSource = surfacesource;
    FramesPerRow = framesperrow;
    NumberofRows = numofrows;
    FrameRate = framerate;
    SourceSizeX = sourcesizex;
    SourceSizeY = sourcesizey;

    OriginX = (int)SourceSizeX / 2;
    OriginY = (int)SourceSizeY / 2;

    if (SurfaceSource != NULL)
      OutputDebugStringA("DEFAULT SurfaceSource is something.\n");
    else
      OutputDebugStringA("DEFAULT SurfaceSource NOT something\n");

    Intialize();
  }
  SpriteSource::~SpriteSource()
  {
  }

  void SpriteSource::Intialize()
  {
  }
}
