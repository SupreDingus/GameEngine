// ---------------------------------------------------------------------------
// Project Name   : 
// File Name      : physics.cpp
// Author         : Sarah Janzen
// Creation Date  : 2016/08/20
// Purpose        : Implementation for the Physics Component
// History        :
// -8/20/16  : created
// -9/9/16   : initialize()
//           : ray constructors and destructors
// -9/10/16  : orientation constructors and destructors
// © Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
// ---------------------------------------------------------------------------
#include "Precompiled.h"
#include "Physics.h"
#include "stdio.h"
#include "Body.h"
#include "Orientation.h"
#include "ComponentCreator.h"
#include "ComponentTypeIds.h"
//#include "DebugDiagnostic.h"


#define PI      3.1415926535897932384626433832795
#define EPSILON 0.0001
#define GRAVITY 9.8f

namespace OBALFramework
{
  //global pointer to Physics.
  Physics * PHYSICS = NULL;

  /*default constructor for Ray*/
  Ray::Ray()
  {
    Start = Vec3(0, 0, 0);
    Direction = Vec3(0, 0, 0);
  }

  /*non-default constructor for Ray*/
  Ray::Ray(Vec3 &start, Vec3 &direction)
  {
    Start.x = start.x;
    Start.y = start.y;
    Start.z = start.z;
    Direction.x = direction.x;
    Direction.y = direction.y;
    Direction.z = direction.z;
  }

  /*destructor for Ray*/
  Ray::~Ray()
  {
  }

  /*default constructor for Physics*/
  Physics::Physics() : Drag(0)
  {
    //ErrorIf(PHYSICS != NULL, "Physics already initialized");
    PHYSICS = this;
    DebugDrawingActive = false;
    TimeAccumulation = 0.0f;
    Gravity = Vec3(0, -GRAVITY, 0);
    MaxVelocity = 1000;
    MaxVelocitySq = MaxVelocity*MaxVelocity;
    PenetrationEpsilon = 0.2f;
    PenetrationResolvePercentage = 0.8f;
    StepModeActive = false;
    AdvanceStep = false;

    //orientation might already be initialized ..
    //Orientation.x = 0;
    //Orientation.y = 0;
    //Orientation.z = 0;
  }

  /*non-default constructor for Physics*/
  Physics::Physics(Vec3& gravity, float drag, Vec3& orientation) : Drag(drag)
  {
    //ErrorIf(PHYSICS != NULL, "Physics already initialized");
    PHYSICS = this;
    Gravity = Vec3(0.0f, -GRAVITY, 0.0f);

    //Orientation.x = orientation.x;
    //Orientation.y = orientation.y;
    //Orientation.z = orientation.z;
  }

  /*destructor for Physics*/
  Physics::~Physics()
  {
    //release anything that is not GameObject based
  }

  void Physics::Initialize()
  {
    OutputDebugStringA("Initializing 'Physics' Component.\n");
    
    //testObjWithMyComp = FACTORY->CreateSprite();
    //FACTORY->GiveComponentToObject("Body", testObjWithMyComp);
  }

  /*returns distance to the closest object in the ray*/
  float Ray::CastRayFirst() 
  {
    return Length(Direction);
  }

  /**/
  void Physics::IntegrateBodies(float dt)
  {
    for (BodyIterator it = Bodies.begin(); it != Bodies.end(); ++it)
    {
      it->Integrate(dt);
    }
  }

  /**/
  void Physics::Update(float dt)
  {
    const float TimeStep = 1.0f / 60.0f;

    if (!StepModeActive)
    {
      TimeAccumulation += dt;
      TimeAccumulation = min(TimeAccumulation, TimeStep * 5);
      if (TimeAccumulation > TimeStep)
      {
        TimeAccumulation -= TimeStep;
        Step(TimeStep);
      }

    }
    else
    {
      TimeAccumulation = 0.0f;
      if (AdvanceStep)
      {
        Step(TimeStep);
        AdvanceStep = false;
      }
    }

    if (DebugDrawingActive)
      DebugDraw();

  }

  /**/
  void Physics::DetectContacts(float dt)
  {
    BodyIterator bodyA = Bodies.begin();
    BodyIterator lastBody = Bodies.last(); //end - 1

    //Broad phase should be added this is N^2
    for (; bodyA != lastBody; ++bodyA)
    {
      BodyIterator bodyB = bodyA;
      ++bodyB;
      for (; bodyB != Bodies.end(); ++bodyB)
      {
        //Do not collide static bodies with other static bodies
        if (!bodyA->IsStatic || !bodyB->IsStatic)
        {
          Collsion.GenerateContacts((bodyA)->BodyShape, (bodyA)->Position, (bodyB)->BodyShape, (bodyB)->Position, &Contacts);
        }
      }
    }
  }

  /**/
  void Physics::UpdatePosition()
  {
    //Commit all physics updates
    for (BodyIterator it = Bodies.begin(); it != Bodies.end(); ++it)
    {
      it->UpdatePosition();
    }

    //Broadcast physics collision messages AFTER physics has updated the bodies
    for (unsigned i = 0; i<Contacts.NumberOfContacts; ++i)
    {
      BodyContact* contact = &Contacts.contactArray[i];
      MessageCollision messageCollision;
      messageCollision.ContactNormal = contact->ContactNormal;
      messageCollision.Impulse = contact->ContactImpulse;
      messageCollision.CollidedWith = contact->Bodies[1]->GetOwner();
      contact->Bodies[0]->GetOwner()->ThrowMessage(&messageCollision);
      //contact->Bodies[0]->GetOwner()->SendMessage(&messageCollision);
      if (contact->Bodies[1] != NULL)
      {
        messageCollision.ContactNormal = -contact->ContactNormal;
        messageCollision.Impulse = contact->ContactImpulse;
        messageCollision.CollidedWith = contact->Bodies[0]->GetOwner();
        contact->Bodies[1]->GetOwner()->ThrowMessage(&messageCollision);
        //contact->Bodies[1]->GetOwner()->SendMessage(&messageCollision);
      }
    }
  }

  /**/
  void Physics::DebugDraw()
  {
    for (BodyIterator it = Bodies.begin(); it != Bodies.end(); ++it)
    {
      it->DebugDraw();
    }
  }

  /**/
  /*
  void Physics::SendMessage(Message * m)
  {
    if (m->MessageId == Mid::ToggleDebugInfo)
    {
      ToggleDebugDisplay * debugM = (ToggleDebugDisplay*)m;
      DebugDrawingActive = debugM->DebugActive;
    }
  }
  */

  /**/
  void Physics::Step(float dt)
  {
    IntegrateBodies(dt);
    Contacts.Reset();
    DetectContacts(dt);
    Contacts.ResolveContacts(dt);
    UpdatePosition();
  }

  /**/
  GOC * Physics::TestPoint(Vec3 testPosition)
  {
    for (BodyIterator it = Bodies.begin(); it != Bodies.end(); ++it)
    {
      if (it->BodyShape->TestPoint(testPosition))
        return it->GetOwner();
    }

    return NULL;
  }

  void Physics::RegisterAppropriateComponents()
  {
    //COMPONENT REGISTRATION
    RegisterComponent(Body);
    RegisterComponent(Orientation);
    RegisterComponent(PlayerController);
  }

  /**/
  /*
  ShapeSource* Physics::GetShapeSource(std::string shape)
  {
    ShapeMap::iterator it = TextureSources.find(shape);
    if (it != ShapeSources.end())
      return it->second;
    else
      return NULL;
  } // END GetShapeSource()
  */
} //!OBALFramework