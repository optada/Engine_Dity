// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include <d3dx11.h> // for shader class


// Safely release a COM object.
template<typename T>
inline void SafeRelease(T& ptr)
{
	if (ptr != nullptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}


// enum - contains type of shaderes for shader list
enum OPTadaE_ShaderList_ForResoursManager
{
	NONE = 0,

	ENUM_ShaderList_PS_SimpleMaterial_01 = 1, // pixel shader SimpleMaterial_01
	ENUM_ShaderList_VS_SimpleMaterial_01 = 2, // vertex shader SimpleMaterial_01

	ENUM_ShaderList_ForResoursManager_MaxCount,
};


// structure - cell of shader
struct OPTadaS_ShaderStructure
{
	OPTadaE_ShaderList_ForResoursManager shaderEnum = NONE; // shader type (Enum)

	ID3D11VertexShader* linkOn_VertexShader = nullptr; // VS
	ID3D11PixelShader*  linkOn_PixelShader  = nullptr; // PS
};


// class for managering all resources (like a textures, models, shaders)
class OPTadaC_ResourceManager
{
private:

	OPTadaS_ShaderStructure shaderMass[ENUM_ShaderList_ForResoursManager_MaxCount]; // shader collection

	// Mash

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

};

