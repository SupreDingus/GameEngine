///////////////////////////////////////////////////////////////////////////////////////
//
//	WinMain
//	The main entry point for the game--everything starts here.
//	
//	Authors:  Jullian Hyatt
//	Copyright 2016, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////

#include "Precompiled.h"
#include "OBALCore.h"
#include <malloc.h>
#include <crtdbg.h>
//#include "WindowsSystem.h"    //(Also includes BaseSystems.h)
//#include "Graphics.h"
//#include "BaseSystems.h"

using namespace OBALFramework;


// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")    // include the Direct3D 9 library file


#ifdef _MSC_VER
// Use Visual C++'s memory checking functionality
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _MSC_VER


//***** GLOBAL DECLARATIONS *****//
//Window Initialization Variables
const WCHAR windowTitle[] = L"Obal Engine";  // the title the window will have at the top
// define the screen resolution
#define SCREEN_WIDTH 1600             // window Width
#define SCREEN_HEIGHT 900        // window Height

#define PIXELS_PER_UNIT 64        // 1 Unit in engine = 64 standardized pixels of 3D space


//The entry point for the application--called automatically when the game starts.
//Parameter 1: Handle to the application instance.
//Parameter 2: The previous app instance (Can be used to prevent multiple launches)
//Parameter 3: The command line string.
//Parameter 4: The manner in which the application's window is to be displayed (not needed).
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  //_CrtSetBreakAlloc(185);
  //_CrtSetBreakAlloc(183);
  //_CrtSetBreakAlloc(181);
  //_CrtSetBreakAlloc(179);
  //_CrtSetBreakAlloc(177);
  //_CrtSetBreakAlloc(175);


#ifdef _MSC_VER
  //_crtBreakAlloc = 1828;
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _MSC_VER

  OutputDebugStringA("START: WinMain Entry\n");
  //1. Create the core engine which manages all the systems that make up the game
  OBALCore* OBALengine = new OBALCore(windowTitle, SCREEN_WIDTH, SCREEN_HEIGHT);    //Global Pointer: OBAL


  // Initialize all systems in the core
  OBALengine->Initialize();

  // Start the game loop.  The engine will loop until a quit message is recieved
  // When the Core recieves a quit message, the next line will be executed
  OBALengine->GameLoop();

  // Delete all systems attached to the OBALCore
  OBALengine->DestroySystems();

  // All systems in OBAL have been deleted, now delete the engine
  delete OBALengine;

  return 0;

} // END WinMain()