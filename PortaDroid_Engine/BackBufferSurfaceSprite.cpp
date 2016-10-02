#include "BackBufferSurfaceSprite.h"
#include "Transform.h"
#include "Graphics.h"

namespace OBALFramework
{
  BackBufferSurfaceSprite::BackBufferSurfaceSprite()
  {
    OutputDebugStringA("Default 'BackBufferSurfaceSprite' Compenent Created.\n");
    Visible = true;
    SourceName = "default.bmp";
    CurrentRow = 0;
    CurrentFrame = 0;
    /// TEMPORARY
    Color = Vec4(0, 0, 0, 1);
    SpriteSourceAsset = NULL;
  }
  BackBufferSurfaceSprite::BackBufferSurfaceSprite(bool visible, std::string sourcename, 
                            unsigned int startingrow, unsigned int startingframe, Vec4 color)
  {
    OutputDebugStringA("Conversion 'BackBufferSurfaceSprite' Compenent Created.\n");
    Visible = visible;
    SourceName = sourcename;
    CurrentRow = startingrow;
    CurrentFrame = startingframe;
    Color = color;
    SpriteSourceAsset = NULL;
  }
  BackBufferSurfaceSprite::~BackBufferSurfaceSprite()
  {
    OutputDebugStringA("'BackBufferSurfaceSprite' Component being erased from GRAPHICS BackBufferSurfaceSpriteList (destroying)\n");

    GRAPHICS->BackBufferSurfaceSpriteList.erase(this);
  }

  void BackBufferSurfaceSprite::Initialize()
  {
    OutputDebugStringA("Initializing 'BackBufferSurfaceSprite' Component.\n");
    // Specify the ComponentTypeId
    TypeId = CT_BackBufferSurfaceSprite;

    // Get this Owner Object's Transform component (dependent)
    transform = GetOwner()->has(Transform);

    // Get the sprite's texture from the graphics engine
    SpriteSourceAsset = GRAPHICS->GetSpriteSource(SourceName);

    // Get the dimensions of the spritesource (size has been deleted)
    ///size.x = (FLOAT)SpriteSourceAsset->SourceSizeX;
    ///size.y = (FLOAT)SpriteSourceAsset->SourceSizeY;

    if (SpriteSourceAsset != NULL)
    {
      OutputDebugStringA("SpriteSource ");
      OutputDebugStringA(SourceName.c_str());
      OutputDebugStringA(" Found!!!\n");

      if (CurrentRow >= SpriteSourceAsset->NumberofRows)
        CurrentRow = 0;
      if (CurrentFrame >= SpriteSourceAsset->FramesPerRow)
        CurrentFrame = 0;

      CurrentRow = 0;
      CurrentFrame = 0;

      BackBufferSourceRect.top = CurrentRow * (LONG)SpriteSourceAsset->SourceSizeY;    // The initial row is the Current 
      BackBufferSourceRect.left = CurrentFrame * (LONG)SpriteSourceAsset->SourceSizeX;
      BackBufferSourceRect.right = BackBufferSourceRect.left + (LONG)SpriteSourceAsset->SourceSizeX;
      BackBufferSourceRect.bottom = BackBufferSourceRect.top + (LONG)SpriteSourceAsset->SourceSizeY;
    }
    else
    {
      OutputDebugStringA("ERROR: SpriteSource with name '");
      OutputDebugStringA(SourceName.c_str());
      OutputDebugStringA("' was not found.\n");
    }


    // Push this sprite on the Graphics system's spritelist for drawing in the future
    GRAPHICS->BackBufferSurfaceSpriteList.push_back(this);
    //(*Owner).GetComponent();
  }


}
