// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include <d3dx11.h> // for shader class


// enum - contains type of shaderes for shader list
enum OPTadaE_ShaderList_ForResoursManager
{
	ENUM_ShaderList_NONE = 0,

	ENUM_ShaderList_PS_SimpleMaterial_01 = 1, // pixel shader SimpleMaterial_01
	ENUM_ShaderList_VS_SimpleMaterial_01 = 2, // vertex shader SimpleMaterial_01

	ENUM_ShaderList_ForResoursManager_MaxCount,
};


// structure - cell of shader
struct OPTadaS_ShaderStructure
{
	OPTadaE_ShaderList_ForResoursManager shaderEnum = ENUM_ShaderList_NONE; // shader type (Enum)

	ID3D11InputLayout* inputLayout_VertexBuffer = nullptr; // input vertex structure for VS

	ID3D11VertexShader* linkOn_VertexShader = nullptr; // VS
	ID3D11PixelShader*  linkOn_PixelShader  = nullptr; // PS
};


// - - - - - - - - Set other shader structures or shader settings here - - - - - - - - -


// Create the input layout for the vertex shader.
D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor,Color),    D3D11_INPUT_PER_VERTEX_DATA, 0 }
};