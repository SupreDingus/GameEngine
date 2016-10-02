#pragma once
/*****************************************************************************
* File: Particle.h
* Author: Mark Rowland
* DOB: 8/26/2016
* Description: Header for the Particle Class.
*****************************************************************************/

#include <d3d9.h>
#include <d3dx9tex.h>

namespace OBALFramework
{
  class Particle
  {
  public:
    Particle();
    ~Particle();

    //Current position / velocity.
    D3DXVECTOR3 currPos;
    D3DXVECTOR3 currVel;

    //Color of particle.
    D3DCOLOR color;

    //Time it's been alive.
    float timeAlive;

    //Alive or no.
    bool alive;
  };
}
