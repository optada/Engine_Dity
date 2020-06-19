// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Class_Window\OPTada_Window.h"

using namespace std;


int WINAPI WinMain(
	HINSTANCE hinstance,     // application instance descriptor - acts as an identifier for a window procedure
	HINSTANCE hprevinstance, // in WIN32 is not used
	LPSTR lpcmdline,         // needed to run the window in command line mode
	int ncmdshow             // window display mode (needed for further WindowShow () functions)
	)
{	

	// create a standard window
	if (!global_Window.InitAndCreateStandartWindow(hinstance)) { 
		return (global_Window.main_window_msg.wParam); // if the window did not create, close
	}

	OPTadaS_Window_Size newWindowSize;
	newWindowSize.width = 1280;
	newWindowSize.height = 720;
	if (!global_Window.Change_DisplayOfWindow(OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_Windowed, newWindowSize)) {
		MessageBox(NULL, L"main hange error", L"wind", NULL);
	};


	float lastTime = (float)timeGetTime();

	// entering the program cycle
	while (true)
	{
		if (PeekMessage(&global_Window.main_window_msg, NULL, 0, 0, PM_REMOVE)) {
			if (global_Window.main_window_msg.message == WM_QUIT) { // check for exit command	
				break;
			}
			TranslateMessage(&global_Window.main_window_msg); // This function translates virtual-key format messages into character messages.			
			DispatchMessage(&global_Window.main_window_msg); // check, send WindowProc () commands to process window commands
		} 
		else {
			float currTime = (float)timeGetTime();
			float timeDelta = (currTime - lastTime) * 0.001f;

			// Tick(timeDelta); 

			lastTime = currTime;
		}
	}

	// Return window close command to window event handler
	return(global_Window.main_window_msg.wParam);
}