// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "OPTada_Window.h"


// use this instance for controll all project logic
// create and destroy all global modules here
class OPTada_Instance
{
public:

	// Do all initialization here (init all global modules)
	// [in] HINSTANCE hinstance_  // hinstance_ of process
	// [in] WNDPROC& windowProc_ // event structure
	// return = true - done | false - error
	static bool Global_InitProject(HINSTANCE hinstance_, WNDPROC windowProc_) {

		// create a standard window
		if (!global_Window.InitAndCreateStandartWindow(hinstance_, windowProc_)) {
			return (global_Window.main_window_msg.wParam); // if the window did not create, close
		}

		// chage window size and mode
		OPTadaS_Window_Size newWindowSize;
		newWindowSize.width = 1280;
		newWindowSize.height = 720;
		if (!global_Window.Change_DisplayOfWindow(OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_Windowed, newWindowSize)) {
			MessageBox(NULL, L"main hange error", L"wind", NULL);
			return false;
		};

		return true;
	}

	// shutdown project project (free all global modules)
	static void Global_ShutdownProject() {

	}


	// Global tick
	static int Tick(float deltaTime_) {
		return 0;
	}


	// call this when you window is loosing focus
	static void Do_Reaction_LooseFocus() {
		global_Window.Do_LooseFocusInFullscreenMode();
	}

	// call this when your window is taking focus
	static void Do_Reaction_TakeFocus() {
		global_Window.Do_RestoreFocusInFullscreenMode();
	}


	// call this for (ALT+TAB) reaction (hide window in fullscreen)
	static void Do_Reaction_AltTab() {
		global_Window.Do_AltTabLooseFocusInFullscreenMode();
	}

	// call this for (ALT+ENTER) reaction (Swap window mode fullscreen|Windowed)
	static void Do_Reaction_AltEnter() {
		global_Window.Do_SwapMode_Fullscreen_LastWindowed();
	}

};


