// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <strstream>

#include <dxgi.h> 
#include <d3dcompiler.h>

#include "MamoryManager_x64\OPTada_MemoryManager.h"
#include "OBJ_Loader.h"
#include "WICTextureLoader11.h"
#include "OPTadaC_ResourceManager_Settings.h"

#include "PixelShaderList.h"
#include "VertexShaderList.h"
#include "MeshList.h"
#include "ConstantBufferList.h"
#include "TextureList.h"



// class for managering all resources (like a textures, models, shaders)
class OPTadaC_ResourceManager
{
private:

	OPTada_MemoryManager memManager; // memory manager
	OPTadaS_Key_MemoryManager* memKey1 = nullptr; // key memory manager


	OPTadaS_PixelShaderStructure  PS_Mass[OPTadaE_PixelShaderList_ForResourceManager::ENUM_PixelShaderList_ForResourceManager_MaxCount];   // PS collection
	OPTadaS_VertexShaderStructure VS_Mass[OPTadaE_VertexShaderList_ForResourceManager::ENUM_VertexShaderList_ForResourceManager_MaxCount]; // VS collection

	OPTadaS_MeshStructure    mesh_Mass[OPTadaE_MeshList_ForResourceManager::ENUM_MeshList_ForResourceManager_MaxCount];          // Mesh collection	
	OPTadaS_TextureStructure texture_Mass[OPTadaE_TextureList_ForResourceManager::ENUM_TextureList_ForResourceManager_MaxCount]; // Texture collection

	ID3D11Buffer* constantBuffers_Mass[ENUM_ConstantBufferList_ForResourceManager_CountItems]; // buffer of constant for VS
	std::vector<ID3D11SamplerState*> samplerState_Mass;                                        // buffer of sampler states for PS

public:

	// use it for init resource manager (create all samplers and constant buffers here)
	// [in] ID3D11Device* gDevice_                    // graphic device
	// [in] std::vector<UINT> constantBufferSizeList_ // mass of size constant buffers
	// return = true - done | false - error
	bool Init_ResourceManager(ID3D11Device* gDevice_, std::vector<UINT> constantBufferSizeList_);

	// Free all COM objects (use it for safety shutdowning)
	void FreeAll();


	// ------------------------ Pixel Shader ----------------------------

	// Create Pixel shader from a binary file
	// [in] OPTadaE_PixelShaderList_ForResourceManager shaderEnum_                    // shader enum
	// [in] const std::wstring& fileName_                                             // filename
	// [in] ID3D11Device* gDevice_                                                    // graphic device
	// [in] std::vector<OPTadaE_SamplerStateList_ForResourceManager> SampleStateList_ // PS sample state list (per 0 to N) [0] == (s0) (watch OPTadaE_SamplerStateList_ForResourceManager)
	// return = true - done | false - error
	bool Create_PixelShader_FromBinaryFile(OPTadaE_PixelShaderList_ForResourceManager shaderEnum_, const std::wstring& fileName_, ID3D11Device* gDevice_, std::vector<OPTadaE_SamplerStateList_ForResourceManager> SampleStateList_);

	// will return pixel shader cell
	// [in] OPTadaE_PixelShaderList_ForResourceManager shaderEnum_ // shader enum
	// return = PS_Mass[item]
	OPTadaS_PixelShaderStructure* Get_PixelShader_Cell(OPTadaE_PixelShaderList_ForResourceManager shaderEnum_);

	// setup pixel shader for render pipeline
	// [in] OPTadaE_PixelShaderList_ForResourceManager shaderEnum_ // shader enum
	// [in] ID3D11DeviceContext* gDeviceContext_                   // device context
	// return = true - done | false - error
	bool Use_PixelShader(OPTadaE_PixelShaderList_ForResourceManager shaderEnum_, ID3D11DeviceContext* gDeviceContext_);

	// free pixel shader from GPU memory and delete
	// [in] OPTadaE_PixelShaderList_ForResourceManager shaderEnum_ // shader enum
	// return = true - done | false - error
	bool Delete_PixelShader(OPTadaE_PixelShaderList_ForResourceManager shaderEnum_);


