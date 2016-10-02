#pragma once
/*****************************************************************************
* File: ParticleManager.h
* Author: Mark Rowland
* DOB: 8/29/2016
* Description: Header for the Particle Manager class.
*****************************************************************************/
#include <vector>
#include <string>
#include "ParticleEmitter.h"
#include "Particle.h"

//forward declarations.
class Particle;
class Emitter;

namespace OBALFramework
{

  class ParticleManager
  {
  public:
    ParticleManager();
    ~ParticleManager();

    bool Init();
    void Shutdown();

    void CreateEmitter(LPDIRECT3DDEVICE9 device,
      int numParticles, std::string textureName, D3DXVECTOR3 position,
      D3DCOLOR color);

    //void RemoveEmitter(int emitterNum);
    void RemoveEmitter(Emitter* which);

    void Update(float);
    void Render(LPDIRECT3DDEVICE9 device);

  private:
    std::vector<Emitter*> emitters;
  };

}
