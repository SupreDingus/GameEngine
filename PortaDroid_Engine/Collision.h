// ---------------------------------------------------------------------------
// Project Name   :
// File Name      : Collision.h
// Author         : Sarah Janzen
// Creation Date  : 2016/09/10
// Purpose        : Header for Collision.cpp
// History        :
// -9/10/16  : created
// © Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
// ---------------------------------------------------------------------------
#pragma once

#include "VMath.h"

namespace OBALFramework
{
  class Body;

  struct BodyContact
  {
    Body* Bodies[2];
    Vec3 Movement[2];
    Vec3 ContactNormal;
    float Penetration;
    float Restitution;
    float FrictionCof;

    float SeperatingVelocity;
    float ContactImpulse;
    float CalculateSeparatingVelocity();
  };

  //Base Shape class
  class Shape
  {
  public:
    enum ShapeId
    {
      SidCircle,
      SidBox,
      SidNumberOfShapes
    };
    ShapeId Id;
    Body * body;
    Shape(ShapeId pid) : Id(pid) {};
    //Shape(std::string shapeName);
    virtual void Draw() = 0;
    virtual bool TestPoint(Vec3) = 0;
    //Shape *CreateShape(shape_id);
  };

  //Circle shape.
  class ShapeCircle : public Shape
  {
  public:
    ShapeCircle() : Shape(SidCircle) {};
    float Radius;
    virtual void Draw();
    virtual bool TestPoint(Vec3);
  };

  //Axis Aligned Box Shape
  class ShapeAAB : public Shape
  {
  public:
    ShapeAAB() : Shape(SidBox) {};
    Vec3 Extents;
    virtual void Draw();
    virtual bool TestPoint(Vec3);
  };

  class ContactSet;
  typedef bool(*CollisionTest)(Shape*a, Vec3 at, Shape*b, Vec3 bt, ContactSet*c);

  //The collision database provides collision detection between shape types.
  class CollsionDatabase
  {
  public:
    CollsionDatabase();
    CollisionTest CollsionRegistry[Shape::SidNumberOfShapes][Shape::SidNumberOfShapes];
    bool GenerateContacts(Shape* shapeA, Vec3 poistionA, Shape* shapeB, Vec3 poistionB, ContactSet*c);
    void RegisterCollsionTest(Shape::ShapeId a, Shape::ShapeId b, CollisionTest test);
  };
}
