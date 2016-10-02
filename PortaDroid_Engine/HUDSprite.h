#pragma once

//#include <string>
#include "VMath.h"
#include "TextureSource.h"
#include "GameObjectComposition.h"

namespace OBALFramework
{
  //Forward Declaration
  class Transform;

  class HUDSprite : public GameComponent
  {
  public:
    HUDSprite();
    HUDSprite::HUDSprite(bool visible, bool visibilityculling, /*std::string*/char* sourcename,
      unsigned int startingrow, unsigned int startingframe, Vec4 color);
    ~HUDSprite();

    void Initialize();
    void Draw(LPD3DXSPRITE pSprite);


    HUDSprite * Next;
    HUDSprite * Prev;
    bool Visible = true;
    bool VisibilityCulling = false;   //Visibility Culling when this sprite is not being viewed by the camera
    /*std::string*/char* TextureName;        //Name of the sprite's SpriteSource asset
    TextureSource * HUDSpriteTextureAsset;      //The Texture assset of the sprite
    RECT SourceRect;
    unsigned int CurrentRow = 0;      // The current row of the bitmap animation
    unsigned int CurrentFrame = 0;    // The current frame upon the current row of the bitmap animation
    Transform * transform;    //What Transform to use for this sprite
    D3DCOLOR HUDSpriteColor;           //Blend color of this sprite
  };
}