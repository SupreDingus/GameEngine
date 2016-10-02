#pragma once

//Include our math headers
#include <d3dx9.h>
#include <cmath>

typedef D3DXVECTOR2 Vec2;
typedef D3DXVECTOR3 Vec3;
typedef D3DXVECTOR4 Vec4;
typedef D3DXMATRIXA16 Mat4;   // NOTE: D3DXMATRIX is being used by Graphics
typedef D3DXQUATERNION Quat;



namespace OBALFramework
{
  /*returns dot product of 2 vectors*/
  float Dot(const Vec3& a, const Vec3& b);
  /*returns the length squared of a vector*/
  float LengthSq(const Vec3& a);
  /*returns the length of a vector*/
  float Length(const Vec3& a);
  /*normalizes a vector*/
  float Normalize(Vec3& a);
  
  void Mat4Concatenate(Mat4 * pResult, Mat4 * pMat0, Mat4 * pMat1);
  
}