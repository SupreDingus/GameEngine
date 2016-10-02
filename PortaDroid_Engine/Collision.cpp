// ---------------------------------------------------------------------------
// Project Name   :
// File Name      : Collision.cpp
// Author         : Sarah Janzen
// Creation Date  : 2016/09/10
// Purpose        : Collision detection file
// History        :
// -9/10/16  : created
// © Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
// ---------------------------------------------------------------------------
#include "Precompiled.h"
#include "Collision.h"
#include "Physics.h"
#include "DebugDraw.h"

namespace OBALFramework
{
  /*
  Shape::Shape(std::string shapeName)
  {
    //set shape to shapeName
  }
  */
  //Functions that determine how restitution and friction 
  //are determined between body pairs.

  //There really is no physically accurate way to mathematically combine 
  //these factors. A material does not have a single friction or restitution.
  //The physically accurate way is to have a large database that
  //define the relationship between all physical materials or simulate 
  //molecular physics.

  float DetermineRestitution(Body * a, Body * b)
  {
    return 	min(a->Restitution, b->Restitution);
  }

  float DetermineFriction(Body * a, Body * b)
  {
    return sqrt(a->Friction*b->Friction);
  }

  void ShapeCircle::Draw()
  {
    Drawer::Instance.DrawCircle(body->Position, Radius);
  }

  bool ShapeCircle::TestPoint(Vec3 testPoint)
  {
    Vec3 delta = body->Position - testPoint;
    float dis = Normalize(delta);
    if (dis < Radius)
      return true;
    else
      return false;
  }


  void ShapeAAB::Draw()
  {
    //check w component on the Vec3s
    Drawer::Instance.MoveTo(body->Position + Vec3(Extents.x, Extents.y, 0));
    Drawer::Instance.LineTo(body->Position + Vec3(-Extents.x, Extents.y, 0));
    Drawer::Instance.LineTo(body->Position + Vec3(-Extents.x, -Extents.y, 0));
    Drawer::Instance.LineTo(body->Position + Vec3(Extents.x, -Extents.y, 0));
    Drawer::Instance.LineTo(body->Position + Vec3(Extents.x, Extents.y, 0));
    //Drawer::Instance.Flush();
  }

  bool ShapeAAB::TestPoint(Vec3 testPoint)
  {
    Vec3 delta = body->Position - testPoint;
    if (fabs(delta.x) < Extents.x)
    {
      if (fabs(delta.y) < Extents.y)
      {
        return true;
      }
    }
    return false;
  }

  /////////////////////Collsion Detection Functions////////////////////

  bool DetectCollisionCircleCircle(Shape*a, Vec3 at, Shape*b, Vec3 bt, ContactSet*c)
  {
    ShapeCircle * spA = (ShapeCircle*)a;
    ShapeCircle * spB = (ShapeCircle*)b;

    Vec3 positionDelta = at - bt;
    float disSquared = LengthSq(positionDelta);
    float combinedRadius = spA->Radius + spB->Radius;

    //Is the squared distance between the two sphere less
    //then the squared radius?
    if (disSquared < combinedRadius*combinedRadius)
    {
      //Get not squared values

      Vec3 normal;
      float penetration;
      if (disSquared == 0.0f)
      {
        //Right on top of each other
        penetration = spA->Radius;
        normal = Vec3(1, 0, 0);  //this is a vector right?
      }
      else
      {
        float dis = Normalize(positionDelta);
        penetration = combinedRadius - dis;
        normal = positionDelta;
      }

      //Add a contact
      BodyContact * contact = c->GetNextContact();
      contact->Bodies[0] = spA->body;
      contact->Bodies[1] = spB->body;
      contact->ContactNormal = positionDelta;//A to B
      contact->Penetration = penetration;
      contact->Restitution = DetermineRestitution(a->body, b->body);
      contact->FrictionCof = DetermineFriction(a->body, b->body);
      return true;
    }
    else
      return false;


    return false;
  }

