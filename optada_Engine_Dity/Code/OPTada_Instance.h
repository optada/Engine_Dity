// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "Window\OPTada_Window.h" 
#include "Render\OPTada_Render.h"


// use this instance for controll all project logic
// create and destroy all global modules here
class OPTada_Instance
{
public:

	// Do all initialization here (init all global modules)
	// [in] HINSTANCE hinstance_ // hinstance_ of process
	// [in] WNDPROC& windowProc_ // event structure
	// return = true - done | false - error
	static bool Global_InitProject(HINSTANCE hinstance_, WNDPROC windowProc_) {

		// create a standard window
		if (!global_Window.InitAndCreateStandartWindow(hinstance_, windowProc_)) {
			MessageBox(NULL, L"main hange error", L"wind", NULL);
			return false;
		}

		// chage window size and mode (define)
		OPTadaS_Window_Size newWindowSize;

		// create 
		HWND hwnd = global_Window.Get_MainWindowHandle(); // get handle
		global_Window.Get_WorkplaceSize(newWindowSize); // get workspace size
		if (!global_Render.Initialization(hwnd, 1, newWindowSize.width, newWindowSize.height, true, true)) {
			MessageBox(NULL, L"Can't init render", L"Instance", NULL);
			return false;
		}

		return true;
	}


	// Do start setup here (Calling after Global_InitProject)
	// return = true - done | false - error
	static bool Global_SetupProject() {

		// chage window size and mode (define)
		OPTadaS_Window_Size newWindowSize;
		newWindowSize.width = 1280;
		newWindowSize.height = 720;
		if (!Do_Change_WindowSettings(OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_Windowed, newWindowSize)) {
			MessageBox(NULL, L"Can't change window size or state", L"Instance", NULL);
			return false;
		}


		return true;
	}

	// shutdown project project (free all global modules)
	static void Global_ShutdownProject() {
		
		// close DirectX
		global_Render.ShuttingDown();

	}


	// -----------------------------------------------------------------------------------------------------------------------------


	// Global tick
	static int Tick(float deltaTime_) {

		DrawScene(deltaTime_);

		return 0;
	}

	// All draw logic here
	static int DrawScene(float deltaTime_) {

		return global_Render.testedDraw();

		return 0;
	}


	// -----------------------------------------------------------------------------------------------------------------------------


	// call this when you need to change window size and state
	static bool Do_Change_WindowSettings(OPTadaE_WindowState_ForClassWindow newWindowState_, OPTadaS_Window_Size& newWindowSize_) {

		if (!global_Window.Change_DisplayOfWindow(newWindowState_, newWindowSize_)) {
			return false;
		};

		return true;
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


