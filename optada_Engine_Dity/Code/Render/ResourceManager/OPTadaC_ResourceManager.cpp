// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#include "OPTadaC_ResourceManager.h"


void OPTadaC_ResourceManager::FreeAll()
{
    // free all pixel shaders
    OPTadaS_PixelShaderStructure* shaderCell_PS = nullptr;
    for (int i = 1; i < OPTadaE_PixelShaderList_ForResoursManager::ENUM_PixelShaderList_ForResoursManager_MaxCount; i++) {
        shaderCell_PS = &PS_Mass[i];
        SafeRelease(shaderCell_PS->pixelShader);
    }

    // free all vertex shaders
    OPTadaS_VertexShaderStructure* shaderCell_VS = nullptr;
    for (int i = 1; i < OPTadaE_VertexShaderList_ForResoursManager::ENUM_VertexShaderList_ForResoursManager_MaxCount; i++) {
        shaderCell_VS = &VS_Mass[i];
        SafeRelease(shaderCell_VS->inputLayout);
        SafeRelease(shaderCell_VS->vertexShader);
    }

    // free all meshes from GPU memory
    OPTadaS_MeshStructure* meshCell = nullptr;
    for (int i = 1; i < OPTadaE_MeshList_ForResoursManager::ENUM_MeshList_ForResoursManager_MaxCount; i++) {
        SafeRelease(meshCell->vertexBuffer_GPU);
        SafeRelease(meshCell->indexBuffer_GPU);
        meshCell->isInGPUMemory = false;
    }

    // free all constant buffers
    ID3D11Buffer* constantBuffer = nullptr;
    for (int i = 1; i < OPTadaE_ConstantBuffersList_ForResoursManager::OPTadaE_ConstantBuffersList_ForResoursManager_MaxCount; i++) {
        SafeRelease(constantBuffer);
    }

    // free all textures
    OPTadaS_TextureStructure* textureCell = nullptr;
    for (int i = 1; i < OPTadaE_TextureList_ForResoursManager::ENUM_TextureList_ForResoursManager_MaxCount; i++) {
        SafeRelease(textureCell->textureSamplerState);
        SafeRelease(textureCell->texture);
        textureCell->isInGPUMemory = false;
    }
}


bool OPTadaC_ResourceManager::Create_PixelShader_FromBinaryFile(OPTadaE_PixelShaderList_ForResoursManager shaderEnum_, const std::wstring& fileName_, ID3D11Device* gDevice_)
{
    ID3D11PixelShader* newPixelShader = nullptr;
    ID3DBlob*          pixelShaderBlob;
    HRESULT            hr;

    OPTadaS_PixelShaderStructure& cell = PS_Mass[shaderEnum_];

    if (cell.shaderEnum != OPTadaE_PixelShaderList_ForResoursManager::ENUM_PixelShaderList_NONE 
        || shaderEnum_ == OPTadaE_PixelShaderList_ForResoursManager::ENUM_PixelShaderList_NONE 
        || cell.pixelShader != nullptr
        || gDevice_ == nullptr) 
    {
        return false;
    }


    hr = D3DReadFileToBlob(fileName_.c_str(), &pixelShaderBlob);
    if (FAILED(hr)) {
        return false; // can't read binary file
    }

    hr = gDevice_->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &newPixelShader);
    if (FAILED(hr)) {
        SafeRelease(pixelShaderBlob);
        return false; // compile failed
    }

    SafeRelease(pixelShaderBlob);

    cell.shaderEnum = shaderEnum_;
    cell.pixelShader = newPixelShader;

    return true;
}

inline OPTadaS_PixelShaderStructure* OPTadaC_ResourceManager::Get_PixelShader_Cell(OPTadaE_PixelShaderList_ForResoursManager shaderEnum_)
{
    return (shaderEnum_ != OPTadaE_PixelShaderList_ForResoursManager::ENUM_PixelShaderList_ForResoursManager_MaxCount)?(&PS_Mass[shaderEnum_]):(nullptr);
}

inline bool OPTadaC_ResourceManager::Use_PixelShader(OPTadaE_PixelShaderList_ForResoursManager shaderEnum_)
{
    return false;
}

