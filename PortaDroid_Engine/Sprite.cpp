#include "Sprite.h"
#include "Transform.h"
#include "Graphics.h"

#include <sstream>

namespace OBALFramework
{
  Sprite::Sprite()
  {
    OutputDebugStringA("Default 'Sprite' Component Created.\n");
    Visible = true;
    VisibilityCulling = false;
    TextureName = "animationtest.png";
    CurrentRow = 0;
    CurrentFrame = 0;
    NumberofRows = 0;
    FramesperRow = 0;
    SpriteColor = D3DCOLOR_RGBA(255, 255, 255, 255);
    SpriteTextureAsset = NULL;
  }
  Sprite::Sprite(bool visible, bool visibilityculling, std::string sourcename, 
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
  Sprite::~Sprite()
  {
    OutputDebugStringA("'Sprite' Component being erased from GRAPHICS SpriteList (destroying)\n");

    GRAPHICS->SpriteList.erase(this);
  }


  void Sprite::Initialize()
  {
    OutputDebugStringA("Initializing 'Sprite' Component.\n");
    // Specify the ComponentTypeId
    TypeId = CT_Sprite;

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

      if (CurrentRow >= NumberofRows)
        CurrentRow = 0;
      if (CurrentFrame >= FramesperRow)
        CurrentFrame = 0;
      
      SourceRect.top = CurrentRow * (LONG)SpriteTextureAsset->SourceSizeY;    // The initial row is the Current 
      SourceRect.left = CurrentFrame * (LONG)SpriteTextureAsset->SourceSizeX;
      SourceRect.right = SourceRect.left + (LONG)SpriteTextureAsset->SourceSizeX;
      SourceRect.bottom = SourceRect.top + (LONG)SpriteTextureAsset->SourceSizeY;

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
    GRAPHICS->SpriteList.push_back(this);
    //(*Owner).GetComponent();
  }

  void Sprite::Update(float dt)
  {
    /*
    // If the sprites current texture is not one to be animated...
    if (SpriteTextureAsset->FramesPerRow != 0 || SpriteTextureAsset->NumberofRows != 0)
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
          ++CurrentFrame;
        else
        {
          // If the next row isn't the last , set the current frame to the first frame of the next row
          if (CurrentRow < SpriteTextureAsset->NumberofRows)
          {
            CurrentFrame = 0;
            ++CurrentRow;
          }
          // Else, the next frame is the first frame
          else
          {
            CurrentFrame = 0;
            CurrentRow = 0;
          }
        }

        Timer = FrameChangeTime;
      }
    }
    */
  }

  void Sprite::SetNewTexture(std::string sourcename, unsigned int startingrow, unsigned int startingframe, Vec4 color)
  {
    TextureName = sourcename;
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

      if (CurrentRow >= NumberofRows)
        CurrentRow = 0;
      if (CurrentFrame >= FramesperRow)
        CurrentFrame = 0;

      SourceRect.top = CurrentRow * (LONG)SpriteTextureAsset->SourceSizeY;    // The initial row is the Current 
      SourceRect.left = CurrentFrame * (LONG)SpriteTextureAsset->SourceSizeX;
      SourceRect.right = SourceRect.left + (LONG)SpriteTextureAsset->SourceSizeX;
      SourceRect.bottom = SourceRect.top + (LONG)SpriteTextureAsset->SourceSizeY;
    }
    else
    {
      OutputDebugStringA("ERROR: SpriteSource with name '");
      OutputDebugStringA(TextureName.c_str());
      OutputDebugStringA("' was not found.\n");
    }
  }
  void Sprite::Draw(LPDIRECT3DDEVICE9 pDevice, FLOAT worldscale)
  {
    SourceRect.top = CurrentRow * (LONG)SpriteTextureAsset->SourceSizeY;
    SourceRect.left = CurrentFrame * (LONG)SpriteTextureAsset->SourceSizeX;
    SourceRect.right = SourceRect.left + (LONG)SpriteTextureAsset->SourceSizeX;
    SourceRect.bottom = SourceRect.top + (LONG)SpriteTextureAsset->SourceSizeY;

    D3DXMATRIX matRotateX;
    D3DXMATRIX matRotateY;
    D3DXMATRIX matRotateZ;
    D3DXMATRIX matScale;
    D3DXMATRIX matTranslate;
    //D3DXMATRIX matTransform;

    D3DXMatrixRotationX(&matRotateX, D3DXToRadian(transform->Rotation.x));
    D3DXMatrixRotationY(&matRotateY, D3DXToRadian(transform->Rotation.y));
    D3DXMatrixRotationZ(&matRotateZ, D3DXToRadian(transform->Rotation.z));
    D3DXMatrixScaling(&matScale, transform->Scale.x, transform->Scale.y, transform->Scale.z);
    D3DXMatrixScaling(&matScale, worldscale, worldscale, worldscale);
    D3DXMatrixTranslation(&matTranslate, transform->Position.x, transform->Position.y, transform->Position.z);

    matTranslate = matRotateX * matRotateY * matRotateZ * matScale * matTranslate;

    pDevice->SetTransform(D3DTS_WORLD, &matTranslate);
    //Set the default texture for anything to be drawn
    pDevice->SetTexture(0, SpriteTextureAsset->Texture);
    
    //This is drawing the testSprite instead of the following square
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
  }

}
