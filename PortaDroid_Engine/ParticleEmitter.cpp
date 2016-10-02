/*****************************************************************************
* File: ParticleEmitter.cpp
* Author: Mark Rowland
* DOB: 8/29/2016
* Description: ParticleEmitter implementation.
*****************************************************************************/
#include "ParticleEmitter.h"
#include "Graphics.h"
#include "VMath.h"

namespace OBALFramework
{
  Emitter::Emitter()
  {
    emitterDevice = 0;
    vertexBuffer = 0;
    texture = 0;
    particleArray = 0;

    OutputDebugStringA("Created the Particle Emitter!");
  }

  Emitter::Emitter(LPDIRECT3DDEVICE9 pDevice)
  {
    emitterDevice = pDevice;
    vertexBuffer = 0;
    texture = 0;
    particleArray = 0;

    OutputDebugStringA("Created the Particle Emitter!");
  }

  Emitter::~Emitter()
  {
    if (emitterDevice)
    {
      emitterDevice->Release();
      emitterDevice = 0;
    }

    if (vertexBuffer)
    {
      vertexBuffer->Release();
      vertexBuffer = 0;
    }

    if (particleArray)
    {
      delete[] particleArray;
      particleArray = 0;
    }

    OutputDebugStringA("Destroyed the Particle Emitter!");
  }

  void Emitter::Initialize()
  {
    // Get the DirectX device 
    emitterDevice = GRAPHICS->pDevice;

    // Push this sprite on the Graphics system's spritelist for drawing in the future
    GRAPHICS->ParticleSystemList.push_back(this);

    // Specify the ComponentTypeId for this component
    TypeId = CT_Emitter;
    
    // Get a pointer to Owner Object's Transform component so its easier to reference
    transform = GetOwner()->has(Transform);

    //Set the number of particles for this system
    SetNumParticles(10);
    //Assign the texture
    AddTexture("default64.bmp");

    // Second Initialize
    InitParticles(transform->Position, 0x00bfbfbf);

    // Push this sprite on the Graphics system's spritelist for drawing in the future
    GRAPHICS->ParticleSystemList.push_back(this);

    //TESTING JSONCPP
    fileName = "wew.txt";
    Serializer yup(fileName);
    
    //yup.Send(currPos);
  }

  void Emitter::AddTexture(std::string textureName)
  {
    //do this when I know how to set textures.
    texture = GRAPHICS->GetTextureSource(textureName);
  }

  void Emitter::SetNumParticles(int particles)
  {
    numParticles = particles;
  }

  void Emitter::InitParticles(D3DXVECTOR3 position, D3DCOLOR color)
  {
    particleArray = new Particle[numParticles];

    //Make a vertex buffer for the emitter.
    vertexBuffer = CreateVertexBuffer(numParticles * sizeof(CUSTOMVERTEX),
      D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS,
      D3DFVF_CUSTOMVERTEX);

    //Loop through the number of particles for the emitter.
    //Also, set initial properties, ya dingus.
    for (int i = 0; i < numParticles; ++i)
    {
      //Particle is alive, has the color from the manager, and a position too!
      particleArray[i].alive = true;
      particleArray[i].color = color;
      particleArray[i].currPos = position;
      particleArray[i].timeAlive = 0.0f;

      //Create and set random velocity vector.
      float vecX = ((float)rand() / RAND_MAX);
      float vecY = ((float)rand() / RAND_MAX);
      float vecZ = 0.0f;

      particleArray[i].currVel = D3DXVECTOR3(vecX, vecY, vecZ);
    }
  }

  void Emitter::Update(float dt)
  {
    //Loop through the particles.
    for (int i = 0; i < numParticles; ++i)
    {
      particleArray[i].currPos += particleArray[i].currVel * dt;
      particleArray[i].timeAlive += dt;
     
      if (particleArray[i].timeAlive > 1.0f)
        particleArray[i].alive = false;
    }
  }

  void Emitter::Render()
  {
    //Set the necessary render states.
    emitterDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

    emitterDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    emitterDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);  //In Graphics, "D3DRS_DESTBLEND" is set to "D3DBLEND_INVSRCALPHA"

    //Turn on point sprites.
    emitterDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
    emitterDevice->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);

    //Size to use when the vertex doesn't provide info.
    //This is also the smallest size of the points.
    emitterDevice->SetRenderState(D3DRS_POINTSIZE, FLOAT_TO_DWORD(0.01f));
    emitterDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FLOAT_TO_DWORD(0.01f));

    //These render states control point sprite scaling.
    //emitterDevice->SetRenderState(D3DRS_POINTSCALE_A, FLOAT_TO_DWORD(0.0f));
    //emitterDevice->SetRenderState(D3DRS_POINTSCALE_B, FLOAT_TO_DWORD(0.0f));
    //emitterDevice->SetRenderState(D3DRS_POINTSCALE_C, FLOAT_TO_DWORD(1.0f));

    CUSTOMVERTEX* pointVerticies;

    //Lock the vertex buffer, update particles in it.
    vertexBuffer->Lock(0, numParticles * sizeof(CUSTOMVERTEX),
      (void**)&pointVerticies, D3DLOCK_DISCARD);

    //Loop through the particles.
    for (int i = 0; i < numParticles; ++i)
    {
      pointVerticies->psPosition = particleArray[i].currPos;
      pointVerticies->color = particleArray[i].color;
      pointVerticies++;
    }

    //Unlock the vertex buffer, we're done updating.
    vertexBuffer->Unlock();

    //Set the texture for the particles (uncomment when I have a fix for the texture type mismatch.)
    emitterDevice->SetTexture(0, texture->Texture);
    //Set the vertex stream.
    emitterDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(CUSTOMVERTEX));  ///Graphics already does this in DrawWorld()
    //Set vertex format.
    emitterDevice->SetFVF(D3DFVF_CUSTOMVERTEX);  ///Graphics already does this in DrawWorld()
    //Call DrawPrimitive to render the particles.
    emitterDevice->DrawPrimitive(D3DPT_POINTLIST, 0, numParticles);

    //Set the render states back.
    emitterDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    emitterDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    emitterDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
    emitterDevice->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);

    emitterDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);  //In Graphics, "D3DRS_DESTBLEND" is set to "D3DBLEND_INVSRCALPHA"
  }

  LPDIRECT3DVERTEXBUFFER9 Emitter::CreateVertexBuffer(unsigned int size, DWORD usage, DWORD fvf)
  {
    HRESULT result;

    //Try to make the vertex buffer.
    result = emitterDevice->CreateVertexBuffer(size, usage, fvf, D3DPOOL_DEFAULT, &vertexBuffer, 0);
    if (FAILED(result))
    {
      return 0;
    }

    return vertexBuffer;
  }

}