bool OPTadaC_ResourceManager::Delete_PixelShader(OPTadaE_PixelShaderList_ForResoursManager shaderEnum_)
{
    OPTadaS_PixelShaderStructure& cell = PS_Mass[shaderEnum_];
    if (cell.shaderEnum != OPTadaE_PixelShaderList_ForResoursManager::ENUM_PixelShaderList_NONE) {
        SafeRelease(cell.pixelShader);
        cell.shaderEnum = ENUM_PixelShaderList_NONE;
        return true;
    }

    return false;
}


bool OPTadaC_ResourceManager::Create_VertexShader_FromBinaryFile(OPTadaE_VertexShaderList_ForResoursManager shaderEnum_, const std::wstring& fileName_, ID3D11Device* gDevice_, D3D11_INPUT_ELEMENT_DESC* vertexLayoutDesc_, UINT countOfvertexLayoutDesc_)
{
    ID3D11VertexShader* newVertexShader = nullptr;
    ID3DBlob*           vertexShaderBlob;
    HRESULT             hr;

    OPTadaS_VertexShaderStructure& cell = VS_Mass[shaderEnum_];

    if (cell.shaderEnum != OPTadaE_VertexShaderList_ForResoursManager::ENUM_VertexShaderList_NONE
        || shaderEnum_ == OPTadaE_VertexShaderList_ForResoursManager::ENUM_VertexShaderList_NONE
        || cell.vertexShader != nullptr
        || cell.inputLayout != nullptr
        || gDevice_ == nullptr)
    {
        return false;
    }


    hr = D3DReadFileToBlob(fileName_.c_str(), &vertexShaderBlob);
    if (FAILED(hr)) {
        return false; // can't read binary file
    }

    hr = gDevice_->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &newVertexShader);
    if (FAILED(hr)) {
        SafeRelease(vertexShaderBlob);
        return false; // compile failed
    }

    hr = gDevice_->CreateInputLayout(vertexLayoutDesc_, countOfvertexLayoutDesc_, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &cell.inputLayout);
    if (FAILED(hr)) {
        SafeRelease(vertexShaderBlob);
        return false; // can't create input layout for vertex buffer
    }

    SafeRelease(vertexShaderBlob);

    cell.shaderEnum = shaderEnum_;
    cell.vertexShader = newVertexShader;

    return true;

}

inline OPTadaS_VertexShaderStructure* OPTadaC_ResourceManager::Get_VertexShader_Cell(OPTadaE_VertexShaderList_ForResoursManager shaderEnum_)
{
    return (shaderEnum_ != OPTadaE_VertexShaderList_ForResoursManager::ENUM_VertexShaderList_ForResoursManager_MaxCount) ? (&VS_Mass[shaderEnum_]) : (nullptr);
}

inline bool OPTadaC_ResourceManager::Use_VertexShader(OPTadaE_VertexShaderList_ForResoursManager shaderEnum_)
{
    return false;
}

bool OPTadaC_ResourceManager::Delete_VertexShader(OPTadaE_VertexShaderList_ForResoursManager shaderEnum_)
{
    OPTadaS_VertexShaderStructure& cell = VS_Mass[shaderEnum_];
    if (cell.shaderEnum != OPTadaE_VertexShaderList_ForResoursManager::ENUM_VertexShaderList_NONE) {
        SafeRelease(cell.inputLayout);
        SafeRelease(cell.vertexShader);
        cell.shaderEnum = ENUM_VertexShaderList_NONE;
        return true;
    }

    return false;
}


