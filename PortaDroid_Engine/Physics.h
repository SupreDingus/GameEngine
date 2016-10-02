// ---------------------------------------------------------------------------
// Project Name   : 
// File Name      : physics.h
// Author         : Sarah Janzen
// Creation Date  : 2016/08/20
// Purpose        : Header file for physics.cpp
// History        :
// -3/6/16 : created
// © Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
// ---------------------------------------------------------------------------
/*
Things to include:
gravity effects on game objects
capsule collider
ray casting for collisions & teleportation
drag
orientation component
parenting/heirarchy
*/

#pragma once

//#include "math.h"
//#include "LineSegment2D.h"
#include <string>
#include "VMath.h"
#include "Containers.h"
#include "GameObjectComposition.h" 
#include "Transform.h"
#include "Factory.h"
#include "Message.h"

#include "Collision.h"
#include "Body.h"
#include "Orientation.h"
#include "Resolution.h"

/*
enum Collision_State
{
  NoCollision = 0,
  Collision = 1
};
*/


namespace OBALFramework
{

  class Ray
  {
  public:
    Ray();
    Ray(Vec3 &start, Vec3 &direction);
    ~Ray();

    //need for ray cast:
    //transform of obj
    //orientation -- direction of ray
    //area or size of obj

  private:
    Vec3 Start;     //starting position
    Vec3 Direction; //direction of ray
    //Vec3 Vector;

    float CastRayFirst(); //returns distance to the closest object in the ray
    //or should this return the obj type or something to that extent?
  };

  class MessageCollision : public Message
  {
  public:
    MessageCollision() : Message(Mid::Collision) {};
    Vec3 ContactNormal;
    float Impulse;
    GOC * CollidedWith;
  };

  //----------------------------------------------------------------------------
  //----------------------------------------------------------------------------
  //PHYSICS COMPONENT

  //class Physics : public GameComponent
  class Physics : public ISystem
  {
  public:
    

    Physics();  
    Physics(Vec3& gravity, float drag, Vec3& orientation);
    ~Physics(); 
    virtual void Update(float dt);
    virtual std::string GetName() { return "Physics"; }
    //void SendMessage(Message * m);
    GOC * TestPoint(Vec3 testPosition); //GOC := GameObjectComposition
    void Initialize();
    //void Draw();    //don't know if ill need this or not 

	GOC *testObjWithMyComp;
	/*
	testObjWithMyComp->has(Body)->Position.x; //to use
	testObjWithMyComp->has(Orientation)->WorldUp.x; //
	*/

    bool AdvanceStep;
    bool StepModeActive;

    //BODY LIST
    typedef ObjectLinkList<Body>::iterator BodyIterator; //to iterate through Bodies
    ObjectLinkList<Body> Bodies;                         //list of Bodies

    //BODY SHAPES
    //Stored shapes
    /*
    typedef std::map<std::string, Shape*> ShapeMap;
    ShapeMap	ShapeSources;
    */

    //ORIENTATION LIST
    typedef ObjectLinkList<Orientation>::iterator OrientationIterator; //to iterate through Orientation Components
    ObjectLinkList<Orientation> OrientationList;                       //list of Orienation Components

  private:
    void IntegrateBodies(float dt);
    void DetectContacts(float dt);
    void UpdatePosition();
    void DebugDraw();
    void Step(float dt);
    void RegisterAppropriateComponents();
    bool DebugDrawingActive;
    float TimeAccumulation;
    CollsionDatabase Collsion;
    ContactSet Contacts;

  public:
    Vec3 Gravity; 
    float Drag;
    float MaxVelocity;
    float MaxVelocitySq;
    Orientation orientation;

    //Used in Resolution.cpp:
    float PenetrationEpsilon;           //Position correction tolerance
    float PenetrationResolvePercentage; //Position correction resolve percentage

    //int friction;
    //2D side scrolling coll detections
    //floor detection
    //coll w/ ground
    //lerp???
    //helper math functions???
  };

  //A global pointer to the Physics system
  extern Physics* PHYSICS;
}
