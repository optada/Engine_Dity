// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "OPTada_Instance.h"



// process messages of the main window
static LRESULT CALLBACK WindowProc(
	HWND hwnd,
	UINT msg,
	WPARAM wparam,
	LPARAM lparam)
{
PAINTSTRUCT	ps;  // using in WM_PAINT
HDC			hdc; // getting device context

// get massage
switch (msg)
{
case WM_SYSKEYDOWN: {

	if (GetKeyState(VK_MENU) & 0x8000) { // ALT +

		if (GetKeyState(VK_RETURN) & 0x8000) { // ALT + ENTER
			OPTada_Instance::Do_Reaction_AltEnter();
			return 1;
		}

		if (GetKeyState(VK_TAB) & 0x8000) { // ALT + TAB
			OPTada_Instance::Do_Reaction_AltTab();
			return 1;
		}

		if (GetKeyState(VK_F4) & 0x8000) { // ALT + F4
			PostMessage(hwnd, WM_DESTROY, 0, 0);
			return 1;
		}		
	}
}

//case WM_KEYDOWN: {
//	switch (wparam)
//	{
//
//	case 0x46: { // pressed 'F' button
//
//		int i = 0;
//		// window debug place here
//
//	} break;
//
//	default: break;
//
//	}
//} break;
//
//case WM_KILLFOCUS: {
//	OPTada_Instance::Do_Reaction_LooseFocus();
//}
//
//case  WM_SETFOCUS: {
//	OPTada_Instance::Do_Reaction_TakeFocus();
//}

case WM_ACTIVATE: {

	// if fullscreen and focus occurs
	if (IsIconic(hwnd) && WA_ACTIVE == LOWORD(wparam)) {

		OPTada_Instance::Do_Reaction_TakeFocus();
	}
	else {
		// if fullscreen and loss of focus occurs
		if (WA_INACTIVE == LOWORD(wparam)) {
			OPTada_Instance::Do_Reaction_LooseFocus();
		}
	}

} break;

case WM_CREATE: {
	// all necessary initialization when creating a window occurs here
	return(0);
} break;

case WM_PAINT: {
	// start drawing
	hdc = BeginPaint(hwnd, &ps);
	// end of drawing
	EndPaint(hwnd, &ps);
	return(0);
} break;

case WM_DESTROY: {
	// delete window	
	PostQuitMessage(0);
	return(0);
} break;

default:break;

} // end switch

// window completionand windows control return
return (DefWindowProc(hwnd, msg, wparam, lparam));
};


// --------------------------------------------------------------------------------------------------------------------------------------


int WINAPI WinMain(
	HINSTANCE hinstance,     // application instance descriptor - acts as an identifier for a window procedure
	HINSTANCE hprevinstance, // in WIN32 is not used
	LPSTR lpcmdline,         // needed to run the window in command line mode
	int ncmdshow             // window display mode (needed for further WindowShow () functions)
	)
{	

	// init project and setup progect
	if (
		OPTada_Instance::Global_InitProject(hinstance, WindowProc) &&
		OPTada_Instance::Global_SetupProject()
		) 
	{ // initialization and setup OK

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

				OPTada_Instance::Tick(timeDelta);

				lastTime = currTime;
			}
		}

	} // end game cicle, or init error

	// free all modules and memory
	OPTada_Instance::Global_ShutdownProject();

	// Return window close command to window event handler
	return(global_Window.main_window_msg.wParam);
}