// ---------------------------------------------------------------------------
// Project Name		:	Simple Demo
// File Name		:	Main.cpp
// Author			:	Antoine Abi Chacra
// Creation Date	:	2012/01/11
// Purpose			:	main entry point2 for the test programpotato
// History			:
// - 2012/01/11		:	- initial implementation

// - 2/12/16        :  Matt - Added sample audio playback.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"

#include "Graphics.h"
#include "LevelManager.h"

#include "Audio.h"
// ---------------------------------------------------------------------------

// Libraries
#pragma comment (lib, "Alpha_Engine.lib")

// ---------------------------------------------------------------------------
// globals

int gGameRunning = 1;

LRESULT CALLBACK MyWinCallBack(HWND hWin, UINT msg, WPARAM wp, LPARAM lp) ;


// ---------------------------------------------------------------------------
// Static function protoypes

// ---------------------------------------------------------------------------
// main


int WINAPI WinMain(HINSTANCE instanceH, HINSTANCE prevInstanceH, LPSTR command_line, int show)
{

  // Initialize the system 
  AESysInitInfo sysInitInfo;
  sysInitInfo.mCreateWindow = 1;
  sysInitInfo.mAppInstance = instanceH;
  sysInitInfo.mShow = show;
  sysInitInfo.mWinWidth = 800;
  sysInitInfo.mWinHeight = 600;
  sysInitInfo.mCreateConsole = 0;
  sysInitInfo.mMaxFrameRate = 60;
  sysInitInfo.mpWinCallBack = NULL;//MyWinCallBack;
  sysInitInfo.mClassStyle = CS_HREDRAW | CS_VREDRAW;
  sysInitInfo.mWindowStyle = WS_OVERLAPPEDWINDOW;//WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;;		
  sysInitInfo.mWindowHandle = NULL;
  sysInitInfo.mHandleWindowMessages = 1;
  AESysInit(&sysInitInfo);

  AllocConsole();
  freopen("CONOUT$", "w", stdout);
  

	if(0 == AESysInit (&sysInitInfo))
		printf("System Init Failed!\n");

  /*----------------------------------------------------------------------------
  AUDIO TEST  PART 1/3  INITIALIZING
  -----------------------------------------------------------------------------*/
    TestAudioINIT();

  LevelLoad(level_splashScreen);
  //GInitialize();

	// Changing the window title
	AESysSetWindowTitle("My New Title!");

	// reset the system modules
	AESysReset();

	// Game Loop
	while(gGameRunning)
	{
    
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();

		///////////////////
		// Game loop update
    LevelRun();

		// Game loop update
		///////////////////

        /*----------------------------------------------------------------------------
        AUDIO TEST  PART 2/3  UPDATE AUDIO
        -----------------------------------------------------------------------------*/
    Audio_UpdatePlayback();

		//////////////////
		// Game loop draw
    GRender();
		//printf("MinX: %f | MaxX: %f | MinY: %f | MaxY: %f\n", AEGfxGetWinMinX(), AEGfxGetWinMaxX(), AEGfxGetWinMinY(), AEGfxGetWinMaxY());


		// Game loop draw
		//////////////////

		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (AEInputCheckTriggered(VK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}

  //LevelUnload();

  /*----------------------------------------------------------------------------
  AUDIO TEST  PART 3/3  FREE AUDIO SYSTEM
  -----------------------------------------------------------------------------*/
    Audio_FreeSystem();

	// free the system
	AESysExit();
}

// ---------------------------------------------------------------------------

LRESULT CALLBACK MyWinCallBack(HWND hWin, UINT msg, WPARAM wp, LPARAM lp) 
{
	HDC dc;   
	PAINTSTRUCT ps;

	switch (msg)
	{
	// when the window is created
	case WM_CREATE:
		printf("My own code in window create message!\n");
		break;

	// when the rectangle is drawn
	case WM_PAINT:
		dc = BeginPaint(hWin, &ps);

		// Cleans up the painting process
		EndPaint(hWin, &ps);
		break;

	// When it's time for the window to go away
	case WM_DESTROY:
		//PostQuitMessage(0);
		//gAESysWinExists = false;
		break;

	// called any time the window is moved
	case WM_MOVE:
		// Invalidate the rect to force a redraw
		InvalidateRect(hWin, NULL, FALSE);
		break;

	case WM_ACTIVATE:
		// DO NOT REMOVE THIS
		// *(AESysGetAppActive()) = (LOWORD(wp) == WA_INACTIVE) ? 0 : 1;
		break;

	default:
		return DefWindowProc(hWin, msg, wp, lp);
	}

	return 0;
}


// ---------------------------------------------------------------------------
// Static functions implementation

// ---------------------------------------------------------------------------
