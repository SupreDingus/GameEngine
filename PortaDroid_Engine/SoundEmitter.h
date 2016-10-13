#pragma once

#include "GameObjectComposition.h"
#include "SoundSystem.h"
#include <unordered_map>

namespace OBALFramework
{
  class SoundEmitter : public GameComponent
  {
  public:
    SoundEmitter();
    ~SoundEmitter();

    void PlayASound(const char* soundName);
    void SetAudio(const char* filename, const char* soundName, SoundType type, float vol, FMOD::Channel* channel);

    //std::unordered_map<std::string, std::string> myFiles;

  private:
    FMOD::Sound* audio;
    std::unordered_map<std::string, FMOD::Sound*> audioFiles;
  };
}
