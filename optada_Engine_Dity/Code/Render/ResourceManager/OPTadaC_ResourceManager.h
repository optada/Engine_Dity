// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include <string>

#include <d3dcompiler.h>

#include "OPTadaC_ResourceManager_Settings.h"
#include "PixelShaderList.h"
#include "VertexShaderList.h"
#include "MeshList.h"
#include "ConstantBufferList.h"


// class for managering all resources (like a textures, models, shaders)
class OPTadaC_ResourceManager
{
private:

	OPTadaS_PixelShaderStructure  PS_Mass[OPTadaE_PixelShaderList_ForResoursManager::ENUM_PixelShaderList_ForResoursManager_MaxCount];   // PS collection
	OPTadaS_VertexShaderStructure VS_Mass[OPTadaE_VertexShaderList_ForResoursManager::ENUM_VertexShaderList_ForResoursManager_MaxCount]; // VS collection

	OPTadaS_MeshStructure meshMass[OPTadaE_MeshName_ForResoursManager::ENUM_MeshName_ForResoursManager_MaxCount]; // Mesh collection

	ID3D11Buffer* constantBuffersMass[OPTadaE_ConstantBuffersList_ForResoursManager::OPTadaE_ConstantBuffersList_ForResoursManager_MaxCount]; // buffer of constant for VS
	
    // Texture

public:

	// Free all COM objects (use it for safety shutdowning)
	void FreeAll();


	// ------------------------ Pixel Shader ----------------------------

	// Create Pixel shader from a binary file
	// [in] OPTadaE_PixelShaderList_ForResoursManager shaderEnum_ // shader enum
	// [in] const std::wstring& fileName_                         // filename
	// [in] ID3D11Device* gDevice_                                // graphic device
	// return = true - done | false - error
	bool Create_PixelShader_FromBinaryFile(OPTadaE_PixelShaderList_ForResoursManager shaderEnum_, const std::wstring& fileName_, ID3D11Device* gDevice_);

	// will return pixel shader cell
	// [in] OPTadaE_PixelShaderList_ForResoursManager shaderEnum_ // shader enum
	// return = OPTadaS_PixelShaderStructure* - done | NULL - error pixel shader is not created
	inline OPTadaS_PixelShaderStructure* Get_PixelShader_Cell(OPTadaE_PixelShaderList_ForResoursManager shaderEnum_);

	// setup pixel shader for render pipeline
	// [in] OPTadaE_PixelShaderList_ForResoursManager shaderEnum_ // shader enum
	// return = true - done | false - error
	inline bool Use_PixelShader(OPTadaE_PixelShaderList_ForResoursManager shaderEnum_);

	// free pixel shader from GPU memory
	// [in] OPTadaE_PixelShaderList_ForResoursManager shaderEnum_ // shader enum
	// return = true - done | false - error
	bool Delete_PixelShader(OPTadaE_PixelShaderList_ForResoursManager shaderEnum_);

	// ------------------------ Vertex Shader ----------------------------

	// Create Vertex shader from a binary file
	// [in] OPTadaE_VertexShaderList_ForResoursManager shaderEnum_ // shader enum
	// [in] const std::wstring& fileName_                          // filename
	// [in] ID3D11Device* gDevice_                                 // graphic device
	// [in] D3D11_INPUT_ELEMENT_DESC* vertexLayoutDesc_            // link on DirectX structure
	// [in] UINT countOfvertexLayoutDesc_,                         // count of vertexLayoutDesc use _countof(vertexLayoutDesc_)
	// return = true - done | false - error
	bool Create_VertexShader_FromBinaryFile(OPTadaE_VertexShaderList_ForResoursManager shaderEnum_, const std::wstring& fileName_, ID3D11Device* gDevice_, 
		D3D11_INPUT_ELEMENT_DESC* vertexLayoutDesc_, UINT countOfvertexLayoutDesc_);

	// will return Vertex shader cell
	// [in] OPTadaE_VertexShaderList_ForResoursManager shaderEnum_ // shader enum
	// return = OPTadaS_VertexShaderStructure* - done | NULL - error vertex shader is not created
	inline OPTadaS_VertexShaderStructure* Get_VertexShader_Cell(OPTadaE_VertexShaderList_ForResoursManager shaderEnum_);

