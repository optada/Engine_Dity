// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "OPTada_Window.h"


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

	case WM_KEYDOWN: {
		switch (wparam)
		{

		case 0x46: { // pressed 'F' button

			OPTadaE_WindowState_ForClassWindow windowProc_WindowState;
			OPTadaS_Window_Size WindowProc_workPlaceSize;

			global_Window.Get_WindowState(windowProc_WindowState);
			global_Window.Get_WorkplaceSize(WindowProc_workPlaceSize);

			if (windowProc_WindowState == OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_FullScreen) {

				if (!global_Window.Change_DisplayOfWindow(OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_Windowed, WindowProc_workPlaceSize)) {
					MessageBox(NULL, L"Swich mode to window error", L"wind", NULL);
				};
			}
			else {

				if (!global_Window.Change_DisplayOfWindow(OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_FullScreen, WindowProc_workPlaceSize)) {
					MessageBox(NULL, L"Swich mode to fullscreen error", L"wind", NULL);
				};
			}

		} break;

		default: break;

		}
	} break;

	case WM_ACTIVATE: {

		OPTadaE_WindowState_ForClassWindow windowState;
		global_Window.Get_WindowState(windowState);

		// if fullscreen and focus occurs
		if (windowState == OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_FullScreen
			&& IsIconic(hwnd) && WA_ACTIVE == LOWORD(wparam)) {

			global_Window.Do_FocusInFullScreenMode(true);
		}
		else {
			// if fullscreen and loss of focus occurs
			if (windowState == OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_FullScreen
				&& WA_INACTIVE == LOWORD(wparam)) {

				global_Window.Do_FocusInFullScreenMode(false);
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

				OPTadaE_WindowState_ForClassWindow windowState;
				global_Window.Get_WindowState(windowState);

				// if fullscreen and focus occurs
				if (windowState == OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_FullScreen) {

					global_Window.Do_FocusInFullScreenMode(false);
				}


				//if (!Screen_Windowed)
				//{
				//	// when full-screen mode and alt + tab activation, the window is minimized, when activated, it returns
				//	if (/*!IsIconic(main_window_handle) */true) // modify alt + tab (modify launch + tab)
				//	{
				//		Main_Window.setMonitorWindowed();
				//		ShowWindow(main_window_handle, SW_SHOWMINNOACTIVE);
				//		//ShowWindow(main_window_handle, SW_SHOWMINNOACTIVE); // SW_SHOWMINNOACTIVE   SW_MINIMIZE  SW_HIDE
				//	}
				//}

			}
					   break;
			case VK_RETURN: { // ALT+ENTER


				return 1;
			}
						  break;
			case VK_ESCAPE: break; // ALT+ESC			
			case VK_DELETE: break; // ALT+DEL				
			};

		}
		break;

		//case WM_KEYUP:
		//	break;
		//case WM_SYSKEYUP:
		//	break;

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
		//  Failed
		return false;
	}
	else {
		//Succeeded.
		return true;
	}
}
int UnHookKeyboardProc()
{
	if (ExistingKeyboardProc) {
		BOOL retcode = UnhookWindowsHookEx((HHOOK)KeyboardProcLowLevel);

		if (retcode) {
			// Successfully Un Hooked keyboard routine.
		}
		else {
			// Error Keyboard Not successfully Un hooked!
			// UnhookWindowsHookEx() returned failure!
		}

		return retcode;
	}
	else {
		// Error Keyboard Not successfully hooked!
		// Could not unhook procedure!
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

	// create a standard window
	if (!global_Window.InitAndCreateStandartWindow(hinstance, WindowProc)) { 
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