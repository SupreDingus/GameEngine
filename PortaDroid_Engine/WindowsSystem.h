///////////////////////////////////////////////////////////////////////////////
//This is the class that creates the window for the game.
//This is the SECOND thing WinMain must create when the engine first runs.
//(Second to the Engine class)
//WindowsSystem is the system that DETECTS AND RECEIVES MESSAGES from the operating
//  system, including key presses, mouse movement, mouse clicks, quit messages, etc.
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Containers.h"
#include "SystemsBase.h"
#include "Message.h"    //(Found in BaseSystems.h)


namespace OBALFramework
{
  // The Types of Message Boxes to Request
  enum MessageBoxType
  {
    Message_Ok = MB_OK,
    Message_OkCancel = MB_CANCELTRYCONTINUE,
    Message_RetryCancel = MB_RETRYCANCEL,
    Message_YesNo = MB_YESNO,
    Message_YesNoCancel = MB_YESNOCANCEL,
    Message_CancelTryContinue = MB_CANCELTRYCONTINUE,
  };
  // The icon to display on requested Message boxes
  enum MessageBoxIcons
  {
    Icon_Information = MB_ICONINFORMATION,
    Icon_Exclamation = MB_ICONEXCLAMATION,
    Icon_Error = MB_ICONASTERISK
  };


	class WindowsSystem : public ISystem
	{
	public:
		WindowsSystem(const WCHAR* windowTitle, int ScreenWidth, int ScreenHeight);
		~WindowsSystem();

    std::string GetName() { return "WINDOWSSYSTEM"; }

    void Initialize();

    void ActivateWindow();

    void Update(float dt);


    // Message Box Requests from other systems
    int Request_OKMessageBox(const char * messagetext, const char * messagecaption)
    { return MessageBoxA(hWnd, messagetext, messagecaption, Icon_Information | Message_Ok); }
    int Request_ERRORMessageBox(const char * messagetext, const char * messagecaption)
    { return MessageBoxA(hWnd, messagetext, messagecaption, Icon_Error | Message_Ok); }
    int RequestYESNOMessageBox(const char * messagetext, const char * messagecaption)
    { return MessageBoxA(hWnd, messagetext, messagecaption, Icon_Information | Message_YesNoCancel); }

    HWND RequestEditControl(const char * messagetext, const char * messagecaption, int posx, int posy, int sizex, int sizey)
    { 
      
      //HWND hEdit = 
      /*return CreateWindowExA(WS_EX_CLIENTEDGE, messagetext, messagecaption, 
                                    WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT, 87, 81, 150, 17, hWnd, 
                                    (HMENU)5, hInstance, NULL);*/
                                    
    }


		HWND hWnd;				// The handle for the window to be created
		HINSTANCE hInstance;	//The handle to the instance
	};


	//Global pointer to this WindowsSystem
	extern WindowsSystem* WINDOWSSYSTEM;
}