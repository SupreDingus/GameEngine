///////////////////////////////////////////////////////////////////////////////////////
//
//	WinMain
//	The main entry point for the game--everything starts here.
//	
//	Authors:  Jullian Hyatt
//	Copyright 2016, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////

//#include <stdio.h>    // include the standard input/output header file
//#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>     // Direct3D 9 Header File


// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")    // include the Direct3D 9 library file


//***** GLOBAL DECLARATIONS *****//
//Window Initialization Variables
const char windowTitle[] = "Engine";  // the title the window will have at the top
                                      // define the screen resolution
#define SCREEN_WIDTH 800             // window Width
#define SCREEN_HEIGHT 600        // window Height

                                      // Custom Flexible Vertex Format
#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

                                      // Direct3D Variables
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;    // the pointer to the vertex buffer


                                            //***** FUNCTION PROTOTYPES *****//
void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame(void);    // renders a single frame
void cleanD3D(void);    // closes Direct3D and releases memory
void init_graphics(void);    // 3D declarations


                             // Custom Vertex Struct
struct CUSTOMVERTEX { FLOAT X, Y, Z, RHW; DWORD COLOR; };


// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



//The entry point for the application--called automatically when the game starts.
//The first parameter is a handle to the application instance.
//The second parameter is the previous app instance which you can use to prevent being launched multiple times
//The third parameter is the command line string.
//The last parameter is the manner in which the application's window is to be displayed (not needed).
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  //***** REGISTER THE WINDOW *****//

  // The handle for the window to be created, filled by a function
  HWND hWnd;
  // This struct holds information for the window's window class
  WNDCLASSEX winclass;
  // clear out the window class for use (Initializes a block of memory to NULL)
  ZeroMemory(&winclass, sizeof(WNDCLASSEX));

  // fill in (Initialize) the struct with the needed information
  winclass.cbSize = sizeof(WNDCLASSEX);
  winclass.style = CS_HREDRAW | CS_VREDRAW;
  winclass.lpfnWndProc = WindowProc;
  winclass.hInstance = hInstance;
  winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
  //winclass.hbrBackground = (HBRUSH)COLOR_WINDOW;    //Comment this out so screen doesn't show until it fully enters fullscreen
  winclass.lpszClassName = L"WindowClass1";

  // register the window class
  RegisterClassEx(&winclass);



  //***** CREATE THE WINDOW *****//

  // create the window and use the result as the window handle "hWnd"
  hWnd = CreateWindowEx(NULL,
    L"WindowClass1",                  // NAME of the window class
    L"Our First Windowed Program",    // TITLE of the window
    WS_EX_TOPMOST | WS_POPUP,    // fullscreen values                   //use "WS_OVERLAPPEDWINDOW," for window STYLE
    0, 0,    // X-position and Y-position of the window
    SCREEN_WIDTH, SCREEN_HEIGHT,    // WIDTH and HEIGHT of the window
    NULL,         // we have no parent window, NULL
    NULL,         // we aren't using menus, NULL
    hInstance,    // application HANDLE
    NULL);        // used with multiple windows, NULL



                  //***** SHOW THE WINDOW *****//

                  // display the window on the screen
  ShowWindow(hWnd, nCmdShow);

  // set up and initialize Direct3D
  initD3D(hWnd);



  //***** MAIN LOOP & MESSAGING*****//

  // Enter the main loop:

  MSG msg;    // this struct holds Windows event messages

              // enter the infinite message loop (will change later)
  while (TRUE)
  {
    // check to see if any messages are waiting in the queue
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      // translate the message and dispatch it to WindowProc()
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    // if the message is WM_QUIT, exit the while loop
    if (msg.message == WM_QUIT)
      break;

    render_frame();

    // Run game code here
    // ...
    // ...
  }

  // clean up DirectX and COM
  cleanD3D();

  // return this part of the WM_QUIT message to Windows
  return msg.wParam;

} // END WinMain()




  // This is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  // Sort through and find what code to run for the message given
  switch (message)
  {
    // This message is read when the window is closed
  case WM_DESTROY:
  {
    // Close the application entirely
    PostQuitMessage(0);
    return 0;
  } break;
  }

  // Handle any messages the switch statement didn't (In this case, anything that isn't 0 or WM_Quit)
  return DefWindowProc(hWnd, message, wParam, lParam);

} // END WindowProc()



  // this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
  d3d = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface

  D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

  ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
  d3dpp.Windowed = FALSE;    // program fullscreen, not windowed  -- Leaving this TRUE with fullscreen settings will only make a boarderless window with the given width and height
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
  d3dpp.hDeviceWindow = hWnd;    // set the window to be used by Direct3D
                                 //...for fullscreen follows
  d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
  d3dpp.BackBufferWidth = SCREEN_WIDTH;      // set the width of the buffer
  d3dpp.BackBufferHeight = SCREEN_HEIGHT;    // set the height of the buffer

                                             // create a device class using this information and information from the d3dpp stuct
  d3d->CreateDevice(D3DADAPTER_DEFAULT,
    D3DDEVTYPE_HAL,
    hWnd,
    D3DCREATE_SOFTWARE_VERTEXPROCESSING,
    &d3dpp,
    &d3ddev);

  init_graphics();    // call the function to initialize the triangle

} // END initD3D()


int bluex = 0;
int bluey = 40;
int bluez = 100;
int blackx = 0;
int blacky = 0;
int blackz = 0;
int framecount = 20;
bool convert = true;
int conversionx = bluex / framecount;
int conversiony = bluey / framecount;
int conversionz = bluez / framecount;
bool xdone = false;
bool ydone = false;
bool zdone = false;
int colorx = bluex;
int colory = bluey;
int colorz = bluez;
// this is the function used to render a single frame
void render_frame(void)
{
  // clear the window to a deep blue
  d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(colorx, colory, colorz), 1.0f, 0);

  d3ddev->BeginScene();    // *BEGIN the 3D scene


                           // select which vertex format we are using
  d3ddev->SetFVF(CUSTOMFVF);

  // select the vertex buffer to display
  d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));

  // copy the vertex buffer to the back buffer
  d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);


  d3ddev->EndScene();    // *END the 3D scene

  d3ddev->Present(NULL, NULL, NULL, NULL);    // displays the created frame
} // END render_frame()



  // this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
  v_buffer->Release();    // close and release the vertex buffer
  d3ddev->Release();    // close and release the 3D device
  d3d->Release();    // close and release Direct3D
} // END cleanD3D()


float firstvertchange = 400.0f;
// this is the function that puts the 3D models into video RAM
void init_graphics(void)
{
  // create the vertices using the CUSTOMVERTEX struct
  CUSTOMVERTEX vertices[] =
  {
    { 400.0f, 62.5f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 255), },
    { 650.0f, 500.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 255, 0), },
    { 150.0f, 500.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255, 0, 0), },
  };

  // create a vertex buffer interface called v_buffer
  d3ddev->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
    0,
    CUSTOMFVF,
    D3DPOOL_MANAGED,
    &v_buffer,
    NULL);

  VOID* pVoid;    // a void pointer

                  // lock v_buffer and load the vertices into it
  v_buffer->Lock(0, 0, (void**)&pVoid, 0);
  memcpy(pVoid, vertices, sizeof(vertices));

  //Then, unlock v_buffer
  v_buffer->Unlock();
}