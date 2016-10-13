// ---------------------------------------------------------------------------
// Project Name   :
// File Name      : Body.h
// Author         : Sarah Janzen
// Creation Date  : 2016/09/09
// Purpose        : Header for Body.cpp
// History        :
// -9/9/16  : created
// © Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
// ---------------------------------------------------------------------------
#pragma once
#include "GameObjectComposition.h"
#include "VMath.h"
#include "Collision.h"
#include "Transform.h"

namespace OBALFramework
{
  //Forward Declarations
  class Transform;
  class Shape;

  class Body : public GameComponent
  {
  public:
    Body();
    Body(Vec3 pos, Vec3 vel, Vec3 accel, /*std::string shape,*/
         Shape::ShapeId shape,
         float mass, float damping, float fric, float rest,
         bool isStatic);
    ~Body();

    void AddForce(Vec3 force);
    void Integrate(float dt);
    //void SetPosition(Vec3 Pos);
    void SetVelocity(Vec3 Vel);
    void UpdatePosition();

    //Draw the object using the debug drawer
    void DebugDraw();

    virtual void Initialize();
    //virtual void Serialize(ISerializer& stream);

    Body *Next;
    Body *Prev;

    Vec3 Position;
    Vec3 PrevPosition;
    Vec3 Velocity;
    Vec3 Acceleration;
    Vec3 AccumulatedForce; //???
    float Mass;
    float InvMass;         //???
    float Restitution;     //???
    float Friction;
    float Damping;         //???


    //body's transform
    Transform *trans;
    //Shape used for collision with this body
    Shape * BodyShape;
    //Static object are immovable fixed objects
    bool IsStatic;
  };

}
