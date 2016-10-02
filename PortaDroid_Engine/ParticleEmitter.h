#pragma once
/*****************************************************************************
* File: ParticleEmitter.h
* Author: Mark Rowland
* DOB: 8/29/2016
* Description: Header for the ParticleEmitter component.
*****************************************************************************/
#include <d3d9.h>
#include <d3dx9tex.h>
#include <string>
#include <vector>
#include "VMath.h"
#include "Particle.h"
#include "TextureSource.h"
#include "GameObjectComposition.h"
#include "OBALCore.h"
#include "json/json.h"
#include "json/json-forwards.h"

#include "Serialize.h"

class Particle;

namespace OBALFramework
{
  class Emitter : public GameComponent
  {
    //Make a struct for the vertices.
    struct CUSTOMVERTEX
    {
      D3DXVECTOR3 psPosition;
      D3DCOLOR color;
    };

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

  public:
    Emitter();
    Emitter(LPDIRECT3DDEVICE9 pDevice);
    ~Emitter();

    void Initialize();

    //Add a texture to the emitter.
    void AddTexture(std::string textureName);

    //Set the number of particles, size of vector.
    void SetNumParticles(int particles);

    //Init particles, set emitter position.
    void InitParticles(D3DXVECTOR3 position, D3DCOLOR color);

    //Update particles!
    void Update(float);
    //Render particles!
    void Render();

    //Inline functions.
    inline bool getAlive() { return isAlive; }
    inline DWORD FLOAT_TO_DWORD(FLOAT f) { return *((DWORD*)&f); }

    Emitter * Next;
    Emitter * Prev;

    Transform * transform;

  private:
    std::string fileName;

    //Store a copy of the D3D device.
    //That way, we don't pass it around all the time.
    LPDIRECT3DDEVICE9 emitterDevice;

    //Current particle position and velocity vectors.
    Vec3 currPos;
    Vec3 currVel;

    //Hold the point sprites in this vertex buffer.
    LPDIRECT3DVERTEXBUFFER9 vertexBuffer;

    //Texture that will be applied to each particle.
    TextureSource *texture;

    //Pointer to particle. Used to make an array of particles.
    Particle* particleArray;

    //Number of particles in this emitter.
    int numParticles;

    //Value to determine whether or not the emitter is alive.
    bool isAlive;

    //Private function that will make a vertex buffer.
    LPDIRECT3DVERTEXBUFFER9 CreateVertexBuffer(unsigned int, DWORD, DWORD);
  };

}
