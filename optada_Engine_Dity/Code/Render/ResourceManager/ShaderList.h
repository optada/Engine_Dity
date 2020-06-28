// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include <d3dx11.h> // for shader class


// structure - cell of shader
struct OPTadaS_ShaderStructure
{
	OPTadaE_ShaderList_ForResoursManager shaderEnum = ENUM_ShaderList_NONE; // shader type (Enum)

	ID3D11InputLayout* inputLayout_VertexBuffer = nullptr; // for VS

	ID3D11VertexShader* linkOn_VertexShader = nullptr; // VS
	ID3D11PixelShader*  linkOn_PixelShader  = nullptr; // PS
};


// enum - contains type of shaderes for shader list
enum OPTadaE_ShaderList_ForResoursManager
{
	ENUM_ShaderList_NONE = 0,

	ENUM_ShaderList_PS_SimpleMaterial_01 = 1, // pixel shader SimpleMaterial_01
	ENUM_ShaderList_VS_SimpleMaterial_01 = 2, // vertex shader SimpleMaterial_01

	ENUM_ShaderList_ForResoursManager_MaxCount,
};


// - - - - - - - - Set other shader structures or shader settings here - - - - - - - - -