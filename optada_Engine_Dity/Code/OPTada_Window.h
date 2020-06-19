// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "OPTada_Window_Settings.h"


// enum for class window - window states
enum OPTadaE_WindowState_ForClassWindow
{
	NONE                                   = 0, // - 
	ENUM_WindowState_Windowed              = 1, // Windowed
	ENUM_WindowState_WindowedWithNoBurders = 2, // Windowed with no borders
	ENUM_WindowState_FullScreen            = 3, // Fullscreen
};


// Structure of size (Width - X  | Height - Y)
struct OPTadaS_Window_Size
{
	int width  = 0; // X
	int height = 0; // Y
};


// The class that creates and controls the window
class OPTada_Window
{
private:

	OPTadaE_WindowState_ForClassWindow windowState = OPTadaE_WindowState_ForClassWindow::NONE; // Current state of window 

	WINDOWPLACEMENT windowPlacement_FullScreen = { 0 }; // information about the position of the window in full screen mode
	WINDOWPLACEMENT windowPlacement_Windowed   = { 0 }; // information about the position of the window in window mode

	OPTadaS_Window_Size windowSize;    // Size of window (with borders)
	OPTadaS_Window_Size workplaceSize; // Size of workplace

	HWND      main_window_handle    = nullptr; // window handle (links to access the window)
	HINSTANCE main_window_hinstance = nullptr; // window procedure indicator


	// set new parameters of window (for changing size) | widnowSize and workplaceSize
	// [in] OPTadaS_Window_Size& new_WorkplaceSize_ // new workplace size
	void Update_WindowSizeWithBorders();

public:

	MSG main_window_msg; // window message descriptor (for processing window messages)


	// method initializes the class and creates a standard window
	// [in] HINSTANCE hinstance_  // hinstance_ of process
	// [in] WNDPROC& windowProc_ // event structure
	bool InitAndCreateStandartWindow(HINSTANCE hinstance_, WNDPROC& windowProc_);

	// method changes window display settings and window size
	// [in] OPTadaE_WindowState_ForClassWindow new_WindowState_ // set new window state --> if NONE - not changing | other - new state
	// [in] OPTadaS_Window_Size& new_WorkplaceSize_             // set new workPlace size (you can use OPTada_Window->Get_WorkplaceSize() for not changing)
	bool Change_DisplayOfWindow(OPTadaE_WindowState_ForClassWindow new_WindowState_, OPTadaS_Window_Size& new_WorkplaceSize_);


	// getting the size of the current monitor
	// [out] OPTadaS_Window_Size& monitorSize_ // size of monitor
	void Get_MonitorSize(OPTadaS_Window_Size& monitorSize_);


	// get current window state
	// [out] OPTadaE_WindowState_ForClassWindow& windowState_
	inline void Get_WindowState(OPTadaE_WindowState_ForClassWindow& windowState_);

	// get current window size
	// [out] OPTadaS_Window_Size& windowSize_
	void Get_WindowSize(OPTadaS_Window_Size& windowSize_);

	// get current workplace size
	// [out] OPTadaS_Window_Size& workplaceSize_
	void Get_WorkplaceSize(OPTadaS_Window_Size& workplaceSize_);

	// get handle of main window
	// return = main_window_handle
	HWND Get_MainWindowHandle();
	

	// USE THIS if you loose focus, when your window mode is fullscreen mode
	// [in] bool haveFocus_ // true - if you take focus again | false - if you loose focus
	void Do_FocusInFullScreenMode(bool haveFocus_);
};


// --------------------------------------------------------------------------------------------


// Created class OPTada_Window for main window
extern OPTada_Window global_Window;
