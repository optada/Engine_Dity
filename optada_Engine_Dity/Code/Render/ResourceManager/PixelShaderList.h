// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include <d3dx11.h> // for shader class


// enum - contains types for pixel shader list
enum OPTadaE_PixelShaderList_ForResoursManager
{
	ENUM_PixelShaderList_NONE = 0,

	ENUM_PixelShaderList_SimpleMaterial_01 = 1, // pixel shader SimpleMaterial_01

	ENUM_PixelShaderList_ForResoursManager_MaxCount,
};


// structure - Pixel shader cell
struct OPTadaS_PixelShaderStructure
{
	OPTadaE_PixelShaderList_ForResoursManager shaderEnum = ENUM_PixelShaderList_NONE; // pixel shader type (Enum)

	bool isInGPUMemory = false; // true - we have this in GPU memory, and can draw | false - only CPU memory 

	ID3D11PixelShader* pixelShader = nullptr; // PS

	// function will free all GPU resources and set all other on default
	void Free_GPU()
	{
		SafeRelease(pixelShader);
		isInGPUMemory = false;
	}
};