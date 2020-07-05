// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#include "OPTada_Input.h"


bool OPTada_Input::Initialization(HINSTANCE hInstance_, HWND hwnd_)
{
    HRESULT hr;

    hr = DirectInput8Create(hInstance_,
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        (void**)&DirectInput,
        NULL);
    if (FAILED(hr)) {
        return false; 
    }

    hr = DirectInput->CreateDevice(GUID_SysKeyboard,
        &DIKeyboard,
        NULL);
    if (FAILED(hr)) {
        return false; 
    }

    hr = DirectInput->CreateDevice(GUID_SysMouse,
        &DIMouse,
        NULL);
    if (FAILED(hr)) {
        return false; 
    }

    hr = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
    hr = DIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

    hr = DIMouse->SetDataFormat(&c_dfDIMouse);
    hr = DIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);


    hwnd = hwnd_;

    return true;
}

void OPTada_Input::ShuttingDown()
{
	DIKeyboard->Unacquire();
	DIMouse->Unacquire();
	DirectInput->Release();
}


void OPTada_Input::DetectInput(float deltaTick_)
{
    DIMOUSESTATE mouseCurrState;

    BYTE keyboardState[256];

    DIKeyboard->Acquire();
    DIMouse->Acquire();

    DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState); // get mouse

    DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState); // get keyboard

    // keyboard
    if (keyboardState[DIK_ESCAPE] & 0x80) {
        PostMessage(hwnd, WM_DESTROY, 0, 0);
    }
    if (keyboardState[DIK_LEFT] & 0x80) {

    }
    if (keyboardState[DIK_RIGHT] & 0x80) {

    }
    if (keyboardState[DIK_UP] & 0x80) {

    }
    if (keyboardState[DIK_DOWN] & 0x80) {

    }

    // Mouse
    if (mouseCurrState.lX != mouseLastState.lX) {
        
    }
    if (mouseCurrState.lY != mouseLastState.lY) {

    }

    // joystic

    mouseLastState = mouseCurrState;

    return;
}


// --------------------------------------------------------------------------------------------


OPTada_Input global_Input;