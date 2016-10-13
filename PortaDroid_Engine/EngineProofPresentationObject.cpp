#include "EngineProofPresentationObject.h"
#include "Body.h"
#include "DefaultText.h"
#include <sstream>
#include "Graphics.h"
#include "WindowsSystem.h"
#include <assert.h>

namespace OBALFramework
{
  EngineProofPresentationObject::EngineProofPresentationObject()
  {
  }
  EngineProofPresentationObject::~EngineProofPresentationObject()
  {
  }

  void EngineProofPresentationObject::Initialize()
  {
  }

  void EngineProofPresentationObject::Update(float dt)
  {
    if (Timer > 0.0f)
    {
      Timer -= dt;
    }
    else
    {
      fps = 1 / dt;
      Timer = SamplingTime;
    }

    if (INPUTMGR->KeyIsPressed(VK_UP))
    {
      if (SamplingTime == 0.01f)
        SamplingTime = 0.1f;
      else
        SamplingTime += 0.1f;

      Timer = SamplingTime;
    }
    if (INPUTMGR->KeyIsPressed(VK_DOWN))
    {
      if (SamplingTime - 0.1f <= 0.1f)
      {
        WINDOWSSYSTEM->Request_OKMessageBox("Warning: You are attempting to sample FPS at 0.00 seconds. Sampling Time has been set to 0.01 seconds.", "Invalid Sampling Time");
        SamplingTime = 0.01f;
      }
      else
        SamplingTime -= 0.1f;

      Timer = SamplingTime;
    }

    // 1 = MeshSprite Create
    if (MeshSprite == NULL)
    {
      if (INPUTMGR->KeyIsPressed(KEY_1))
      {
        MeshSprite = FACTORY->CreateSprite();
      }
    }
    else
    {
      if (INPUTMGR->KeyIsDown(KEY_1))
      {
        if (INPUTMGR->KeyIsPressed(KEY_2))
        {
          FACTORY->GiveComponentToObject("Body", MeshSprite);
          MeshSprite->has(Body)->IsStatic = false;
        }
      }
    }

    // 2 = BackBufferSprite Create  CURRENTLY BROKEN
    /*
    if (BackBufferSprite == NULL)
    {
      if (INPUTMGR->KeyIsPressed(KEY_2))
      {
        BackBufferSprite = FACTORY->CreateBackBufferSprite();
      }
    }
    */

    // 3 = AnimatableSprite Create
    if (Animatablesprite == NULL)
    {
      if (INPUTMGR->KeyIsPressed(KEY_3))
      {
        Animatablesprite = FACTORY->CreateAnimatableSprite();
      }
    }
    else
    {
      if (INPUTMGR->KeyIsDown(VK_LEFT))
      {
        Animatablesprite->has(Transform)->Scale -= Vec3(0.1f, 0.1f, 0.1f);
      }
      if (INPUTMGR->KeyIsDown(VK_RIGHT))
      {
        Animatablesprite->has(Transform)->Scale += Vec3(0.1f, 0.1f, 0.1f);
      }
    }

    // 4 = DefaultText Create
    if (Defaulttext == NULL)
    {
      if (INPUTMGR->KeyIsPressed(KEY_4))
      {
        Defaulttext = FACTORY->CreateDefaultText();
      }
    }
    else
    {
      std::ostringstream convert;   // stream used for the conversion

      convert << fps;

      Defaulttext->has(DefaultText)->SetText("FPS: ");
      Defaulttext->has(DefaultText)->Text += convert.str() + "\n";
      convert.str("");
      convert.clear();
     
      convert << SamplingTime;

      Defaulttext->has(DefaultText)->AppendText("- Sampling every ");
      Defaulttext->has(DefaultText)->Text += convert.str() + " seconds -\n\n";
      convert.str("");
      convert.clear();

      Defaulttext->has(DefaultText)->AppendText("Camera Position: (");
      
      
      convert << GRAPHICS->CurrentCamera->has(Transform)->Position.x;
      Defaulttext->has(DefaultText)->Text += convert.str() + ", ";
      convert.str("");
      convert.clear();
      convert << GRAPHICS->CurrentCamera->has(Transform)->Position.y;
      Defaulttext->has(DefaultText)->Text += convert.str() + ", ";
      convert.str("");
      convert.clear();
      convert << GRAPHICS->CurrentCamera->has(Transform)->Position.z;
      Defaulttext->has(DefaultText)->Text += convert.str() + ")\n";
      convert.str("");
      convert.clear();
    }

    if (INPUTMGR->KeyIsPressed(KEY_0))
    {
      WINDOWSSYSTEM->Request_OKMessageBox("Assertion Key '0' has been Pressed.  Terminating OBAL Engine.", "Forced Assertion");
      assert(0);
    }

  }
  
}