// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once


// Shader constant buffers
enum OPTadaE_ConstantBufferList_ForResourceManager
{
	ENUM_ConstantBufferList_Application = 0, // update this CB, once
	ENUM_ConstantBufferList_Frame       = 1, // update this CB, every frame
	ENUM_ConstantBufferList_ObjectData  = 2, // update this CB, for every object (position, object parameters, light, other)

	ENUM_ConstantBufferList_ForResourceManager_CountItems, // count of constan buffers
};


//struct OPTadaS_CB_ObjectData
//{
//    OPTadaS_CB_ObjectData()
//    {
//        ZeroMemory(this, sizeof(OPTadaS_CB_ObjectData));
//    }
//
//    XMFLOAT4 
//
//    XMFLOAT3 dir;
//    float pad;
//    XMFLOAT4 ambient;
//    XMFLOAT4 diffuse;
//};
//
//OPTadaS_CB_ObjectData CB_ObjectData;
//
//CB_ObjectData.dir     = XMFLOAT3(0.25f, 0.5f, -1.0f);
//CB_ObjectData.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
//CB_ObjectData.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);