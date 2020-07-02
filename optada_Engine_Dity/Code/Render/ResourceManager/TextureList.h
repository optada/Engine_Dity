// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once


// enum - contains Texture names
enum OPTadaE_TextureList_ForResoursManager
{
	ENUM_TextureList_NONE = 0,

	//ENUM_TextureList_DefaultColoredBox_V_VertexPosColor_I_DXGI_FORMAT_R16_UINT = 1, // vertex structure VertexPosColor, index structure (WORD)

	ENUM_TextureList_ForResoursManager_MaxCount,
};


// structure - cell of texture
struct OPTadaS_TextureStructure
{
	OPTadaE_TextureList_ForResoursManager textureEnum = ENUM_TextureList_NONE; // texture type (Enum)

	bool isInGPUMemory = false; // true - we have this in GPU memory, and can draw | false - only CPU memory 

	ID3D11ShaderResourceView* texture             = nullptr;
	ID3D11SamplerState*       textureSamplerState = nullptr;

	// function will free all GPU resources and set all other on default
	void Free_GPU()
	{
		SafeRelease(texture);
		SafeRelease(textureSamplerState);
		isInGPUMemory = false;
	}
};

