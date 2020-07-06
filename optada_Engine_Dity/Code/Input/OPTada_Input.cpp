// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //


#include "OPTada_Input.h"


bool OPTada_Input::ReadKeyboard()
{
	HRESULT result;

	// Read the keyboard device.
	result = keyboardDevice->GetDeviceState(sizeof(kb_State0), (LPVOID)&kb_State0);
	if (FAILED(result)) {
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED)) {
			keyboardDevice->Acquire();
		}
		else {
			return false;
		}
	}

	// ---- ---- ---- update all keyboard data

	// update keyboard button events
	for (int i = 0; i < 256; i++) {

		if (kb_State0[i] != 0) { // button pressed
			// if 0 -> set 2 (key down)
			if (kb_State_Out[i] == 0) {
				kb_State_Out[i] = 2;
				continue;
			}
			// if 2 -> set 3 (key keep)
			if (kb_State_Out[i] == 2) {
				kb_State_Out[i] = 3;
				continue;
			}
		}

		if (kb_State0[i] == 0) { // button not pressed
			// if 1 -> set 0 (key free)
			if (kb_State_Out[i] == 1) {
				kb_State_Out[i] = 0;
				continue;
			}
			// if 2 or 3 -> set 1 (key down)
			if (kb_State_Out[i] >= 2) {
				kb_State_Out[i] = 1;
				continue;
			}
		}
	}

	return true;
}

bool OPTada_Input::ReadMouse()
{
	HRESULT result;

	// Read the mouse device.
	result = mouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseCurrState);
	if (FAILED(result)) {
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED)) {
			mouseDevice->Acquire();
		}
		else {
			return false;
		}
	}

	// ---- ---- ---- update all mouse data

	// save mouse delta
	m_DX = m_mouseCurrState.lX;
	m_DY = m_mouseCurrState.lY;


	// if mouse locked and we have focus - update mouse coursore coordinate to workspace center
	if (mouseIsLocked && programHaveFocus) {
		POINT pt;
		pt.x = workpace_Width / 2;
		pt.y = workspace_Height / 2;
		ClientToScreen(hwnd, &pt);
		SetCursorPos(pt.x, pt.y);
	}


	// get mouse workspace coursor
	POINT mouseCoord;
	GetCursorPos(&mouseCoord);
	ScreenToClient(hwnd, &mouseCoord);
	m_X = mouseCoord.x;
	m_Y = mouseCoord.y;
	// Ensure the mouse location doesn't exceed the screen width or height.
	if (m_X < 0) {
		m_X = 0;
	}
	if (m_Y < 0) {
		m_Y = 0;
	}
	if (m_X > workpace_Width) {
		m_X = workpace_Width;
	}
	if (m_Y > workspace_Height) {
		m_Y = workspace_Height;
	}

	// update mouse button events
	for (int i = 0; i < 8; i++) {

		if (m_mouseCurrState.rgbButtons[i] != 0) { // button pressed
			// if 0 -> set 2 (key down)
			if (m_State_Out[i] == 0) { 
				m_State_Out[i] = 2;
				continue;
			}
			// if 2 -> set 3 (key keep)
			if (m_State_Out[i] == 2) {
				m_State_Out[i] = 3;
				continue;
			}
		}

		if (m_mouseCurrState.rgbButtons[i] == 0) { // button not pressed
			// if 1 -> set 0 (key free)
			if (m_State_Out[i] == 1) {
				m_State_Out[i] = 0;
				continue;
			}
			// if 2 or 3 -> set 1 (key down)
			if (m_State_Out[i] >= 2) {
				m_State_Out[i] = 1;
				continue;
			}
		}
	}

	return true;
}


bool OPTada_Input::Initialization(HINSTANCE hinstance_, HWND hwnd_, int newWorkspace_Width_, int newWorkspace_Height_, bool showCursor_, bool mouseIsLocked_)
{
	HRESULT hr;
	Set_Workspace(newWorkspace_Width_, newWorkspace_Height_);
	Set_ShowMouseCursor(showCursor_);
	Set_LockMouse(mouseIsLocked_);

	// Store the screen size which will be used for positioning the mouse cursor.
	hwnd = hwnd_;
	
	// Initialize the main direct input interface.
	hr = DirectInput8Create(hinstance_, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&i_directInput, NULL);
	if (FAILED(hr)) {
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	hr = i_directInput->CreateDevice(GUID_SysKeyboard, &keyboardDevice, NULL);
	if (FAILED(hr)) {
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	hr = keyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) {
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	hr = keyboardDevice->SetCooperativeLevel(hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr)) {
		return false;
	}

	// Now acquire the keyboard.
	hr = keyboardDevice->Acquire();
	if (FAILED(hr)) {
		return false;
	}

	// Initialize the direct input interface for the mouse.
	hr = i_directInput->CreateDevice(GUID_SysMouse, &mouseDevice, NULL);
	if (FAILED(hr)) {
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	hr = mouseDevice->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hr)) {
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	hr = mouseDevice->SetCooperativeLevel(hwnd_, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr)) {
		return false;
	}

	// Acquire the mouse.
	hr = mouseDevice->Acquire();
	if (FAILED(hr)) {
		return false;
	}

	for (int i = 0; i < 256; i++) {
		kb_State_Out[i] = 0;
	}

	for (int i = 0; i < 8; i++) {
		m_State_Out[i] = 0;
	}

	return true;
}

void OPTada_Input::ShuttingDown()
{
	// Release the mouse.
	if (mouseDevice) {
		mouseDevice->Unacquire();
		mouseDevice->Release();
		mouseDevice = 0;
	}

	// Release the keyboard.
	if (keyboardDevice) {
		keyboardDevice->Unacquire();
		keyboardDevice->Release();
		keyboardDevice = 0;
	}

	// Release the main interface to direct input.
	if (i_directInput) {
		i_directInput->Release();
		i_directInput = 0;
	}
}


bool OPTada_Input::Update()
{
	bool result;

	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	// Read the current state of the mouse.
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	return true;
}


void OPTada_Input::Get_Input_8Mouse_256Keyboard(POINT& mouseCoord_, POINT& mouseDelta_, unsigned char** mouseButtons_, unsigned char** keyboardButtons_)
{
	mouseCoord_.x = m_X;
	mouseCoord_.y = m_Y;

	mouseDelta_.x = m_DX;
	mouseDelta_.y = m_DY;

	*mouseButtons_ = m_State_Out;

	*keyboardButtons_ = kb_State_Out;
}


void OPTada_Input::Set_Workspace(int newWorkspace_Width_, int newWorkspace_Height_)
{
	workpace_Width = newWorkspace_Width_;
	workspace_Height = newWorkspace_Height_;
}

void OPTada_Input::Set_HaveFocus(bool isInFocus_)
{
	programHaveFocus = isInFocus_;
}

void OPTada_Input::Set_LockMouse(bool isLocked_)
{
	mouseIsLocked = isLocked_;
}

void OPTada_Input::Set_ShowMouseCursor(bool isShow_)
{
	ShowCursor(isShow_);
}


// --------------------------------------------------------------------------------------------


OPTada_Input global_Input;