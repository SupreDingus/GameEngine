#pragma once

///Pretransformed Verticies
// Custom Flexible Vertex Format
///#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
// Custom Vertex Struct
///struct CUSTOMVERTEX { FLOAT X, Y, Z, RHW; DWORD COLOR; };

namespace OBALFramework
{
  // Custom Flexible Vertex Format
  #define CUSTOM_FVF     ( D3DFVF_XYZ | D3DFVF_DIFFUSE )
  #define VERTEX2D_FVF  ( D3DFVF_XYZ | D3DFVF_TEX1 )
  #define LINE_FVF      ( D3DFVF_XYZ )
  // Custom Vertex Struct
  struct CUSTOMVERTEX 
  { 
    FLOAT X, Y, Z; 
    DWORD COLOR; 
  };

  struct VERTEX2D
  {
    FLOAT X, Y, Z;
    float tu, tv;   //The texture coordinates used to map the texture to the vertex.
    /// 0.0f,0.0f = Top-Left    1.0f,1.0f = Bottom-Right
  };
}