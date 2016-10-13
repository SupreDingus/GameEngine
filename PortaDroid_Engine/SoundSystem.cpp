#include "SoundSystem.h"

namespace OBALFramework
{
  void SoundSystem::Initialize()
  {
    //Variables!
    FMOD_RESULT result;
    unsigned int version;
    int numDrivers;
    FMOD_SPEAKERMODE speakerMode;
    FMOD_CAPS caps;
    char name[256];

    //Make the FMOD interface object.
    result = FMOD::System_Create(&system);
    FMODErrorCheck(result);

    //Check the version of FMOD.
    result = system->getVersion(&version);
    FMODErrorCheck(result);

    //Say if we're using an old version if FMOD, if necessary.
    if (version < FMOD_VERSION)
    {
      std::cout << "Error: Old version of FMOD being used." << std::endl;
      return;
    }

    //Get number of sound cards.
    result = system->getNumDrivers(&numDrivers);
    FMODErrorCheck(result);

    //If there's no sound cards, turn off sound.
    if (numDrivers == 0)
    {
      result = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
      FMODErrorCheck(result);
    }
    else
    {
      //get the capabilities of the default sound card.
      result = system->getDriverCaps(0, &caps, 0, &speakerMode);
      FMODErrorCheck(result);

      //Set the speaker mode.
      result = system->setSpeakerMode(speakerMode);
      FMODErrorCheck(result);
    }

    //If acceleration is set to off, increase buffer size.
    if (caps & FMOD_CAPS_HARDWARE_EMULATED)
    {
      result = system->setDSPBufferSize(1024, 10);
      FMODErrorCheck(result);
    }

    //Get the driver name.
    result = system->getDriverInfo(0, name, 256, 0);
    FMODErrorCheck(result);

    //Change the format for SigmaTel devices.
    if (strstr(name, "SigmaTel"))
    {
      result = system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT,
        0, 0, FMOD_DSP_RESAMPLER_LINEAR);
      FMODErrorCheck(result);
    }

    //We can initialize FMOD now.
    result = system->init(100, FMOD_INIT_NORMAL, 0);

    //If the speaker mode isn't supported by the sound card,
    //change it and try again.
    if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
    {
      result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
      FMODErrorCheck(result);

      result = system->init(100, FMOD_INIT_NORMAL, 0);
    }

    FMODErrorCheck(result);
  }

  void SoundSystem::Update(float dt)
  {
    system->update();
  }

  void SoundSystem::PlayASound(FMOD::Sound * sound)
  {
    FMODErrorCheck(system->playSound(FMOD_CHANNEL_FREE, sound, false, &channelEffects));
  }

  void SoundSystem::StartMusic(FMOD::Sound * sound)
  {
    FMODErrorCheck(system->playSound(FMOD_CHANNEL_FREE, sound, false, &channelMusic));

    channelMusic->setMode(FMOD_LOOP_NORMAL);
    channelMusic->setLoopCount(-1);
  }

  void SoundSystem::StopMusic()
  {
    FMODErrorCheck(channelMusic->setPaused(true));
  }

  void SoundSystem::ChangeVolume(float vol, FMOD::Channel * channel)
  {
  }

  void SoundSystem::CreateStream(const char * fileName, SoundType type)
  {
    //Open a stream
    FMODErrorCheck(system->createSound(fileName, FMOD_DEFAULT, 0, &audio));

    //Check the given type.
    if (type == EFFECT)
    {
      FMODErrorCheck(system->playSound(FMOD_CHANNEL_FREE, audio, true, &channelEffects));
      effectLibrary.push_back(fileName);
    }
    else
    {
      FMODErrorCheck(system->playSound(FMOD_CHANNEL_FREE, audio, true, &channelMusic));
      musicLibrary.push_back(fileName);
    }
  }

  void SoundSystem::FMODErrorCheck(FMOD_RESULT result)
  {
    if (result != FMOD_OK)
    {
      std::cout << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
      exit(-1);
    }
  }
}
