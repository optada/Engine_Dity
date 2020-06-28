// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#include "OPTadaC_ResourceManager.h"


template<>
bool OPTadaC_ResourceManager::Add_Shader(OPTadaE_ShaderList_ForResoursManager shaderEnum_, ID3D11VertexShader* newShader_)
{
    OPTadaS_ShaderStructure& cell = shaderMass[shaderEnum_];
    if (cell.shaderEnum == OPTadaE_ShaderList_ForResoursManager::ENUM_ShaderList_NONE && newShader_) {
        cell.shaderEnum = shaderEnum_;
        cell.linkOn_VertexShader = newShader_;
        
        return true; 
    }

    return false;
}

template<>
bool OPTadaC_ResourceManager::Add_Shader(OPTadaE_ShaderList_ForResoursManager shaderEnum_, ID3D11PixelShader* newShader_)
{
    OPTadaS_ShaderStructure& cell = shaderMass[shaderEnum_];
    if (cell.shaderEnum == OPTadaE_ShaderList_ForResoursManager::ENUM_ShaderList_NONE && newShader_) {
        cell.shaderEnum = shaderEnum_;
        cell.linkOn_PixelShader = newShader_;
        
        return true; 
    }

    return false;
}

bool OPTadaC_ResourceManager::Delete_Shader(OPTadaE_ShaderList_ForResoursManager shaderEnum_)
{
    OPTadaS_ShaderStructure& cell = shaderMass[shaderEnum_];
    if (cell.shaderEnum != OPTadaE_ShaderList_ForResoursManager::ENUM_ShaderList_NONE) {
        SafeRelease(cell.linkOn_PixelShader);
        SafeRelease(cell.linkOn_VertexShader);
        cell.shaderEnum = ENUM_ShaderList_NONE;

        return true;
    }

    return false;
}

template<>
ID3D11VertexShader* OPTadaC_ResourceManager::Get_Shader(OPTadaE_ShaderList_ForResoursManager shaderEnum_)
{
    return shaderMass[shaderEnum_].linkOn_VertexShader;
}

template<>
ID3D11PixelShader* OPTadaC_ResourceManager::Get_Shader(OPTadaE_ShaderList_ForResoursManager shaderEnum_)
{
    return shaderMass[shaderEnum_].linkOn_PixelShader;
}

