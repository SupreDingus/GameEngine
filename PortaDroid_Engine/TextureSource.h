#pragma once

#include "EngineBases.h"

namespace OBALFramework
{
  class TextureSource
  {
  public:
    TextureSource(std::string name, IDirect3DTexture9* surfacesource, unsigned int framesperrow, 
                    unsigned int numofrows, unsigned int framerate, unsigned int sourcesizex, 
                    unsigned int sourcesizey);
    ~TextureSource();

    void Intialize();


    std::string TextureName;        //Name of the sprite's SpriteSource asset
    IDirect3DTexture9 * Texture;
    unsigned int FramesPerRow = 1;
    unsigned int NumberofRows = 1;
    unsigned int FrameRate = 12;
    unsigned int SourceSizeX;     // The texture's full width
    unsigned int SourceSizeY;     // The texture's full height
    unsigned int FrameSizeX;
    unsigned int FrameSizeY;
    unsigned int OriginX;     // The texture's center width
    unsigned int OriginY;     // The texture's center height

  };
}