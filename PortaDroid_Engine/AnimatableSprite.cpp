#include "AnimatableSprite.h"
#include "Transform.h"
#include "Graphics.h"

#include <sstream>

namespace OBALFramework
{
  AnimatableSprite::AnimatableSprite()
  {
    OutputDebugStringA("Default 'Sprite' Component Created.\n");
    Visible = true;
    VisibilityCulling = false;
    TextureName = "animationtest.png";
    CurrentRow = 1;
    CurrentFrame = 1;
    NumberofRows = 0;
    FramesperRow = 0;
    SpriteColor = D3DCOLOR_RGBA(255, 255, 255, 255);
    SpriteTextureAsset = NULL;
  }
  AnimatableSprite::AnimatableSprite(bool visible, bool visibilityculling, std::string sourcename,
    unsigned int startingrow, unsigned int startingframe, unsigned int numframes, Vec4 color)
  {
    OutputDebugStringA("Conversion 'Sprite' Component Created.\n");
    Visible = visible;
    VisibilityCulling = visibilityculling;
    TextureName = sourcename;
    CurrentRow = startingrow;
    CurrentFrame = startingframe;
    NumberofRows = 0;
    FramesperRow = 0;
    SpriteColor = D3DCOLOR_RGBA((int)color.x, (int)color.y, (int)color.z, (int)color.w);
    SpriteTextureAsset = NULL;
  }
  AnimatableSprite::~AnimatableSprite()
  {
    OutputDebugStringA("'Sprite' Component being erased from GRAPHICS SpriteList (destroying)\n");

    GRAPHICS->AnimatableSpriteList.erase(this);
  }


  void AnimatableSprite::Initialize()
  {
    OutputDebugStringA("Initializing 'Sprite' Component.\n");
    // Specify the ComponentTypeId
    TypeId = CT_AnimatableSprite;

    // Get this Owner Object's Transform component (dependent)
    transform = GetOwner()->has(Transform);

    // Get the sprite's texture from the graphics engine
    SpriteTextureAsset = GRAPHICS->GetTextureSource(TextureName);
    
    // Get the dimensions of the spritesource (size has been deleted)
    ///size.x = (FLOAT)SpriteSourceAsset->SourceSizeX;
    ///size.y = (FLOAT)SpriteSourceAsset->SourceSizeY;

    if (SpriteTextureAsset != NULL)
    {
      OutputDebugStringA("Texture '");
      OutputDebugStringA(TextureName.c_str());
      OutputDebugStringA("' Found!!!\n");

      NumberofRows = SpriteTextureAsset->NumberofRows;
      FramesperRow = SpriteTextureAsset->FramesPerRow;

      if (CurrentRow <= 0)
        CurrentRow = 1;
      if (CurrentFrame <= 0)
        CurrentFrame = 1;

      // Set up the SourceRect for the first time
      UpdateSourceRect();

      // Set the frame rate time at which the animation plays
      FrameChangeTime = 1.0f / SpriteTextureAsset->FrameRate;
      Timer = FrameChangeTime;
    }
    else
    {
      OutputDebugStringA("ERROR: SpriteSource with name '");
      OutputDebugStringA(TextureName.c_str());
      OutputDebugStringA("' was not found.\n");
    }


    // Push this sprite on the Graphics system's spritelist for drawing in the future
    GRAPHICS->AnimatableSpriteList.push_back(this);
    //(*Owner).GetComponent();
  }

  void AnimatableSprite::Update(float dt)
  {
    // If the sprites current texture is not one to be animated...
    if (SpriteTextureAsset->FramesPerRow != 0 && SpriteTextureAsset->NumberofRows != 0)
    {
      // If the frame timer hasn't reached 0, keep counting down
      if (Timer > 0.0f)
      {
        Timer -= dt;
      }
      // When the timer does reach 0, set the current frame/row to the next in sequence
      else
      {
        // If the next frame isn't the last in the row, set the current frame to the next frame
        if (CurrentFrame < SpriteTextureAsset->FramesPerRow)
        {
          CurrentFrame++;
        }
        else
        {
          // If the next row isn't the last , set the current frame to the first frame of the next row
          if (CurrentRow < SpriteTextureAsset->NumberofRows)
          {
            CurrentFrame = 1;
            CurrentRow++;
          }
          // Else, the next frame is the first frame
          else
          {
            CurrentFrame = 1;
            CurrentRow = 1;
          }
        }

        UpdateSourceRect();

        Timer = FrameChangeTime;
      }
    }
  }

  void AnimatableSprite::Draw(LPD3DXSPRITE pSprite, FLOAT worldscale, Vec2 position2D)
  {
    // Texture being used is 64 by 64:
    Vec2 spriteCenter2D((float)SpriteTextureAsset->FrameSizeX / 2, (float)SpriteTextureAsset->FrameSizeY / 2);
    Vec3 spriteCenter3D(spriteCenter2D.x, spriteCenter2D.y, 0.0f);

    // Screen position of the sprite
    Vec2 trans(position2D.x, position2D.y);
    //Vec2 trans(transform->Position.x, transform->Position.y);

    // Rotate based on the time passed
    float rotation = timeGetTime() / 500.0f;

    // Build our matrix to rotate, scale and position our sprite
    D3DXMATRIX mat;

    Vec2 scaling(transform->Scale.x, transform->Scale.y);

    // out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
    //D3DXMatrixTransformation2D(&mat, &spriteCenter, NULL, NULL, &spriteCenter, NULL, &trans);
    D3DXMatrixTransformation2D(&mat, NULL, 0.0, &scaling, NULL, rotation, &trans);

    // Tell the sprite about the matrix
    pSprite->SetTransform(&mat);
    
    // Draw the sprite 
    pSprite->Draw(SpriteTextureAsset->Texture, &SourceRect, &spriteCenter3D, NULL, 0xFFFFFFFF);
    
  }

  void AnimatableSprite::UpdateSourceRect()
  {
    SourceRect.top = (CurrentRow - 1) * (LONG)SpriteTextureAsset->FrameSizeY;    // The initial row is the Current 
    SourceRect.left = (CurrentFrame - 1) * (LONG)SpriteTextureAsset->FrameSizeX;
    SourceRect.right = SourceRect.left + (LONG)SpriteTextureAsset->FrameSizeX;
    SourceRect.bottom = SourceRect.top + (LONG)SpriteTextureAsset->FrameSizeY;
  }

}