	// setup Vertex shader for render pipeline
	// [in] OPTadaE_VertexShaderList_ForResoursManager shaderEnum_ // shader enum
	// return = true - done | false - error
	inline bool Use_VertexShader(OPTadaE_VertexShaderList_ForResoursManager shaderEnum_);

	// free Vertex shader from GPU memory
	// [in] OPTadaE_VertexShaderList_ForResoursManager shaderEnum_ // shader enum
	// return = true - done | false - error
	bool Delete_VertexShader(OPTadaE_VertexShaderList_ForResoursManager shaderEnum_);

	// ------------------------- Mesh -----------------------------


	// Add links on vertex and index  buffers, create mesh cell, add other parameters
	// [in] OPTadaE_MeshName_ForResoursManager meshName_ // mesh enum
	// [in] ID3D11Buffer* vertexBuffer_                  // link on vertor buffer (in CPU memory)
	// [in] ID3D11Buffer* indexBuffer_                   // link on index buffer (in CPU memory) | can be NULL
	// [in] UINT vertexStride_                           // size of vector structure (per one dot)
	// [in] UINT vertexOffset_                           //
	// [in] DXGI_FORMAT indexBufferFormat_               // DXGI_FORMAT::DXGI_FORMAT_UNKNOWN --> if indexBuffer == NULL
	// return = true - done | false - error
	bool Add_Mesh(OPTadaE_MeshName_ForResoursManager meshName_, ID3D11Buffer* vertexBuffer_, ID3D11Buffer* indexBuffer_, UINT vertexStride_, UINT vertexOffset_, DXGI_FORMAT indexBufferFormat_);

	// !- this method will update the cell to completely rewrite it again.METHOD DOES NOT REMOVE MEMORY      -!
	// !- To clear memory correctly, call the Unload_FromGPU_Mesh method,                                    -!
	// !- then request a cell using the Get_MeshCell method, and manually free the buffer_Mem memory buffers -!
	// use this for update cell to default parameters
	// [in] OPTadaE_MeshName_ForResoursManager meshName_ // mesh enum
	void SetToDefault_MeshCell(OPTadaE_MeshName_ForResoursManager meshName_);

	// load resourses to GPU memory (vertex buffer, and index buffer if not NULL)
	// [in] OPTadaE_MeshName_ForResoursManager meshName_ // mesh enum
	// [in] ID3D11Device* device_d3d11_                  // link on DirectX Device
	// return = true - done | false - error
	bool Load_ToGPU_Mesh(OPTadaE_MeshName_ForResoursManager meshName_, ID3D11Device* device_d3d11_);

	// free resources from GPU memory
	// [in] OPTadaE_MeshName_ForResoursManager meshName_ // mesh enum
	void Unload_FromGPU_Mesh(OPTadaE_MeshName_ForResoursManager meshName_);

	// will return mesh cell, if cell has been created
	// [in] OPTadaE_MeshName_ForResoursManager meshName_ // mesh enum
	// return = OPTadaS_MeshStructure* - done | NULL - error or mash is not created
	inline OPTadaS_MeshStructure* Get_MeshCell(OPTadaE_MeshName_ForResoursManager meshName_);

	// will return mesh cell, if mesh has been loaded to GPU memory
	// [in] OPTadaE_MeshName_ForResoursManager meshName_ // mesh enum
	// return = OPTadaS_MeshStructure* - done | NULL - error or mash is not created
	inline OPTadaS_MeshStructure* Get_MeshCell_IfInGPU(OPTadaE_MeshName_ForResoursManager meshName_);


	// ------------------------ Constant buffer ----------------------------


	// save link on constant buffer for save free
	// [in] OPTadaE_ConstantBuffersList_ForResoursManager constantBufferID_ // constant buffer enum
	// [in] ID3D11Buffer* constantBuffer_                                   // link on constant buffer
	inline void Set_ConstantBuffer(OPTadaE_ConstantBuffersList_ForResoursManager constantBufferID_, ID3D11Buffer* constantBuffer_);

	// will return link on constant buffer
	// [in] OPTadaE_ConstantBuffersList_ForResoursManager constantBufferID_ // constant buffer enum
	// return = ID3D11Buffer* - done | nullptr - error
	inline ID3D11Buffer* Get_ConstantBuffer(OPTadaE_ConstantBuffersList_ForResoursManager constantBufferID_);


	// ------------------------ Texture ----------------------------

	
};