	// ------------------------ Vertex Shader ----------------------------

	// Create Vertex shader from a binary file
	// [in] OPTadaE_VertexShaderList_ForResourceManager shaderEnum_ // shader enum
	// [in] const std::wstring& fileName_                           // filename
	// [in] ID3D11Device* gDevice_                                  // graphic device
	// [in] D3D11_INPUT_ELEMENT_DESC* vertexLayoutDesc_             // link on DirectX structure
	// [in] UINT countOfvertexLayoutDesc_,                          // count of vertexLayoutDesc use _countof(vertexLayoutDesc_)
	// return = true - done | false - error
	bool Create_VertexShader_FromBinaryFile(OPTadaE_VertexShaderList_ForResourceManager shaderEnum_, const std::wstring& fileName_, ID3D11Device* gDevice_,
		D3D11_INPUT_ELEMENT_DESC* vertexLayoutDesc_, UINT countOfvertexLayoutDesc_);

	// will return Vertex shader cell
	// [in] OPTadaE_VertexShaderList_ForResourceManager shaderEnum_ // shader enum
	// return = VS_Mass[item]
	OPTadaS_VertexShaderStructure* Get_VertexShader_Cell(OPTadaE_VertexShaderList_ForResourceManager shaderEnum_);

	// setup Vertex shader for render pipeline
	// [in] OPTadaE_VertexShaderList_ForResourceManager shaderEnum_ // shader enum
	// [in] ID3D11DeviceContext* gDeviceContext_                    // device context
	// return = true - done | false - error
	bool Use_VertexShader(OPTadaE_VertexShaderList_ForResourceManager shaderEnum_, ID3D11DeviceContext* gDeviceContext_);

	// free Vertex shader from GPU memory and delete
	// [in] OPTadaE_VertexShaderList_ForResourceManager shaderEnum_ // shader enum
	// return = true - done | false - error
	bool Delete_VertexShader(OPTadaE_VertexShaderList_ForResourceManager shaderEnum_);


	// ------------------------- Mesh -----------------------------

	// Add links on vertex and index  buffers, create mesh cell, add other parameters
	// [in] OPTadaE_MeshList_ForResourceManager meshName_ // mesh enum
	// [in] const std::string& fileName_                  // filename 
	// [in] ID3D11Device* gDevice_                        // graphic device
	// [in] UINT vertexStride_                            // size of vector structure (per one dot)
	// [in] UINT vertexOffset_                            //
	// [in] DXGI_FORMAT indexBufferFormat_                // DXGI_FORMAT::DXGI_FORMAT_UNKNOWN --> if indexBuffer == NULL
	// return = true - done | false - error
	bool Create_Mesh_FromFileToMem(OPTadaE_MeshList_ForResourceManager meshName_, const std::string fileName_, ID3D11Device* gDevice_, UINT vertexStride_, UINT vertexOffset_, DXGI_FORMAT indexBufferFormat_);

	// !- this method will update the cell to completely rewrite it again.METHOD DOES NOT REMOVE MEMORY      -!
	// !- To clear memory correctly, call the Unload_FromGPU_Mesh method,                                    -!
	// !- then request a cell using the Get_MeshCell method, and manually free the buffer_Mem memory buffers -!
	// use this for update cell to default parameters
	// [in] OPTadaE_MeshList_ForResourceManager meshName_ // mesh enum
	void SetToDefault_MeshCell(OPTadaE_MeshList_ForResourceManager meshName_);

	// load resourses to GPU memory (vertex buffer, and index buffer if not NULL)
	// [in] OPTadaE_MeshList_ForResourceManager meshName_ // mesh enum
	// [in] ID3D11Device* device_d3d11_                   // link on DirectX Device
	// return = true - done | false - error
	bool Load_ToGPU_Mesh(OPTadaE_MeshList_ForResourceManager meshName_, ID3D11Device* device_d3d11_);