bool OPTadaC_ResourceManager::Add_Mesh(OPTadaE_MeshList_ForResoursManager meshName_, ID3D11Buffer* vertexBuffer_, ID3D11Buffer* indexBuffer_, UINT vertexStride_, UINT vertexOffset_, DXGI_FORMAT indexBufferFormat_)
{
    OPTadaS_MeshStructure* meshCell = &mesh_Mass[meshName_];

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

void OPTadaC_ResourceManager::SetToDefault_MeshCell(OPTadaE_MeshList_ForResoursManager meshName_)
{
    OPTadaS_MeshStructure* meshCell = &mesh_Mass[meshName_];

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

    meshCell->meshName          = ENUM_MeshList_NONE;
}

bool OPTadaC_ResourceManager::Load_ToGPU_Mesh(OPTadaE_MeshList_ForResoursManager meshName_, ID3D11Device* device_d3d11_)
{
    OPTadaS_MeshStructure* meshCell = &mesh_Mass[meshName_];

    if (!device_d3d11_ || meshCell->meshName == ENUM_MeshList_NONE || meshCell->isInGPUMemory) {
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

void OPTadaC_ResourceManager::Unload_FromGPU_Mesh(OPTadaE_MeshList_ForResoursManager meshName_)
{
    OPTadaS_MeshStructure* meshCell = &mesh_Mass[meshName_];
    SafeRelease(meshCell->vertexBuffer_GPU);
    SafeRelease(meshCell->indexBuffer_GPU);
    meshCell->isInGPUMemory = false;
}

inline OPTadaS_MeshStructure* OPTadaC_ResourceManager::Get_MeshCell(OPTadaE_MeshList_ForResoursManager meshName_)
{
    OPTadaS_MeshStructure* meshCell = &mesh_Mass[meshName_];
    return (meshCell->meshName != ENUM_MeshList_NONE) ? (meshCell) : (nullptr);
}

inline OPTadaS_MeshStructure* OPTadaC_ResourceManager::Get_MeshCell_IfInGPU(OPTadaE_MeshList_ForResoursManager meshName_)
{
    OPTadaS_MeshStructure* meshCell = &mesh_Mass[meshName_];
    return (meshCell->isInGPUMemory && meshCell->meshName != ENUM_MeshList_NONE) ? (meshCell) : (nullptr);
}


//inline void OPTadaC_ResourceManager::Set_ConstantBuffer(OPTadaE_ConstantBuffersList_ForResoursManager constantBufferID_, ID3D11Buffer* constantBuffer_)
//{
//    constantBuffersMass[constantBufferID_] = constantBuffer_;
//}

//inline ID3D11Buffer* OPTadaC_ResourceManager::Get_ConstantBuffer(OPTadaE_ConstantBuffersList_ForResoursManager constantBufferID_)
//{
//    return constantBuffersMass[constantBufferID_];
//}


bool OPTadaC_ResourceManager::Create_Texture_LoadFromFile(OPTadaE_TextureList_ForResoursManager textureEnum_, const std::wstring& fileName_, ID3D11Device* gDevice_)
{
    OPTadaS_TextureStructure& cell = texture_Mass[textureEnum_];
    HRESULT                   hr;

    if (cell.textureEnum != OPTadaE_TextureList_ForResoursManager::ENUM_TextureList_NONE 
        || textureEnum_ == OPTadaE_TextureList_ForResoursManager::ENUM_TextureList_NONE
        || cell.texture != nullptr
        || cell.textureSamplerState != nullptr
        || gDevice_ == nullptr)
    {
        return false;
    }

    hr = D3DX11CreateShaderResourceViewFromFile(gDevice_, fileName_.c_str(), NULL, NULL, &cell.texture, NULL);
    if (FAILED(hr)) {
        return false; // can't load texture from file
    }

    // create texture sampler state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD         = 0;
    sampDesc.MaxLOD         = D3D11_FLOAT32_MAX;

    hr = gDevice_->CreateSamplerState(&sampDesc, &cell.textureSamplerState);
    if (FAILED(hr)) {
        SafeRelease(cell.texture); // free texture
        return false; // can't create sample state
    }

    cell.textureEnum = textureEnum_;
    cell.isInGPUMemory = true;

    return true;
}

OPTadaS_TextureStructure* OPTadaC_ResourceManager::Get_Texture_Cell(OPTadaE_TextureList_ForResoursManager textureEnum_)
{
    return (textureEnum_ != OPTadaE_TextureList_ForResoursManager::ENUM_TextureList_ForResoursManager_MaxCount) ? (&texture_Mass[textureEnum_]) : (nullptr);;
}

bool OPTadaC_ResourceManager::Delete_Texture_FromGPU()
{
    return false;
}
