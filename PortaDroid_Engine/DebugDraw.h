// ---------------------------------------------------------------------------
// Project Name   :
// File Name      : DebugDraw.h
// Author         : Sarah Janzen
// Creation Date  : 2016/09/10
// Purpose        : Header file for DebugDraw.cpp
// History        :
// -9/10/16  : created
// © Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
// ---------------------------------------------------------------------------
#pragma once
#include "VMath.h"

namespace OBALFramework
{
  //Global object that provides debug drawing functionality. This enables any system
  //to draw to the screen in a immediate mode method without a dependency on
  //graphics or having to be called in the render loop.
  class Drawer
  {
  public:
    //Set the color of the lines being drawn.
    void SetColor(Vec4 color);
    //Move the draw position.
    void MoveTo(Vec3 newPos);
    //Draw a line between the current position and the provided position.
    void LineTo(Vec3 newPos);
    //Draw a circle using line segments.
    void DrawCircle(Vec3 center, float radius);
    //Draw an axis aligned box.
    void DrawBox(Vec3 center, float size);
    //Draw a line between two points.
    void DrawSegment(Vec3 start, Vec3 end);
    static Drawer Instance;
  private:
    friend class Graphics;
    void Flush();
    struct LineSet
    {
      Vec4 Color;
      unsigned Segments;
    };

    struct LineSegment
    {
      Vec3 A;
      Vec3 B;
    };

    Vec3 WritePosition;
    Vec4 Color;
    unsigned SegmentsDrawn;
    std::vector< LineSet > Sets;
    std::vector< LineSegment > LineSegments;
    void Clear();
  };
}