  bool  DetectCollisionCircleAABox(Shape*a, Vec3 at, Shape*b, Vec3 bt, ContactSet*c)
  {
    ShapeCircle * spA = (ShapeCircle*)a;
    ShapeAAB * boxB = (ShapeAAB*)b;

    float radius = spA->Radius;
    Vec3 p = at;
    Vec3 minV(bt.x - boxB->Extents.x, bt.y - boxB->Extents.y, 0);
    Vec3 maxV(bt.x + boxB->Extents.x, bt.y + boxB->Extents.y, 0);

    Vec3 q;
    for (int i = 0; i < 2; ++i)
    {
      float v = p[i];
      if (v < minV[i]) v = minV[i];
      if (v > maxV[i]) v = maxV[i];
      q[i] = v;
    }

    Vec3 delta = p - q;
    float disSq = LengthSq(delta);
    if (disSq < radius * radius)
    {
      if (disSq == 0.0f)
      {
        //internal
        Vec3 bdelta = at - bt;

        float xd = radius + boxB->Extents.x - fabs(bdelta.x);
        float yd = radius + boxB->Extents.y - fabs(bdelta.y);

        Vec3 normal;
        float penetration;

        if (xd < yd)
        {
          normal = bdelta.x < 0 ? Vec3(-1, 0, 1) : Vec3(1, 0, 1);
          penetration = xd;
        }
        else
        {
          normal = bdelta.y < 0 ? Vec3(0, -1, 1) : Vec3(0, 1, 1);
          penetration = yd;
        }

        BodyContact * contact = c->GetNextContact();
        contact->Bodies[0] = spA->body;
        contact->Bodies[1] = boxB->body;
        contact->ContactNormal = normal;
        contact->Penetration = penetration;
        contact->Restitution = DetermineRestitution(a->body, b->body);
        contact->FrictionCof = DetermineFriction(a->body, b->body);
      }
      else
      {
        float dis = Normalize(delta);
        BodyContact * contact = c->GetNextContact();
        contact->Bodies[0] = spA->body;
        contact->Bodies[1] = boxB->body;
        contact->ContactNormal = delta;
        contact->Penetration = -(dis - spA->Radius);
        contact->Restitution = DetermineRestitution(a->body, b->body);
        contact->FrictionCof = DetermineFriction(a->body, b->body);
      }
    }
    return false;
  }

  bool  DetectCollisionAABoxAABox(Shape*a, Vec3 at, Shape*b, Vec3 bt, ContactSet*c)
  {
    ShapeAAB * boxA = (ShapeAAB*)a;
    ShapeAAB * boxB = (ShapeAAB*)b;

    //Check X
    Vec3 positionDelta = at - bt;
    float xDiff = boxA->Extents.x + boxB->Extents.x - fabs(positionDelta.x);

    //Boxes overlapping on x-axis?
    if (0 < xDiff)
    {
      float yDiff = boxA->Extents.y + boxB->Extents.y - fabs(positionDelta.y);

      //Boxes overlapping on y-axis?
      if (0 < yDiff)
      {
        //Which axis is overlapping less? that is the axis we want
        //to use for the collision.
        if (xDiff < yDiff)
        {
          //X is smallest
          BodyContact * contact = c->GetNextContact();
          Vec3 normal = positionDelta.x < 0 ? Vec3(-1, 0, 1) : Vec3(1, 0, 1);
          contact->Bodies[0] = boxA->body;
          contact->Bodies[1] = boxB->body;
          contact->ContactNormal = normal;
          contact->Penetration = xDiff;
          contact->Restitution = DetermineRestitution(a->body, b->body);
          contact->FrictionCof = DetermineFriction(a->body, b->body);
          return true;
        }
        else
        {
          //Y is smallest
          BodyContact * contact = c->GetNextContact();
          Vec3 normal = positionDelta.y < 0 ? Vec3(0, 1, 1) : Vec3(0, -1, 1);
          contact->Bodies[1] = boxA->body;
          contact->Bodies[0] = boxB->body;
          contact->ContactNormal = normal;
          contact->Penetration = yDiff;
          contact->Restitution = DetermineRestitution(a->body, b->body);
          contact->FrictionCof = DetermineFriction(a->body, b->body);
          return true;
        }
      }
    }
    return false;
  }


  //Auxiliary
  bool  DetectCollisionBoxCircle(Shape*a, Vec3 at, Shape*b, Vec3 bt, ContactSet*c)
  {
    return DetectCollisionCircleAABox(b, bt, a, at, c);
  }


  CollsionDatabase::CollsionDatabase()
  {
    //Register collision tests for all the shape types
    RegisterCollsionTest(Shape::SidCircle, Shape::SidCircle, DetectCollisionCircleCircle);
    RegisterCollsionTest(Shape::SidBox, Shape::SidBox, DetectCollisionAABoxAABox);
    RegisterCollsionTest(Shape::SidCircle, Shape::SidBox, DetectCollisionCircleAABox);
    RegisterCollsionTest(Shape::SidBox, Shape::SidCircle, DetectCollisionBoxCircle);
  }

  void CollsionDatabase::RegisterCollsionTest(Shape::ShapeId a, Shape::ShapeId b, CollisionTest test)
  {
    CollsionRegistry[a][b] = test;
  }

  bool CollsionDatabase::GenerateContacts(Shape* shapeA, Vec3 poistionA, Shape* shapeB, Vec3 poistionB, ContactSet*c)
  {
    return (*CollsionRegistry[shapeA->Id][shapeB->Id])(shapeA, poistionA, shapeB, poistionB, c);
  }
}
