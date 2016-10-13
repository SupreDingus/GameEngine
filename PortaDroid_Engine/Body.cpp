// ---------------------------------------------------------------------------
// Project Name   :
// File Name      : Body.cpp
// Author         : Sarah Janzen
// Creation Date  : 2016/09/09
// Purpose        : 
// History        :
// -9/9/16  : created
// © Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
// ---------------------------------------------------------------------------
#include "Body.h"
#include "DebugDraw.h"
#include "Physics.h"

namespace OBALFramework
{
  /*default constructor for Body*/
  Body::Body()
  {
    Position = Vec3(0, 0, 0);
    PrevPosition = Vec3(0, 0, 0);
    Velocity = Vec3(0, 0, 0); 
    Mass = 0.0f;
    InvMass = 0.0f;
    Damping = 0.9f;
    Acceleration = Vec3(0, 0, 0); 
    //BodyShape = NULL;
    ShapeAAB *box = new ShapeAAB(); //default shape
    BodyShape = box;
    Friction = 0.0f;
    Restitution = 0.0f;
    IsStatic = false;
    AccumulatedForce = Vec3(0, 0, 0); 
  }

  Body::Body(Vec3 pos, Vec3 vel, Vec3 accel, /*std::string shape,*/
    Shape::ShapeId shape, 
    float mass, float damping, float fric, float rest, 
    bool isStatic)
  {
    Position     = Vec3(pos.x, pos.y, 0);
    PrevPosition = Vec3(pos.x, pos.y, 0);//Vec3(prevPos.x, prevPos.y, 1);
    Velocity     = Vec3(vel.x, vel.y, 0);
    Acceleration = Vec3(accel.x, accel.y, 0);

    Mass = mass;
    InvMass = 0.0f; //need to invert mass
    Damping = damping;

    //BodyShape->Id = shape;

    Friction = fric;
    Restitution = rest;
    IsStatic = isStatic;

    AccumulatedForce = Vec3(0, 0, 0);

    ShapeAAB *box;
    ShapeCircle *circle;

    //create shape
    switch (shape)
    {
    case Shape::SidBox:
      box = new ShapeAAB();
      BodyShape = box;
      break;
    case Shape::SidCircle:
      circle = new ShapeCircle();
      BodyShape = circle;
      break;
    default: //create box
      box = new ShapeAAB();
      BodyShape = box;
      break;
    }
  }

  /*destructor for Body*/
  Body::~Body()
  {
    OutputDebugStringA("'Body' Component being erased from PHYSICS Bodies (destroying)\n");
    delete BodyShape;
    PHYSICS->Bodies.erase(this);
  }

  /*Set the object's position*/
  /*  If you need to set position, just set the objects position from its transform component
  void Body::SetPosition(Vec3 Pos)
  {
    Position = Pos;
    trans->Position = Pos;
  }
  */

  /*Set the object's velocity*/
  void Body::SetVelocity(Vec3 Vel)
  {
    Velocity = Vel;
  }

  /*Update the object's position*/
  void Body::UpdatePosition()
  {
    trans->Position = Position;
  }

  /*initialize body (the object)*/
  void Body::Initialize()
  {
    OutputDebugStringA("Initializing 'Body' Component.\n");

    // Specify the ComponentTypeId
    TypeId = CT_Body;

    //Get the transform to write results to
    trans = GetOwner()->has(Transform);

    //Get the shape
    //BodyShape = PHYSICS->GetShapeSource(ShapeName);
    //BodyShape = 

    //Get the starting position
    Position = trans->Position;

    //Add this body to the body list
    PHYSICS->Bodies.push_back(this);

    //If mass is zero object is interpreted
    //to be static
    if (Mass > 0.0f)
    {
      IsStatic = false;
      InvMass = 1.0f / Mass;
    }
    else
    {
      IsStatic = true;
      InvMass = 0.0f;
    }

    BodyShape->body = this;
  }

  /**/
  void Body::Integrate(float dt)
  {
    // First get the _Owner's Current Position, then proceed with calculations
    Position = trans->Position;

    //Do not integrate static bodies
    if (IsStatic) return;

    //Store prev position
    PrevPosition = Position;

    //Integrate the position using Euler 
    Position = Position + Velocity * dt; //acceleration term is small

                                         //Determine the acceleration
    Acceleration = PHYSICS->Gravity;
    Vec3 newAcceleration = AccumulatedForce * InvMass + Acceleration;

    //Integrate the velocity
    Velocity = Velocity + newAcceleration * dt;

    //Dampen the velocity for numerical stability and soft drag
    Velocity *= std::pow(Damping, dt);

    //Clamp to velocity max for numerical stability
    if (Dot(Velocity, Velocity) > PHYSICS->MaxVelocitySq) //frm Vmath
    {
      Normalize(Velocity);
      Velocity = Velocity * PHYSICS->MaxVelocity;
    }

    //Clear the force
    AccumulatedForce = Vec3(0, 0, 0);
  }

  /*Adds force to the Body's AccumulatedForce*/
  void Body::AddForce(Vec3 force)
  {
    AccumulatedForce += force;
  }

  /**/
  void Body::DebugDraw()
  {

    if (IsStatic)
    {
      //White
      Drawer::Instance.SetColor(Vec4(1, 1, 1, 1));

      //Draw the shape of the object
      BodyShape->Draw();
    }
    else
    {
      //Red
      Drawer::Instance.SetColor(Vec4(1, 0, 0, 1));

      //Draw the shape of the object
      BodyShape->Draw();

      //Draw the velocity of the object
      Drawer::Instance.SetColor(Vec4(1, 1, 1, 1));
      Drawer::Instance.MoveTo(Position);
      Drawer::Instance.LineTo(Position + Velocity * 0.25f);
    }
  }

  /*
  void Body::Serialize(ISerializer& stream)
  {
    StreamRead(stream, Mass);
    StreamRead(stream, Friction);
    StreamRead(stream, Restitution);

    std::string shapeName;
    StreamRead(stream, shapeName);

    if (shapeName == "Circle")
    {
      ShapeCircle * shape = new ShapeCircle();
      StreamRead(stream, shape->Radius);
      this->BodyShape = shape;
    }

    if (shapeName == "Box")
    {
      ShapeAAB * shape = new ShapeAAB();
      StreamRead(stream, shape->Extents);
      this->BodyShape = shape;
    }
  }
  */

}//!OBALFramework
