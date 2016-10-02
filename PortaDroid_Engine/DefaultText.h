#pragma once

#include "EngineBases.h"
#include "GameObjectComposition.h"

namespace OBALFramework
{
  class DefaultText : public GameComponent
  {
  public:
    DefaultText();
    DefaultText(char * txt, int left, int right, int top, int bottom, UINT format, D3DXCOLOR color);
    ~DefaultText();

    void Initialize();

    // Setting Methods
    void SetText(char * newtxt);
    void AppendText(char * newtxt);

    void SetFormat_TopLeft();
    void SetFormat_Center();
    void SetFormat_Right();

    void Draw(LPD3DXFONT * pFont);


    /// TEXT DRAW VARIABLES
    DefaultText * Next;
    DefaultText * Prev;
    std::string Text;
    int Count;
    RECT TextRect;
    int LetterWidth;
    int LetterHeight;
    UINT TextFormat;
    D3DXCOLOR TextColor;

    Transform * transform;
  };
}