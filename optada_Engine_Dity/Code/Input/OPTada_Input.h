// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include <dinput.h>


// mocroses for cheking key actyvity
// a == 0 kb_Mass[DIK_ ]
#define _B_NOTPRESSED(a) (a == 0)
// a >= 2 kb_Mass[DIK_ ]
#define _B_PRESSED(a)    (a >= 2)
// a == 2 kb_Mass[DIK_ ]
#define _B_DOWN(a)       (a == 2)
// a == 1 kb_Mass[DIK_ ]
#define _B_UP(a)         (a == 1)


// input class
class OPTada_Input
{
private:

	HWND hwnd;

	IDirectInput8* i_directInput;
	IDirectInputDevice8* keyboardDevice;
	IDirectInputDevice8* mouseDevice;

	// kb
	unsigned char kb_State0[256];

	unsigned char kb_State_Out[256];

	// m
	DIMOUSESTATE2 m_mouseCurrState; // now
	bool mouseIsLocked = false;

	int m_X = 0, m_Y = 0;   // workspace coordinate
	int m_DX = 0, m_DY = 0; // delta move
	unsigned char m_State_Out[8]; // mouse buttons

	// other
	int workpace_Width = 0, workspace_Height = 0;
	bool programHaveFocus = true;


	// update keyboard data
	bool ReadKeyboard();
	
	// update mouse data
	bool ReadMouse();


public:

	// use this for init OPTada_Input
	// [in] HINSTANCE hInstance_     // hinstance of process
	// [in] HWND hwnd_               // main window handle
	// [in] int newWorkspace_Width_  // new ws width
	// [in] int newWorkspace_Height_ // new ws height
	// [in] bool showCursor_         // true - show | false - hide
	// [in] bool mouseIsLocked_      // locked - true | unlocked - false
    // return = true - done | false - error
	bool Initialization(HINSTANCE hinstance_, HWND hwnd_, int newWorkspace_Width_, int newWorkspace_Height_, bool showCursor_, bool mouseIsLocked_);

	// Use this for free all resourses
	void ShuttingDown();


	// use this for detect and update input
	// return = true - done | false - error
	bool Update();


	// call this function for take all new input (8 mouse, 256 keyboard)
	// [out] POINT& mouseCoord_               // will return mouse workspace coord
	// [out] POINT& mouseDelta_               // will return mouse delta moove
	// [out] unsigned char** mouseButtons_    // m_Buttons_Out (use )
	// [out] unsigned char** keyboardButtons_ // kb_Buttons_Out
	void Get_Input_8Mouse_256Keyboard(POINT& mouseCoord_, POINT& mouseDelta_, unsigned char** mouseButtons_, unsigned char** keyboardButtons_);


	// set new workspace size
	// [in] int newWorkspace_Width_  // new ws width
	// [in] int newWorkspace_Height_ // new ws height
	void Set_Workspace(int newWorkspace_Width_, int newWorkspace_Height_);

	// set program focus state for mouse lock
	// [in] bool isInFocus_ // have focus - true | lost focus - false
	void Set_HaveFocus(bool isInFocus_);

	// set lock on mouse (by workspace center)
	// [in] bool isLocked_ // locked - true | unlocked - false
	void Set_LockMouse(bool isLocked_);

	// show | hide cursor
	// [in] isShow_ // true - show | false - hide
	void Set_ShowMouseCursor(bool isShow_);
};


// --------------------------------------------------------------------------------------------


// Created class OPTada_Input for input
extern OPTada_Input global_Input;