#pragma once

namespace OBALFramework
{
    enum ComponentTypeId
  {
    //Invalid
    CT_None = 0,

    //Basic Component for all New Objects
    CT_Transform,

    // Camera Components
    CT_Camera,
    CT_CameraController,

    //Graphics Components
    CT_DirectLight,
    CT_Sprite,
    CT_AnimatableSprite,
    CT_HUDSprite,
    CT_BackBufferSurfaceSprite,
    CT_SpriteText,
    CT_DefaultText,
    CT_Emitter,

    //Physics
    CT_Body,
    CT_Orientation,

    //Game Logic
    CT_Controller,

    //Game Components
    ///

    //Maximum ALWAYS
    CT_MaxComponents
  };
}