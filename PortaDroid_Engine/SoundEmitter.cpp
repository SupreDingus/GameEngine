#include "SoundEmitter.h"

namespace OBALFramework
{
  SoundEmitter::SoundEmitter() { }
  SoundEmitter::~SoundEmitter() { }

  void SoundEmitter::PlayASound(const char * soundName)
  {
    if (audioFiles.size() > 0 && audioFiles.count(soundName))
    {
      SOUNDSYSTEM->PlayASound(audioFiles[std::string(soundName)]);
    }
  }

  void SoundEmitter::SetAudio(const char * filename, const char * soundName, SoundType type, float vol, FMOD::Channel * channel)
  {
    SOUNDSYSTEM->CreateStream(filename, type);
    SOUNDSYSTEM->ChangeVolume(vol, channel);

    //myFiles[std::string(soundName)] = audio;
    audio = SOUNDSYSTEM->audio;
  }
}
