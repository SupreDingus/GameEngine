/*****************************************************************************
* File: ParticleManager.cpp
* Author: Mark Rowland
* DOB: 8/29/2016
* Description: Particle Manager class implementation.
*****************************************************************************/
#include "ParticleManager.h"

namespace OBALFramework
{

  ParticleManager::ParticleManager()
  {
    OutputDebugStringA("Created the Particle Manager!");
  }

  ParticleManager::~ParticleManager()
  {
    OutputDebugStringA("Destroyed the Particle Manager!");
  }

  bool ParticleManager::Init() { return true; }

  void ParticleManager::Shutdown() { }

  void ParticleManager::CreateEmitter(LPDIRECT3DDEVICE9 device, int numParticles,
    std::string textureName, D3DXVECTOR3 position, D3DCOLOR color)
  {
    //Make a new emitter.
    Emitter *temp = new Emitter(device);

    //Load the texture, set particle numbers.
    temp->AddTexture(textureName);
    temp->SetNumParticles(numParticles);

    temp->InitParticles(position, color);

    //Add the new emitter to the emitter vector.
    emitters.push_back(temp);
  }

  void ParticleManager::Update(float dt)
  {
    //Loop through each emiiter made thus far.
    for (unsigned int i = 0; i < emitters.size(); ++i)
    {
      //Only update alive emitters.
      if (emitters[i]->getAlive())
      {
        emitters[i]->Update(dt);
      }
    }
  }

  void ParticleManager::Render(LPDIRECT3DDEVICE9 device)
  {
    //We need to render after updating.
    for (unsigned int i = 0; i < emitters.size(); ++i)
    {
      //Check for alive, then update if it is.
      if (emitters[i]->getAlive())
      {
        emitters[i]->Render();
      }
    }
  }


  void ParticleManager::RemoveEmitter(Emitter* which)
  {
    //Check to see if it exists. If so, KILL IT.
    if (which)
      delete which;

    return;
  }
}
