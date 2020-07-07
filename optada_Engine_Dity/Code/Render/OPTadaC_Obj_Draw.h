// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once  

#include "ResourceManager\OPTadaC_ResourceManager.h"


// struct for draw pipeline
class OPTadaC_Obj_Draw
{
public:

	OPTadaE_VertexShaderList_ForResourceManager VS;              // vertex shader
	OPTadaE_PixelShaderList_ForResourceManager  PS;              // pixel shader
	OPTadaS_WorldNavigationData*                world = nullptr; // world matrix for object (set->constant buffer update)
	OPTadaE_MeshList_ForResourceManager         mesh;            // mesh

	std::vector<OPTadaE_TextureList_ForResourceManager> texture_mass; // texture pack

};
