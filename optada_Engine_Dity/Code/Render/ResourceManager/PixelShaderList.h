// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include <d3dx11.h> // for shader class


// enum - contains types for pixel shader list
enum OPTadaE_PixelShaderList_ForResourceManager
{
	ENUM_PixelShaderList_NONE = 0,

	ENUM_PixelShaderList_PS_Color = 1, // pixel shader SimpleMaterial_01

	ENUM_PixelShaderList_ForResourceManager_MaxCount,
};

//  ---- ---- ---- Sampler state list ENUM here for every pixel shader ---- ---- ----

enum OPTadaE_SamplerStateList_ForResourceManager
{
	ENUM_SamplerStateList_Linear_1  = 0, // linear sempler x1
	ENUM_SamplerStateList_Linear_16 = 1, // linear sempler x16
};

//  ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

// structure - Pixel shader cell
struct OPTadaS_PixelShaderStructure
{
	OPTadaE_PixelShaderList_ForResourceManager shaderEnum = ENUM_PixelShaderList_NONE; // pixel shader type (Enum)

	bool isInGPUMemory = false; // true - we have this in GPU memory, and can draw | false - only CPU memory 

	ID3D11PixelShader* pixelShader = nullptr; // PS

	std::vector<OPTadaE_SamplerStateList_ForResourceManager> SampleStateList; // sample state list 0 - to max

	// function will free all GPU resources and set all other on default
	void Free_GPU()
	{
		SafeRelease(pixelShader);
		SampleStateList.clear();
		isInGPUMemory = false;
	}
};


