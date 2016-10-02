#pragma once

#include <string>
#include "VMath.h"
//#include "Transform.h"
#include "TextureSource.h"    //For TextureSource [Doesn't just use a DirectX Texture: is a class]
#include "GameObjectComposition.h"

namespace OBALFramework
{
  //Forward Declaration
  class Transform;

  class Sprite : public GameComponent
  {
  public:
    Sprite();
    Sprite(bool visible, bool visibilityculling, std::string sourcename, 
            unsigned int startingrow, unsigned int startingframe, unsigned int numframes, Vec4 color);
    ~Sprite();

    void Initialize();

    void Update(float dt);

    void SetNewTexture(std::string sourcename, unsigned int startingrow, unsigned int startingframe, Vec4 color);
    void Draw(LPDIRECT3DDEVICE9 pDevice, FLOAT worldscale);

    
    Sprite * Next;
    Sprite * Prev;
    bool Visible = true;
    bool VisibilityCulling = false;   //Visibility Culling when this sprite is not being viewed by the camera
    std::string TextureName;          //Name of the sprite's SpriteSource asset
    TextureSource * SpriteTextureAsset;      //The Texture assset of the sprite
    RECT SourceRect;
    unsigned int CurrentRow = 0;      // The current row of the bitmap animation
    unsigned int CurrentFrame = 0;    // The current frame upon the current row of the bitmap animation
    unsigned int NumberofRows = 0;
    unsigned int FramesperRow = 0;
    Transform * transform;    //What Transform to use for this sprite
    D3DCOLOR SpriteColor;     //Blend color of this sprite
    float Timer = 0.0f;
    float FrameChangeTime = 0.0f;
  };
}