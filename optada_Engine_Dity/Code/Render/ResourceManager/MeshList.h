// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include <DirectXMath.h>
#include <xnamath.h> // for DirectX types


// enum - contains Mesh names
enum OPTadaE_MeshList_ForResourceManager
{
	ENUM_MeshList_NONE = 0,

	ENUM_MeshList_DefaultBox = 1, // vertex structure VertexPosColor, index structure (WORD) DXGI_FORMAT_R16_UINT
	//ENUM_MeshList_ColoredBox_V_VertexPosColor_I_DXGI_FORMAT_R16_UINT = 2, // vertex structure VertexPosColor, index structure (WORD)

	ENUM_MeshList_ForResourceManager_MaxCount,
};


// structure - cell of mash
// if we have no IndexBuffer - we will draw no indexed
struct OPTadaS_MeshStructure
{
	OPTadaE_MeshList_ForResourceManager meshName = ENUM_MeshList_NONE; // mesh name (Enum)
	
	bool isInGPUMemory = false; // true - we have this in GPU memory, and can draw | false - only CPU memory 
	
	UINT vertexStride = 0; // size of vector structure (per one dot)
	UINT vertexOffset = 0;

	DXGI_FORMAT indexBufferFormat = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	UINT indexBufferCount = 0;

	ID3D11Buffer* vertexBuffer_GPU = nullptr; // VB GPU
	ID3D11Buffer* indexBuffer_GPU  = nullptr; // IB GPU

	void* vertexBuffer_InMEM  = nullptr;   // VB CPU
	UINT ByteWidth_VertexBuffer_InMEM = 0; // byte with in CPU memory
	void* indexBuffer_InMEM   = nullptr;   // IB CPU
	UINT ByteWidth_IndexBuffer_InMEM  = 0; // byte with inCPU memory

	// function will free all GPU resources and set all other on default
	void Free_GPU()
	{
		SafeRelease(vertexBuffer_GPU);
		SafeRelease(indexBuffer_GPU);
		isInGPUMemory = false;
	}
};

