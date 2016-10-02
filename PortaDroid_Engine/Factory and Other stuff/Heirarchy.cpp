#include "Heirarchy.h"

namespace OBALFramework
{
  Heirarchy::Heirarchy()
  {
  }
  Heirarchy::~Heirarchy()
  {
    EmptyChildren();
  }


  void Heirarchy::Update()
  {
    for (int i = 0; i < Children.size(); ++i)
    {
      if (Children[i]->tempTransform != NULL)
      {
        //Vec3 change = Children[i]->tempTransform->Position - Children[i]->tempTransform->LastPos;
        //Children[i]->tempTransform->Position += Owner->tempTransform->Position;
      }
    }
  }


  void Heirarchy::AddChild(Sprite * child)
  {
    OutputDebugStringA("ADDING CHILD: Heirarchy\n");
    Children.push_back(child);
    OutputDebugStringA(Children[0]->SpriteName.c_str());
  }
  void Heirarchy::RemoveChild(Sprite child)
  {
  }

  void Heirarchy::EmptyChildren()
  {
    Children.clear();
  }


}