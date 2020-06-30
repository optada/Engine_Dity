// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#include "OPTadaC_ResourceManager.h"


void OPTadaC_ResourceManager::FreeAll()
{
    // free all shaders
    OPTadaS_ShaderStructure* shaderCell = nullptr;
    for (int i = 1; i < OPTadaE_ShaderList_ForResoursManager::ENUM_ShaderList_ForResoursManager_MaxCount; i++) {
        shaderCell = &shaderMass[i];
        SafeRelease(shaderCell->inputLayout_VertexBuffer);
        SafeRelease(shaderCell->linkOn_PixelShader);
        SafeRelease(shaderCell->linkOn_VertexShader);
    }

    // free all meshes from GPU memory
    OPTadaS_MeshStructure* meshCell = nullptr;
    for (int i = 1; i < OPTadaE_MeshName_ForResoursManager::ENUM_MeshName_ForResoursManager_MaxCount; i++) {
        SafeRelease(meshCell->vertexBuffer_GPU);
        SafeRelease(meshCell->indexBuffer_GPU);
        meshCell->isInGPUMemory = false;
    }

    // free all constant buffers
    ID3D11Buffer* constantBuffer = nullptr;
    for (int i = 1; i < OPTadaE_ConstantBuffersList_ForResoursManager::OPTadaE_ConstantBuffersList_ForResoursManager_MaxCount; i++) {
        SafeRelease(constantBuffer);
    }

    // free ...
}


template<>
bool OPTadaC_ResourceManager::Add_Shader(OPTadaE_ShaderList_ForResoursManager shaderEnum_, ID3D11VertexShader* newShader_, ID3D11InputLayout* inputLayout_)
{
    OPTadaS_ShaderStructure& cell = shaderMass[shaderEnum_];
    if (cell.shaderEnum == OPTadaE_ShaderList_ForResoursManager::ENUM_ShaderList_NONE && newShader_) {
        cell.inputLayout_VertexBuffer = inputLayout_;
        cell.shaderEnum = shaderEnum_;
        cell.linkOn_VertexShader = newShader_;
        
        return true; 
    }

    return false;
}

template<>
bool OPTadaC_ResourceManager::Add_Shader(OPTadaE_ShaderList_ForResoursManager shaderEnum_, ID3D11PixelShader* newShader_, ID3D11InputLayout* inputLayout_)
{
    OPTadaS_ShaderStructure& cell = shaderMass[shaderEnum_];
    if (cell.shaderEnum == OPTadaE_ShaderList_ForResoursManager::ENUM_ShaderList_NONE && newShader_) {
        cell.inputLayout_VertexBuffer = inputLayout_;
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
        SafeRelease(cell.inputLayout_VertexBuffer);
        SafeRelease(cell.linkOn_PixelShader);
        SafeRelease(cell.linkOn_VertexShader);
        cell.shaderEnum = ENUM_ShaderList_NONE;

        return true;
    }

    return false;
}

template<>
inline ID3D11VertexShader* OPTadaC_ResourceManager::Get_Shader(OPTadaE_ShaderList_ForResoursManager shaderEnum_, ID3D11InputLayout** inputLayout_)
{
    *inputLayout_ = shaderMass[shaderEnum_].inputLayout_VertexBuffer;
    return shaderMass[shaderEnum_].linkOn_VertexShader;
}

template<>
inline ID3D11PixelShader* OPTadaC_ResourceManager::Get_Shader(OPTadaE_ShaderList_ForResoursManager shaderEnum_, ID3D11InputLayout** inputLayout_)
{
    return shaderMass[shaderEnum_].linkOn_PixelShader;
}


bool OPTadaC_ResourceManager::Add_Mesh(OPTadaE_MeshName_ForResoursManager meshName_, ID3D11Buffer* vertexBuffer_, ID3D11Buffer* indexBuffer_, UINT vertexStride_, UINT vertexOffset_, DXGI_FORMAT indexBufferFormat_)
{
    OPTadaS_MeshStructure* meshCell = &meshMass[meshName_];

    if (vertexBuffer_ == nullptr || vertexStride_ == 0 || (indexBuffer_ != nullptr && indexBufferFormat_ == DXGI_FORMAT::DXGI_FORMAT_UNKNOWN)) {
        return false;
    }

    meshCell->vertexBuffer_InMEM = vertexBuffer_;
    meshCell->indexBuffer_InMEM  = vertexBuffer_;   
    meshCell->vertexStride       = vertexStride_;
    meshCell->vectorOffset       = vertexOffset_;
    meshCell->indexBufferFormat  = indexBufferFormat_;
    meshCell->isInGPUMemory      = false;

    return true;
}

