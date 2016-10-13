#pragma once

#include <string>
#include "VMath.h"
#include "TextureSource.h"
#include "GameObjectComposition.h"
#include "InputManager.h"

namespace OBALFramework
{
  class EngineProofPresentationObject : public GameComponent
  {
  public:
    EngineProofPresentationObject();
    ~EngineProofPresentationObject();

    void Initialize();
    void Update(float dt);

    float defaultsamples = 0;
    int samplesperformed = 0;
    float Timer = 0.0f;
    float SamplingTime = 0.5f;
    float fps = 0.0f;

    GOC * MeshSprite;
    GOC * BackBufferSprite;
    GOC * HUDSprite;
    GOC * Animatablesprite;
    GOC * Defaulttext;

  private:

  };
}