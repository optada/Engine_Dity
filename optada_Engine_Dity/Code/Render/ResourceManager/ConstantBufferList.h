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
