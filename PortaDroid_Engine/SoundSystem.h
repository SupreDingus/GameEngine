#pragma once

#include "fmod.hpp"
#include "fmod_errors.h"
#include "EngineBases.h"
#include <iostream>
#include <list>

namespace OBALFramework
{
  enum SoundType
  {
    EFFECT,
    MUSIC
  };

  class SoundSystem : public ISystem
  {
  public:
    void Initialize();
    void Update(float dt);

    void PlayASound(FMOD::Sound* sound);
    void StartMusic(FMOD::Sound* sound);
    void StopMusic();

    void ChangeVolume(float vol, FMOD::Channel* channel);
    void CreateStream(const char* filename, SoundType type);

    void FMODErrorCheck(FMOD_RESULT result);

    //Work with FMOD.
    FMOD::System* system;
    FMOD::Sound* audio;

    //Groups of channels.
    FMOD::ChannelGroup* groupMusic;
    FMOD::ChannelGroup* groupEffects;

    //Channels.
    FMOD::Channel* channelMusic;
    FMOD::Channel* channelEffects;

    //"Libraries" of all sounds/music.
    std::list<std::string> effectLibrary;
    std::list<std::string> musicLibrary;

  private:
    
  };

  extern SoundSystem* SOUNDSYSTEM;
}
