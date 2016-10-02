#pragma once

#include <string>
#include "VMath.h"
#include "SpriteSource.h"
#include "GameObjectComposition.h"

namespace OBALFramework
{
  //Forward Declaration
  class Transform;

  class BackBufferSurfaceSprite : public GameComponent
  {
  public:
    BackBufferSurfaceSprite();
    BackBufferSurfaceSprite(bool visible, std::string sourcename, 
                              unsigned int startingrow, unsigned int startingframe, Vec4 color);
    ~BackBufferSurfaceSprite();

    void Initialize();


    BackBufferSurfaceSprite * Next;
    BackBufferSurfaceSprite * Prev;
    bool Visible = true;
    std::string SourceName;        //Name of the sprite's SpriteSource asset
    SpriteSource * SpriteSourceAsset;      //The Texture assset of the sprite
    RECT BackBufferSourceRect;
    unsigned int CurrentRow = 0;      // The current row of the bitmap animation
    unsigned int CurrentFrame = 0;    // The current frame upon the current row of the bitmap animation
    Transform * transform;    //What Transform to use for this sprite
    Vec4 Color;           //Blend color of this sprite
  };
}