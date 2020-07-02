// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include <d3dx11.h> // for shader class
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <xnamath.h>


// enum - contains types for vertex shader list
enum OPTadaE_VertexShaderList_ForResoursManager
{
	ENUM_VertexShaderList_NONE = 0,

	ENUM_VertexShaderList_SimpleMaterial_01 = 1, // vertex shader SimpleMaterial_01

	ENUM_VertexShaderList_ForResoursManager_MaxCount,
};


// structure - Vertex shader cell
struct OPTadaS_VertexShaderStructure
{
	OPTadaE_VertexShaderList_ForResoursManager shaderEnum = ENUM_VertexShaderList_NONE; // vertex shader type (Enum)

	bool isInGPUMemory = false; // true - we have this in GPU memory, and can draw | false - only CPU memory 

	ID3D11InputLayout*  inputLayout  = nullptr; // input vertex structure
	ID3D11VertexShader* vertexShader = nullptr; // VS

	// function will free all GPU resources and set all other on default
	void Free_GPU()
	{
		SafeRelease(inputLayout);
		SafeRelease(vertexShader);
		isInGPUMemory = false;
	}
};


// - - - - - - - - Set other mash and shader structures or shader settings here - - - - - - - - -

// Vertex data for a default model
struct Vertex_F3Coord_F3Normal_F2TextCoord
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 textureCoord;
};

// Vertex data for a colored cube.
//struct VertexPosColor_SimpleShader
//{
//	XMFLOAT3 Position;
//	XMFLOAT3 Color;
//};

// Create the input layout for the vertex shader.
//D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc_VertexPosColor[] =
//{
//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor_SimpleShader,Position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	{ "COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor_SimpleShader,Color),    D3D11_INPUT_PER_VERTEX_DATA, 0 }
//};
