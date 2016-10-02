#pragma once
#include "VMath.h"
#include "Transform.h"
#include "GameObjectComposition.h"

namespace OBALFramework
{

  class Orientation : public GameComponent
  {
  public:
    Orientation();
    Orientation(Vec3 &localUp, Vec3 &localRight, Vec3 &worldUp, Vec3 &worldRight);
    ~Orientation();
    virtual void Initialize();
    void Update(); //should check the object's transform 

    void LookAtPoint(Vec3 point); //orientates towards the point
    void LookAtDirection(Vec3 direction); //orientates to the given direction

    //Vec3 Local(); //returns the value in Local
    //void Local(Vec3 loc); //sets the value in Local
    //Vec3 World(); //returns the value in World
    //void World(Vec3 world); //sets the value in world

    Transform * transform;  //transform to use for Orientation

    //WORLD ORIENTATION
    Vec3 WorldUp;
    Vec3 WorldRight;

    //LOCAL ORIENTATION
    Vec3 LocalUp;
    Vec3 LocalRight;

    Orientation *Next;
    Orientation *Prev;

  private:

    Vec3 position;
  };
}
