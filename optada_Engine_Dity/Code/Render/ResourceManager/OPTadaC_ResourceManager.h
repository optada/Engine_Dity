// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "OPTadaC_ResourceManager_Settings.h"

#include "ShaderList.h"
#include "MeshList.h"


// class for managering all resources (like a textures, models, shaders)
class OPTadaC_ResourceManager
{
private:

	OPTadaS_ShaderStructure shaderMass[ENUM_ShaderList_ForResoursManager_MaxCount]; // shader collection

	OPTadaS_MeshStructure meshMass[OPTadaE_MeshName_ForResoursManager::ENUM_MeshName_ForResoursManager_MaxCount]; // Mesh collection

	// Texture

public:

	// add shader to shader list
	// [in] OPTadaE_ShaderList_ForResoursManager shaderEnum_ // shader enum
	// [in] ShaderClass* newShader_                          // link on shader  
	// return = true - done | false - error
	template< class ShaderClass >
	bool Add_Shader(OPTadaE_ShaderList_ForResoursManager shaderEnum_, ShaderClass* newShader_);

	// free shader from GPU memory
	// [in] OPTadaE_ShaderList_ForResoursManager shaderEnum_ // shader enum
	// return = true - done | false - error
	bool Delete_Shader(OPTadaE_ShaderList_ForResoursManager shaderEnum_);

	// return shader link (ID3D11VertexShader | ID3D11PixelShader)
	// [in] OPTadaE_ShaderList_ForResoursManager shaderEnum_ // shader enum
	// return = shader link - done | NULL - error
	template< class ShaderClass >
	ShaderClass* Get_Shader(OPTadaE_ShaderList_ForResoursManager shaderEnum_);


	// return = shader link - done | NULL - error
	bool Add_Mesh(OPTadaE_MeshName_ForResoursManager mashName_, ID3D11Buffer* vertexBuffer_, ID3D11Buffer* indexBuffer_, UINT vertexCellLength_, UINT vertexOffset_, );

	// return = shader link - done | NULL - error
	bool Delete_Mesh(OPTadaE_MeshName_ForResoursManager meshName_);

	// return = shader link - done | NULL - error
	bool Load_ToGPU_Mesh(OPTadaE_MeshName_ForResoursManager meshName_);

	// return = shader link - done | NULL - error
	bool Unload_FromGPU_Mesh(OPTadaE_MeshName_ForResoursManager meshName_);

	// return = OPTadaS_MeshStructure* - done | NULL - error or mash is not created
	OPTadaS_MeshStructure* Get_MeshCell(OPTadaE_MeshName_ForResoursManager meshName_);
};

