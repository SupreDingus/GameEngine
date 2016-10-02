// ---------------------------------------------------------------------------
// Project Name   : 
// File Name      : VMath.cpp
// Author         : Sarah Janzen
// Creation Date  : 2016/09/17
// Purpose        : Implementation for Vector Math library
// History        :
// -9/17/16 : created
// © Copyright 1996-2016, DigiPen Institute of Technology (USA). All rights reserved.
// ---------------------------------------------------------------------------
#include "VMath.h"

namespace OBALFramework
{
  /*returns dot product of 2 vectors*/
  float Dot(const Vec3& a, const Vec3& b)
  {
    return a.x * b.x + a.y * b.y;
  }

  /*returns the length squared of a vector*/
  float LengthSq(const Vec3& a)
  {
    return a.x * a.x + a.y * a.y;
  }

  /*returns the length of a vector*/
  float Length(const Vec3& a)
  {
    return sqrt(LengthSq(a));
  }

  /*normalizes a vector*/
  float Normalize(Vec3& a)
  {
    float len = Length(a);
    a.x /= len;
    a.y /= len;
    return len;
  }

  void Mat4Concatenate(Mat4 * pResult, Mat4 * pMat0, Mat4 * pMat1)
  {
    Mat4 temp;

    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;

    int m = 4;
    int n = 4;

    int s = 4;
    int t = 4;

    FLOAT result = 0.0f;

    for (i = 0; i < m; ++i)
    {
      for (j = 0; j < t; ++j)
      {
        for (k = 0; k < s; ++k)
        {
          result += pMat0->m[i][k] * pMat1->m[k][j];
        }

        temp.m[i][j] = result;
        result = 0;
      }
    }

    for (i = 0; i < m; ++i)
    {
      for (j = 0; j < n; ++j)
      {
        pResult->m[i][j] = temp.m[i][j];
      }
    }
  }
}
