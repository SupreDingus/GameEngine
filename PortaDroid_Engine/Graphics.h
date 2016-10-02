///////////////////////////////////////////////////////////////////////////////////////////////////
// TODO:
// ---Set up SpriteSources so they are drawn similarly to how textures are drawn (ObjectLinkedList)
// ---
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "EngineBases.h"
// Graphics related Components
#include "Camera.h"
#include "DefaultText.h"
#include "TextureSource.h"
#include "HUDSprite.h"
#include "Sprite.h"   //Has SpriteSource.h in it
#include "AnimatableSprite.h"
#include "BackBufferSurfaceSprite.h"
#include "DirectLight.h"
#include "ParticleEmitter.h"

namespace OBALFramework
{
  //Forward Declaration of Graphics Objects
  //class Camera;

	class Graphics : public ISystem
	{
  public:
		Graphics();
		~Graphics();
    virtual std::string GetName() { return "GRAPHICS"; }

    // Sets the window handle and screen size variables
    void SetWindowProperties(HWND hWnd, int screenWidth, int screenHeight);

    //Get a texture asset. Will return null if texture is not loaded
    ///IDirect3DTexture9* GetTexture(std::string);

    // Returns a texture that's in the TextureMap
    TextureSource * GetTextureSource(std::string texture);
    SpriteSource * GetSpriteSource(std::string spritesource);


  private:    /// Private member functions
    // Sets up DirectX and prepares it for use
    void Initialize();
    // Gets Video Adapter Info of this computer's specs
    void GetAdapterInfo();
    // Creates and initializes the DirectX device, loads graphical assests, etc.
    void init_graphics(void);

    // Executes codes when a registered message is received
    virtual void ThrowMessage(Message* message);

    // Update every game loop
    void Update(float dt);    //Virtual ISystem Function

    // Register all Components that this system will handle and need upon creation an initialization
    void RegisterAppropriateComponents();

    // A main function that draws everything
    void DrawWorld(void);
    // Upon DrawWorld(), set the new camera matricies
    void SetCameraMatrices(void);

    /// DRAWING FUNCTIONS FOR OBJECTS
    // Draws default texts that are created by directx
    void DrawDefaultTexts();
    void DrawHUDSprites();
    /*IDirect3DSurface9 * */
    void DrawSprites();
    void DrawAnimatableSprites();
    void DrawBackBufferSurfaceSprites();

    void DrawDirectLights();

    // Load all graphical assests (Bitmaps, shaders, textures, etc.)
    void LoadAssets();
    // Loads a texture at filename
    void LoadTexture(const std::string& filename, unsigned int framesperrow, unsigned int numofrows, 
                      unsigned int framerate);
    void LoadSpriteSource(const std::string & filename, unsigned int framesperrow, unsigned int numofrows, 
                            unsigned int framerate);


    Vec2 WorldPositontoScreenPosition(Vec3 point3D);


    // Cleanup and shutdown DirectX
    void ShutDownD3D(void);


    ///////////////////////////////////////////////////////////////////////////////////////////////
    /// DEBUG TRACKING METHODS


    void Debug_TrackCamera();


  public:   /// Public Member Variables
    HWND HWnd;          // Handle of the Game Window
    int ScreenWidth;    // Width of the Window
    int ScreenHeight;   // Height of the Window

    // The following two Ortho variables are only used when the camera is set to Orthographic mode
    // (when "#define Ortho_Cam" is set to 1 at the top of Graphics.cpp)
    FLOAT WorldScale;
    int OrthoScale = 64;
    Mat4 OrthoMapTransform;

    D3DFILLMODE FillMode;

    GOC * CurrentCamera;

    Vec3 CamPosition;     // the camera position  (THIS VARIABLE WILL BE THE CAMERA'S POSITION VARIABLE IN THE TRANSFORM COMPONENT)
    Vec3 CamLookAt;       // the look-at position
    Vec3 CamUpDirection;  // the up direction

    //World Projection and view matrices  (These use the above three Camera variables to apply camera transformations and stuff)
    Mat4 ProjMatrix;
    Mat4 ViewMatrix;
    Mat4 ViewProjMatrix;

    // Direct3D Variables
    LPDIRECT3D9 pD3D;    // the pointer to our Direct3D interface (Object)
    LPDIRECT3DDEVICE9 pDevice;    // the pointer to the device class
    LPDIRECT3DVERTEXBUFFER9 v_buffer;    // the pointer to the vertex buffer 
    LPD3DXFONT pFont;

    //LPDIRECT3DINDEXBUFFER9 i_buffer;    // the pointer to the index buffer

    LPD3DXEFFECT pShader;

    // Sprite object used to sort and draw sprites using textures
    LPD3DXSPRITE pSprite;
    
    
    //Stored texture assets
    typedef std::map<std::string, TextureSource*> TextureMap;
    TextureMap	TextureSources;

    //Stored surface assets
    typedef std::map<std::string, SpriteSource*> SurfaceMap;
    SurfaceMap	SpriteSources;



    /// *LINKED LISTS OF DRAW OBJECTS*
    // Linked List of created DefaultText objects
    ObjectLinkList<DefaultText> DirectTextVec;
    // Linked List of created Sprite objects
    ObjectLinkList<HUDSprite> HUDSpriteList;
    // Linked List of created Sprite objects
    ObjectLinkList<Sprite> SpriteList;
    ObjectLinkList<AnimatableSprite> AnimatableSpriteList;
    // Linked List of created BackBufferSurfaceSprite objects
    ///BackBufferSprites are Sprites that are rendered to the background, so there is always a background
    /// other than the back buffer background clear color
    ObjectLinkList<BackBufferSurfaceSprite> BackBufferSurfaceSpriteList;

    // Linked List of created DirectX Light objects
    ObjectLinkList<DirectLight> DirectLightList;

    ObjectLinkList<Emitter> ParticleSystemList;



    // The size of the game window screen
    Vec2 ScreenSize;

	};


  //A global pointer to the Graphics system, used to access it anywhere.
  extern Graphics* GRAPHICS;
}