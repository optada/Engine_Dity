// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include <dinput.h>


class OPTada_Input
{
private:

    IDirectInputDevice8* DIKeyboard;
    IDirectInputDevice8* DIMouse;

    DIMOUSESTATE mouseLastState;
    LPDIRECTINPUT8 DirectInput;

    HWND hwnd;

public:

    // use this for init OPTada_Input
    // [in] HINSTANCE hInstance_ //
    // [in] HWND hwnd_           // main window handle
    // return = true - done | false - error
    bool Initialization(HINSTANCE hInstance_, HWND hwnd_);

    // Use this for free all resourses
    void ShuttingDown();


    // use this for detect input
    // [in] float deltaTick_ // tick
    void DetectInput(float deltaTick_);
};


// --------------------------------------------------------------------------------------------


// Created class OPTada_Input for input
extern OPTada_Input global_Input;