void OPTadaC_ResourceManager::SetToDefault_MeshCell(OPTadaE_MeshName_ForResoursManager meshName_)
{
    OPTadaS_MeshStructure* meshCell = &meshMass[meshName_];

    if (meshCell->isInGPUMemory == true) {
        Unload_FromGPU_Mesh(meshName_);
    }

    meshCell->indexBuffer_GPU    = nullptr;
    meshCell->indexBuffer_InMEM  = nullptr;
    meshCell->vertexBuffer_GPU   = nullptr;
    meshCell->vertexBuffer_InMEM = nullptr;

    meshCell->ByteWidth_VertexBuffer_InMEM = 0;
    meshCell->ByteWidth_IndexBuffer_InMEM = 0;

    meshCell->vertexStride      = 0;
    meshCell->vectorOffset      = 0;
    meshCell->isInGPUMemory     = false;
    meshCell->indexBufferFormat = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;

    meshCell->meshName          = ENUM_MeshName_NONE;
}

bool OPTadaC_ResourceManager::Load_ToGPU_Mesh(OPTadaE_MeshName_ForResoursManager meshName_, ID3D11Device* device_d3d11_)
{
    OPTadaS_MeshStructure* meshCell = &meshMass[meshName_];

    if (!device_d3d11_ || meshCell->meshName == ENUM_MeshName_NONE || meshCell->isInGPUMemory) {
        return false;
    }

    D3D11_SUBRESOURCE_DATA resourceData;
    ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));
    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
    HRESULT hr;

    // Create an initialize the vertex buffer.
    vertexBufferDesc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.ByteWidth      = meshCell->ByteWidth_VertexBuffer_InMEM;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
    resourceData.pSysMem = meshCell->vertexBuffer_InMEM;

    hr = device_d3d11_->CreateBuffer(&vertexBufferDesc, &resourceData, &meshCell->vertexBuffer_GPU);
    if (FAILED(hr)) {
        return false;
    }

    if (meshCell->vertexBuffer_InMEM) { // if we have memory link on index buffer

        // Create and initialize the index buffer.
        D3D11_BUFFER_DESC indexBufferDesc;
        ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));

        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.ByteWidth = meshCell->ByteWidth_IndexBuffer_InMEM;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        resourceData.pSysMem = meshCell->indexBuffer_InMEM;

        hr = device_d3d11_->CreateBuffer(&indexBufferDesc, &resourceData, &meshCell->indexBuffer_GPU);
        if (FAILED(hr)) {
            return false;
        }
    }

    meshCell->isInGPUMemory = true; // we are in game now!

    return true;
}

void OPTadaC_ResourceManager::Unload_FromGPU_Mesh(OPTadaE_MeshName_ForResoursManager meshName_)
{
    OPTadaS_MeshStructure* meshCell = &meshMass[meshName_];
    SafeRelease(meshCell->vertexBuffer_GPU);
    SafeRelease(meshCell->indexBuffer_GPU);
    meshCell->isInGPUMemory = false;
}

inline OPTadaS_MeshStructure* OPTadaC_ResourceManager::Get_MeshCell(OPTadaE_MeshName_ForResoursManager meshName_)
{
    OPTadaS_MeshStructure* meshCell = &meshMass[meshName_];
    return (meshCell->meshName != ENUM_MeshName_NONE) ? (meshCell) : (nullptr);
}

inline OPTadaS_MeshStructure* OPTadaC_ResourceManager::Get_MeshCell_IfInGPU(OPTadaE_MeshName_ForResoursManager meshName_)
{
    OPTadaS_MeshStructure* meshCell = &meshMass[meshName_];
    return (meshCell->isInGPUMemory && meshCell->meshName != ENUM_MeshName_NONE) ? (meshCell) : (nullptr);
}


inline void OPTadaC_ResourceManager::Set_ConstantBuffer(OPTadaE_ConstantBuffersList_ForResoursManager constantBufferID_, ID3D11Buffer* constantBuffer_)
{
    constantBuffersMass[constantBufferID_] = constantBuffer_;
}

inline ID3D11Buffer* OPTadaC_ResourceManager::Get_ConstantBuffer(OPTadaE_ConstantBuffersList_ForResoursManager constantBufferID_)
{
    return constantBuffersMass[constantBufferID_];
}