	// free resources from GPU memory
	// [in] OPTadaE_MeshList_ForResourceManager meshName_ // mesh enum
	void Unload_FromGPU_Mesh(OPTadaE_MeshList_ForResourceManager meshName_);

	// setup mesh for render pipeline
	// [in] OPTadaE_MeshList_ForResourceManager meshName_ // mesh enum
	// [in] ID3D11DeviceContext* gDeviceContext_          // device context
	// return = true - done | false - error
	bool Use_Mesh_WithIndexBuffer(OPTadaE_MeshList_ForResourceManager meshName_, ID3D11DeviceContext* gDeviceContext_);

	// will return mesh cell, if cell has been created
	// [in] OPTadaE_MeshList_ForResourceManager meshName_ // mesh enum
	// return = OPTadaS_MeshStructure* - done | NULL - error or mash is not created
	OPTadaS_MeshStructure* Get_MeshCell(OPTadaE_MeshList_ForResourceManager meshName_);

	// will return mesh cell, if mesh has been loaded to GPU memory
	// [in] OPTadaE_MeshList_ForResourceManager meshName_ // mesh enum
	// return = OPTadaS_MeshStructure* - done | NULL - error or mash is not created
	OPTadaS_MeshStructure* Get_MeshCell_IfInGPU(OPTadaE_MeshList_ForResourceManager meshName_);


	// ------------------------ Texture ----------------------------

	// loading texture from file to GPU
	// [in] OPTadaE_TextureList_ForResourceManager constantBufferID_ // texture enum
	// [in] const std::wstring& fileName_                            // filename
	// [in] ID3D11Device* gDevice_                                   // graphic device
	// return = true - done | false - error
	bool Create_Texture_LoadFromFile(OPTadaE_TextureList_ForResourceManager textureEnum_, const std::wstring& fileName_, ID3D11Device* gDevice_);

	// get texture cell
	// [in] OPTadaE_TextureList_ForResourceManager constantBufferID_ // texture enum
	// return = texture_Mass[item]
	OPTadaS_TextureStructure* Get_Texture_Cell(OPTadaE_TextureList_ForResourceManager textureEnum_);

	// get texture cell if it is in GPU memory
	// [in] OPTadaE_TextureList_ForResourceManager constantBufferID_ // texture enum
	// return = texture_Mass[item]
	OPTadaS_TextureStructure* Get_Texture_Cell_IfInGPU(OPTadaE_TextureList_ForResourceManager textureEnum_);

	// use texture for directX
	// [in] OPTadaE_TextureList_ForResourceManager constantBufferID_ // texture enum
	// [in] ID3D11DeviceContext* gDeviceContext_                     // device context
	// [in] UINT resourceSlot_                                       // resource slot for texture
	// return = true - done | false - error
	bool Use_Texture(OPTadaE_TextureList_ForResourceManager textureEnum_, ID3D11DeviceContext* gDeviceContext_, UINT resourceSlot_);

	// delete texture and other Texture's interfaces
	// [in] OPTadaE_TextureList_ForResourceManager constantBufferID_ // texture enum
	// return = true - done | false - error
	bool Delete_Texture(OPTadaE_TextureList_ForResourceManager textureEnum_);


	// ------------------------ Constant buffer ----------------------------

	// update constant buffer
	// [in] OPTadaE_ConstantBufferList_ForResourceManager constantBufferEnum_ // Constant buffer ENUM
	// [in] void* linkOnData_                                                 // data
	// [in] ID3D11DeviceContext* gDeviceContext_                              // device context
	void UpdateSubresource(OPTadaE_ConstantBufferList_ForResourceManager constantBufferEnum_, void* linkOnData_, ID3D11DeviceContext* gDeviceContext_);

};


// Simple mesh loader

//bool load_SimpleMesh_FromOBJFile_Vertex_CoordTextCoordNormal_Indexes_UINT(const std::string fileName_, std::vector<Vertex_F3Coord_F3Normal_F2TextCoord>& outputVertexMass_, 
//	std::vector<UINT>& outputIndexMass_, bool reverseNormal_, bool reverseVTexture_);