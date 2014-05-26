// AMBER.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "headers.h"
#include <stdio.h>
#include <ctime>

// Globals
OpenGLContext gl; // GL Class
int runLevel = 1; 
MSG msg;

char Keys[256];  // Key monitor
MouseInfo Mouse; // Mouse monitor
AppGlobalVars AppGlobals; // Screen Res, etc

HINSTANCE gInst;

bool RegisterWin();
bool StartGL(int ScrX, int ScrY, int BPP);
bool RenderProc(LPVOID lpParam);

bool createWindow(LPCSTR title, int width, int height);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	gInst = hInstance;

	createWindow("OpenGL3.3 Core", 800, 600); // Create our OpenGL window  
  
	gl.setupScene(); // Setup our OpenGL scene  

	long curFrame, lastFrame;

	lastFrame = curFrame = clock();


	/** 
	This is our main loop, it continues for as long as running is true 
	*/  
	while (runLevel)  
	{  
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{ // If we have a message to process, process it  
			if (msg.message == WM_QUIT) {  
				runLevel = false; // Set running to false if we have a message to quit  
			}  
			else {  
				TranslateMessage(&msg);  
				DispatchMessage(&msg);  
			}  
		}  
		else
		{ // If we don't have a message to process  
			lastFrame = curFrame;
			curFrame = clock();
			gl.renderScene(Keys, curFrame - lastFrame); // Render our scene (which also handles swapping of buffers)  
		}  

		if(Keys[VK_ESCAPE])
		{
			PostQuitMessage(0);
			OutputDebugString("Esc pressed!\n");
		}

	}  
	
	OutputDebugString("Quitting!\n");
	exit(0);
}


// Message Handler for our Window
LRESULT CALLBACK GLWinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg) 
	{
		case WM_SIZE:
			gl.reshapeWindow(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_CLOSE:
			OutputDebugString("Close Message!\n");
			runLevel=0;
			break;

		case WM_DESTROY:
			OutputDebugString("Destroy Message!\n");
			runLevel=0;
			PostQuitMessage(0);
			break;

		// Grab inputs
		case WM_KEYDOWN:
			char msg_str[256];
			sprintf(msg_str, "Key %d pressed\n", wParam);
			OutputDebugString(msg_str);
			Keys[wParam] = TRUE;
			return 0;

		case WM_KEYUP:	
			Keys[wParam] = FALSE;	
			return 0;

		case WM_MOUSEMOVE:
			Mouse.Mx=LOWORD(lParam);
			Mouse.My=HIWORD(lParam);
			return 0;

		case WM_LBUTTONDOWN:
			Mouse.Mleft=TRUE;
			return 0;
 
		case WM_LBUTTONUP:
			Mouse.Mleft=FALSE;
			return 0;
    
		case WM_RBUTTONDOWN:
			Mouse.Mright=TRUE;
			return 0;

		case WM_RBUTTONUP:
			Mouse.Mright=FALSE;
			return 0;

		default:
			return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;
}

bool createWindow(LPCSTR title, int width, int height) {  

	WNDCLASSEX glWin;

	HINSTANCE hi = GetModuleHandle(NULL);

	glWin.cbSize=sizeof(WNDCLASSEX); 
	glWin.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;		// Window has it's own context
	glWin.lpfnWndProc	= (WNDPROC) GLWinProc;				         
	glWin.cbClsExtra	= 0;
	glWin.cbWndExtra	= 0;
	glWin.hInstance		= gInst;
	glWin.hIcon			= LoadIcon(NULL, IDI_APPLICATION);			      // Default icon
	glWin.hCursor		= LoadCursor(NULL, IDI_APPLICATION);		     // Default pointer
 	glWin.hbrBackground	= NULL;
 	glWin.lpszMenuName	= NULL;
 	glWin.lpszClassName	= "OpenGLBaseWin";
	glWin.hIconSm		= NULL;

	if(!RegisterClassEx(&glWin)) {
		return false;				
	}

	HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, "OpenGLBaseWin", title, WS_OVERLAPPEDWINDOW | WS_VISIBLE,  
	CW_USEDEFAULT, 0, width, height, NULL, NULL, gInst, NULL);  

	gl.create30Context(hWnd); // Create our OpenGL context on the given window we just created  
  
	ShowWindow(hWnd, SW_SHOW);  
	UpdateWindow(hWnd);

	return true;
}  



