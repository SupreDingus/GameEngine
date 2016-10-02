#include "Orientation.h"
#include "Physics.h"

namespace OBALFramework
{
  /*default constructor for Orientation*/
  Orientation::Orientation()
  {
    LocalUp = Vec3(0, 1, 0);
    LocalRight = Vec3(1, 0, 0);

    WorldUp = Vec3(0, 1, 0);
    WorldRight = Vec3(1, 0, 0);
  }

  /*non-default constructor for Orientation*/
  Orientation::Orientation(Vec3 &localUp, Vec3 &localRight, Vec3 &worldUp, Vec3 &worldRight) : LocalUp(localUp), LocalRight(localRight), WorldUp(worldUp), WorldRight(worldRight)
  {
  }

  /*deconstructor for Orientation*/
  Orientation::~Orientation()
  {
    OutputDebugStringA("'Orientation' Component being erased from PHYSICS OrientationList (destroying)\n");
    PHYSICS->OrientationList.erase(this);
  }

  /*initialize orientation*/
  void Orientation::Initialize()
  {
    OutputDebugStringA("Initializing 'Orientation' Component.\n");

    // Specify the ComponentTypeId
    TypeId = CT_Orientation;

    //Get the transform to write results to
    transform = GetOwner()->has(Transform);

    //Add this Orientation to the Orientation List
    PHYSICS->OrientationList.push_back(this);

    //BodyShape->body = this;
  }

  void Orientation::Update()
  {
    transform = GetOwner()->has(Transform);
  }

  //orientates towards the point
  void Orientation::LookAtPoint(Vec3 point)
  {
    //rotate obj to face point:

    // to find the direction:
    //VecAB = PointB - PointA 
    Vec3 direction = point - transform->Position;

    LookAtDirection(direction);
  }

  //orientates to the given direction
  void Orientation::LookAtDirection(Vec3 direction)
  {
    //w component is 0 -> is a vector

    //rotate obj to face in a direction

    //translate to origin 
    //find the angle to the "point" (even though it's a vector) & set it

    //transform->SetOwner.Rotation = direction; //im not really sure if this'll work
  }
}
