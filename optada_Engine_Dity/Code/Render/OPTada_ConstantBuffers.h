// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "OPTadaC_Obj_Camera.h"
#include "OPTadaC_Obj_Draw.h"
#include "OPTadaC_Obj_Light.h"



// constant buffer per global
struct OPTadaS_CB_GlobalData
{
    OPTadaS_CB_GlobalData()
    {
        ZeroMemory(this, sizeof(OPTadaS_CB_GlobalData));
    }

    XMFLOAT4 NONE;
};

static OPTadaS_CB_GlobalData cb_GlobalData;


// -----------------------------------------------------------------------


// constant buffer per frame
struct OPTadaS_CB_FrameData
{
    OPTadaS_CB_FrameData()
    {
        ZeroMemory(this, sizeof(OPTadaS_CB_FrameData));
    }

    OPTadaS_Light light_Mass[100]; // 100 light mass (0 -  global)

    XMFLOAT4 light_param; // X - count global (simple) | Y - count point | Z - count spotlight
};

static OPTadaS_CB_FrameData cb_FrameData;


// -----------------------------------------------------------------------


// constant buffer per object
struct OPTadaS_CB_ObjectData
{
    OPTadaS_CB_ObjectData()
    {
        ZeroMemory(this, sizeof(OPTadaS_CB_ObjectData));
    }
    
    XMMATRIX WVP;
    XMMATRIX World;
};

static OPTadaS_CB_ObjectData cb_ObjectData;


// -----------------------------------------------------------------------