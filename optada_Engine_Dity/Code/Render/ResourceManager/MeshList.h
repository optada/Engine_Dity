// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include <d3dx11.h> // for vector and indexes
#include <xnamath.h> // for DirectX types


// structure - cell of mash
// if we have no IndexBuffer - we will draw no indexed
struct OPTadaS_MeshStructure
{
	OPTadaE_MeshName_ForResoursManager  meshType  = ENUM_MeshName_NONE;  // mesh type (Enum)
	
	bool isInGPUMemory = false; // true - we have this in GPU memory, and can draw | false - only CPU memory 
	
	UINT vertexStride = 0;
	UINT offset       = 0;

	DXGI_FORMAT indexBufferFormat = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;

	ID3D11Buffer* vertexBuffer_GPU = nullptr; // VB
	ID3D11Buffer* indexBuffer_GPU  = nullptr; // IB

	ID3D11Buffer* vertexBuffer_InMEM = nullptr; // VB
	ID3D11Buffer* indexBuffer_InMEM  = nullptr; // IB
};

// enum - contains Mesh names
enum OPTadaE_MeshName_ForResoursManager
{
	ENUM_MeshName_NONE = 0,

	// ENUM_MeshName_Simple_V_VertexPosColor_I_DXGI_FORMAT_R16_UINT
	// ----_--------_MeshNameHere_V_NameOfVertexStructureHere_I_TypeOfIndexedStructureHere

	ENUM_MeshName_Simple_V_VertexPosColor_I_DXGI_FORMAT_R16_UINT = 1, // vertex structure VertexPosColor, index structure (WORD)
	ENUM_MeshName_Simple_V_VertexPosColor_I_NoIndexed            = 2, // vertex structure VertexPosColor, with no indexed

	ENUM_MeshName_ForResoursManager_MaxCount,
};


// - - - - - - - - Set other mash structures or shader settings here - - - - - - - - -


// Vertex data for a colored cube.
struct VertexPosColor
{
	XMFLOAT3 Position;
	XMFLOAT3 Color;
};