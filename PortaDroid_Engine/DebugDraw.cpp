// ---------------------------------------------------------------------------
// Project Name   :
// File Name      : DebugDraw.cpp
// Author         : Sarah Janzen
// Creation Date  : 2016/09/10
// Purpose        : The debug drawer.
// History        :
// -9/10/16  : created
// © Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
// ---------------------------------------------------------------------------
#include "Precompiled.h"
#include "DebugDraw.h"

namespace OBALFramework
{
  Drawer Drawer::Instance;

  void Drawer::MoveTo(Vec3 v)
  {
    WritePosition = v;
  }

  void Drawer::SetColor(Vec4 color)
  {
    Flush();
    Color = color;
  }

  void Drawer::LineTo(Vec3 newPosition)
  {
    LineSegment seg = { WritePosition, newPosition };
    LineSegments.push_back(seg);
    ++SegmentsDrawn;
    WritePosition = newPosition;
  }

  void Drawer::Flush()
  {
    if (SegmentsDrawn > 0)
    {
      LineSet set = { Color , SegmentsDrawn };
      Sets.push_back(set);
      SegmentsDrawn = 0;
    }
  }

  void Drawer::Clear()
  {
    SegmentsDrawn = 0;
    Sets.clear();
    LineSegments.clear();
  }

  void Drawer::DrawSegment(Vec3 start, Vec3 end)
  {
    MoveTo(start);
    LineTo(end);;
  }

  void  Drawer::DrawCircle(Vec3 center, float radius)
  {
    const unsigned numberOfSegments = 16;
    const float increment = 2.0f * (D3DX_PI) / float(numberOfSegments);

    //make sure that the z component is correct
    float theta = 0.0f;
    MoveTo(center + radius * Vec3(cosf(theta), sinf(theta), 1));
    for (unsigned i = 1; i <= numberOfSegments; ++i)
    {
      LineTo(center + radius * Vec3(cosf(increment*i), sinf(increment*i), 1));
    }
  }

  void  Drawer::DrawBox(Vec3 center, float size)
  {
    //make sure that the z component is correct
    float halfSize = size / 2.0f;
    MoveTo(center + Vec3(halfSize, halfSize, 0));
    LineTo(center + Vec3(halfSize, -halfSize, 0));
    LineTo(center + Vec3(-halfSize, -halfSize, 0));
    LineTo(center + Vec3(-halfSize, halfSize, 0));
    LineTo(center + Vec3(halfSize, halfSize, 0));
  }


}
