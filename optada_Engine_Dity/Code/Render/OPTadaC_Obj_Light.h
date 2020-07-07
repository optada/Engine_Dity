// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "OPTadaS_WorldNavigationData.h"


struct OPTadaS_Light
{
    OPTadaS_Light()
    {
        ZeroMemory(this, sizeof(OPTadaS_Light));
    }

    XMFLOAT3 pos; 
    float    range;
    XMFLOAT3 dir;
    float    cone;
    XMFLOAT3 att;
    float    pad2;
    XMFLOAT4 ambient;
    XMFLOAT4 diffuse;
};



enum OPTadaE_Obj_Light_Type
{
    ENUM_Obj_Light_Simple     = 0, // simple
    ENUM_Obj_Light_Point      = 1, // point
    ENUM_Obj_Light_Spotlights = 2, // spotlight
};



class OPTadaC_Obj_Light
{
public:

    OPTadaE_Obj_Light_Type type = ENUM_Obj_Light_Simple; // enum OPTadaE_Obj_Light_Type

    OPTadaS_Light Light; // light parameters

    bool isOn = false;

    void Set_ON_OFF(bool isOn_) {
        isOn = isOn_;
    }
};
