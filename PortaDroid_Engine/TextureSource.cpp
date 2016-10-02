#include "TextureSource.h"

namespace OBALFramework
{
  TextureSource::TextureSource(std::string name, IDirect3DTexture9 * texture, unsigned int framesperrow, 
                                unsigned int numofrows, unsigned int framerate, unsigned int sourcesizex, 
                                unsigned int sourcesizey)
  {
    TextureName = name;
    Texture = texture;
    FramesPerRow = framesperrow;
    NumberofRows = numofrows;
    FrameRate = framerate;
    SourceSizeX = sourcesizex;
    SourceSizeY = sourcesizey;
    FrameSizeX = SourceSizeX / FramesPerRow;
    FrameSizeY = SourceSizeY / NumberofRows;

    OriginX = (int)SourceSizeX / 2;
    OriginY = (int)SourceSizeY / 2;

    if (Texture != NULL)
      OutputDebugStringA("DEFAULT Texture Source is something.\n");
    else
      OutputDebugStringA("DEFAULT SurfaceSource NOT something!!!\n");
    
    ///Intialize();
  }
  TextureSource::~TextureSource()
  {
  }

  void TextureSource::Intialize()
  {
  }

}