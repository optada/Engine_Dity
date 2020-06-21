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
KBDLLHOOKSTRUCT* hookstruct = (KBDLLHOOKSTRUCT*)(lparam);

// get massage
switch (msg)
{

case WM_KEYDOWN: {
	switch (wparam)
	{

	case 0x46: { // pressed 'F' button

		//global_Window.Do_SwapMode_Fullscreen_LastWindowed();

		static int cocos = 1;

		OPTadaS_Window_Size newWindowSize;

		if (cocos == 1) {
			newWindowSize.width = 1200;
			newWindowSize.height = 720;
			if (!OPTada_Instance::Do_Change_WindowSettings(OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_Windowed, newWindowSize, true, 1, D3D11_FILL_SOLID)) {
				MessageBox(NULL, L"cocos 1", L"cocos test", NULL);
				return false;
			}
		}

		if (cocos == 2) {
			newWindowSize.width = 800;
			newWindowSize.height = 600;
			if (!OPTada_Instance::Do_Change_WindowSettings(OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_Windowed, newWindowSize, true, 1, D3D11_FILL_SOLID)) {
				MessageBox(NULL, L"cocos 2", L"cocos test", NULL);
				return false;
			}
		}

		if (cocos == 3) {
			newWindowSize.width = 1280;
			newWindowSize.height = 720;
			if (!OPTada_Instance::Do_Change_WindowSettings(OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_FullScreen, newWindowSize, true, 1, D3D11_FILL_SOLID)) {
				MessageBox(NULL, L"cocos 3", L"cocos test", NULL);
				return false;
			}
		}

		if (cocos == 4) {
			newWindowSize.width = 800;
			newWindowSize.height = 600;
			if (!OPTada_Instance::Do_Change_WindowSettings(OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_FullScreen, newWindowSize, true, 1, D3D11_FILL_SOLID)) {
				MessageBox(NULL, L"cocos 4", L"cocos test", NULL);
				return false;
			}
		}

		if (cocos == 5) {
			newWindowSize.width = 800;
			newWindowSize.height = 600;
			if (!OPTada_Instance::Do_Change_WindowSettings(OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_Windowed, newWindowSize, true, 1, D3D11_FILL_SOLID)) {
				MessageBox(NULL, L"cocos 5", L"cocos test", NULL);
				return false;
			}
		}

		if (cocos == 6) {

		}

		cocos++;

	} break;

	default: break;

	}
} break;

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


////////////////////////// low - level keyboard processing code /////////////////////////

HHOOK ExistingKeyboardProc;
// low-level keyboard polling to control (alt + tab) and (alt + win) presses
LRESULT CALLBACK KeyboardProcLowLevel(int nCode, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT* hookstruct = (KBDLLHOOKSTRUCT*)(lParam);

	switch (wParam)
	{
	case WM_KEYDOWN:
		break;
	case WM_SYSKEYDOWN:
		// Take no Action, Signal app to take action in main loop
		if ((((hookstruct->flags) >> 5) & 1)) {
			// ALT +
			switch (hookstruct->vkCode)
			{

			case VK_TAB: { // ALT+TAB

				OPTada_Instance::Do_Reaction_AltTab();

			} break;

			case VK_RETURN: { // ALT+ENTER 

				OPTada_Instance::Do_Reaction_AltEnter();
				return 1;
			} break;

			case VK_ESCAPE: break; // ALT+ESC	

			case VK_DELETE: break; // ALT+DEL	

			};

		}
		break;

	}

	return CallNextHookEx(ExistingKeyboardProc, nCode, wParam, lParam);
}
bool HookKeyboardProc(HINSTANCE hinst)
{
	ExistingKeyboardProc = SetWindowsHookEx(
		WH_KEYBOARD_LL,			// int idHook,
		KeyboardProcLowLevel,	// HOOKPROC lpfn,
		hinst,					// HINSTANCE hMod,
		NULL);					// DWORD dwThreadId

	if (!ExistingKeyboardProc) {
		MessageBox(NULL, L"Hook failed", L"WinMain, HookKeyboardProc()", NULL);
		return false;
	}
	else {
		//Succeeded.
		return true;
	}
}

////////////////////////// low - level keyboard processing code /////////////////////////



// --------------------------------------------------------------------------------------------------------------------------------------


int WINAPI WinMain(
	HINSTANCE hinstance,     // application instance descriptor - acts as an identifier for a window procedure
	HINSTANCE hprevinstance, // in WIN32 is not used
	LPSTR lpcmdline,         // needed to run the window in command line mode
	int ncmdshow             // window display mode (needed for further WindowShow () functions)
	)
{	

	// hook low-level keyboard command(s) for process
	if (!HookKeyboardProc(hinstance)) {
		return (global_Window.main_window_msg.wParam); // hook failed - close
	}

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