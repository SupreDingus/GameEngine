#include "Precompiled.h"
#include "WindowsSystem.h"
#include "OBALCore.h"

namespace OBALFramework
{
	//Initialize global pointer to the windows system (externed in WindowsSystem.h)
	WindowsSystem* WINDOWSSYSTEM = NULL;

	//Used by windows to identify our window class type
	//(it's needed for registering/creating/unregistering the window)
	const WCHAR windowsClassName[] = L"FrameworkEngineWindowClass";


  void WindowsSystem::Update(float dt)
  {
    MSG msg;
    //Look for any pending windows messages, remove them, then handle them
    //The second parameter is the window handle--NULL just means get any message from the current thread
    //The third and forth parameters are the start and end message types to process
    //The last parameter determines whether or not the message is removed
    while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) //It is important to get all windows messages available not just one
    {
      TranslateMessage(&msg);	//Makes sure WM_CHAR and similar messages are generated
      DispatchMessage(&msg);	//Calls the message procedure (see below) with this message

                              //If we get a quit message, broadcast it to all systems
      if (msg.message == WM_QUIT)
      {
        Request_OKMessageBox("WM_QUIT message received.", "WM_QUIT Message");
        // Create a quit message and tell the CoreEngine to broadcast it
        Message_Quit q;
        OBAL->BroadcastMessage(&q);

      }
    }
  }

  /// ******LR-Key Translations***** ///
  // This function retrieves the specific left/right SHIFT, CTRL, and ALT key code that is pressed
  WPARAM MapLeftRightKeys(WPARAM lrkey, LPARAM lParam)
  {
    WPARAM new_lrkey = lrkey;
    UINT scancode = (lParam & 0x00ff0000) >> 16;
    unsigned int extendedcode = (lParam & 0x01000000) != 0;

    switch (lrkey) {
    case VK_SHIFT:
      new_lrkey = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
      break;
    case VK_CONTROL:
      new_lrkey = extendedcode ? VK_RCONTROL : VK_LCONTROL;
      break;
    case VK_MENU:
      new_lrkey = extendedcode ? VK_RMENU : VK_LMENU;
      break;
    default:
      // If we get a nonmappable key, just return what we got.
      new_lrkey = lrkey;
      break;
    }

    // Returned the remapped key code.
    return new_lrkey;
  }

  /// *****Message Handling***** ///
  // This is the main message handler for the program
  LRESULT WINAPI MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
  {
    // Sort through and find what code to run for the message given
    switch (msg)
    {
      /// *****CHARACTER KEY PRESSES***** ///
    case WM_CHAR: //A character key was pressed
      {
        //Create a key message
        Message_CharacterKey key;
        //Set the character pressed (the wParam is the ascii value)
        key.character = wParam;
        //Broadcast the message to all systems
		    OBAL->BroadcastMessage(&key);
        break;
      }

    /// *****MOUSE OPERATIONS***** ///
    /// *****Mouse Buttons***** ///
    case WM_LBUTTONDOWN:
      {
        break;
      }
    case WM_RBUTTONDOWN:
      {

        break;
      }
    case WM_LBUTTONUP:
      {

        break;
      }
    case WM_RBUTTONUP:
      {

        break;
      }
    /// *****Mouse Movement***** ///
    case WM_MOUSEMOVE:
      {

        break;
      }
    /// *****ANY KEYS***** ///
    case WM_KEYDOWN: //A key was pressed
                     //TODO: Handle any key logic you might need for game controls
                     //Use virtual key codes (VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, VK_SPACE, etc.)
                     //to detect specific keys (the wParam is the key pressed) and then
                     //broadcast whatever message you need
      {
        WPARAM recievedkey = wParam;

        // First, we need to check if key pressed has a left/right versions (like 'SHIFT', 'CONTROL', etc.)
        if (wParam == VK_SHIFT || wParam == VK_CONTROL || wParam == VK_MENU)
          recievedkey = MapLeftRightKeys(wParam,lParam);
        
        //Create a key message
        Message_Key anydown;
        //Set the character pressed (the wParam is the ascii value)
        anydown.key = recievedkey;
        //Broadcast the message to all systems
        OBAL->BroadcastMessage(&anydown);
        break;
      }
    case WM_KEYUP: //A key was released
                   //TODO: Handle any key logic you might need for game controls
      {
        WPARAM recievedupkey = wParam;

        // First, we need to check if key pressed has a left/right versions (like 'SHIFT', 'CONTROL', etc.)
        if (wParam == VK_SHIFT || wParam == VK_CONTROL || wParam == VK_MENU)
          recievedupkey = MapLeftRightKeys(wParam, lParam);

        //Create a key message
        Message_KeyUp anyup;
        //Set the character released (the wParam is the ascii value)
        anyup.keyup = recievedupkey;
        //Broadcast the message to all systems
        OBAL->BroadcastMessage(&anyup);
        break;
      }

    /// *****DESTROY MESSAGE***** ///
    case WM_DESTROY: //A destroy message--time to kill the game
                     //Make sure we shut everything down properly by telling Windows
                     //to post a WM_QUIT message (the parameter is the exit code).
      {
        PostQuitMessage(0);
        return 0;
      }

    /// *****DROP FILES***** ///
    case WM_DROPFILES:
      {

        return 0;
      }

    /// *****SYSTEM DOWN***** ///
    case WM_SYSKEYDOWN:
      {
        //Eat the WM_SYSKEYDOWN message to prevent freezing the game when
        //the alt key is pressed
        switch (wParam)
        {
        case VK_LMENU:
        case VK_RMENU:
          return 0;//Stop Alt key from stopping the winProc
        case VK_F4:
          //Check for Alt F4
          DWORD dwAltKeyMask = (1 << 29);
          if ((lParam & dwAltKeyMask) != 0)
            PostQuitMessage(0);
          return 0;
        }
        return 0;//
      }

    //TODO: Handle mouse messages and other windows messages
    // ........
    
    } /// END Switch (message)

    // Handle any messages the switch statement didn't handle
    // Thus, pass it on for Windows to handle.
    return DefWindowProc(hWnd, msg, wParam, lParam);

  } /// END WindowProc()



	WindowsSystem::WindowsSystem(const WCHAR * windowTitle, int ScreenWidth, int ScreenHeight)
	{
		//Set the global pointer to this new windows system
		WINDOWSSYSTEM = this;

		WNDCLASSEX winclass = 
		  { sizeof(WNDCLASSEX),	//The SIZE of this structure (passing the size allows Microsoft to update their interfaces and maintain backward compatibility)
        CS_CLASSDC,//CS_HREDRAW | CS_VREDRAW,		//The STYLE of the window class--this is the base type (one device context for all windows in the process)
        MessageHandler,						    //The NAME of the MESSAGE HANDLING function
			  0L, 0L,							     	//The amount of EXTRA MEMORY to allocate for this class and window
			  GetModuleHandle(NULL),				//HANDLE to the INSTANCE that has the windows procedure--NULL means use this file.
			  NULL,								//Add an ICON as a resource and add them here
			  LoadCursor(NULL, IDC_ARROW),		//Use the default arrow CURSOR
			  NULL, NULL,							//The background BRUSH and MENU--these can be NULL
			  windowsClassName, NULL  //The CLASS NAME and the small ICON (NULL just uses the default)
      };			

		// register the window class
		RegisterClassEx(&winclass);

		// Store the handle to the instance
		hInstance = winclass.hInstance;



		//***** CREATE THE WINDOW *****//

		// create the window and use the result as the window handle "hWnd"
		hWnd = CreateWindow(windowsClassName,  // NAME of the window class
			windowTitle,					               // TITLE of the window
      WS_OVERLAPPEDWINDOW,                 // windowed values         //use "WS_EX_TOPMOST | WS_POPUP," for fullscreen STYLE
      CW_USEDEFAULT, CW_USEDEFAULT,        // X-position and Y-position of the window
			ScreenWidth, ScreenHeight,    // WIDTH and HEIGHT of the window
			GetDesktopWindow(),         // we have no parent window, NULL
			NULL,         // we aren't using menus, NULL
			hInstance,    // application HANDLE
			NULL);        // used with multiple windows, NULL
      

    //Registers whether a window accepts dropped files (Set to "true" to allow drag n' drop files)
    DragAcceptFiles(hWnd, false);

	}
	WindowsSystem::~WindowsSystem()
	{
		//Unregister the window class
		UnregisterClass(windowsClassName, hInstance);
	}

  void WindowsSystem::Initialize()
  {
    OutputDebugStringA("INITIALIZING: WindowsSystem\n");
    OutputDebugStringA("REGISTERING AS Quit MESSAGE RECIPIENT: WindowsSystem\n");
    //If a Message Recipient List is being used, tell the CoreEngine to Register the windows system
    //  into the Quit message Recipient Vector This way, the windows system will ONLY get QUIT messages
    if (MSGRECIPIENTS != NULL)
    {
      MSGRECIPIENTS->AddRecipient(this, Mid::Quit);
    }
    
  }

  void WindowsSystem::ActivateWindow()
  {
    OutputDebugStringA("ACTIVATING WINDOW: WindowsSystem\n");
    //Show the window (could also be SW_SHOWMINIMIZED, SW_SHOWMAXIMIZED, etc.)
    ShowWindow(hWnd, SW_SHOWDEFAULT);
    //Send a WM_PAINT message to the window
    UpdateWindow(hWnd);
    
  } // END ActivateWindow()

 

} /// END namespace OBALFramework
