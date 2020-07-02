// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#include "OPTadaC_ResourceManager.h"


bool OPTadaC_ResourceManager::Init_ResourceManager()
{
    memManager.Init_Mamager(2);
    // 500000000 byte ~ 500 mb
    if (!(memKey1 = memManager.CreateNewMemoryBuffer(1, 500000000, 5000, 8, OPTadaE_BufferTypes_ForMemoryManager::ENUM_SimpleMemoryBuffer))) {
        return false;
    }

    return true;
}

void OPTadaC_ResourceManager::FreeAll()
{
    // free all pixel shaders
    OPTadaS_PixelShaderStructure* shaderCell_PS = nullptr;
    for (int i = 0; i < OPTadaE_PixelShaderList_ForResoursManager::ENUM_PixelShaderList_ForResoursManager_MaxCount; i++) {
        shaderCell_PS = &PS_Mass[i];
        shaderCell_PS->Free_GPU();
    }

    // free all vertex shaders
    OPTadaS_VertexShaderStructure* shaderCell_VS = nullptr;
    for (int i = 0; i < OPTadaE_VertexShaderList_ForResoursManager::ENUM_VertexShaderList_ForResoursManager_MaxCount; i++) {
        shaderCell_VS = &VS_Mass[i];
        shaderCell_VS->Free_GPU();
    }

    // free all meshes from GPU memory
    OPTadaS_MeshStructure* meshCell = nullptr;
    for (int i = 0; i < OPTadaE_MeshList_ForResoursManager::ENUM_MeshList_ForResoursManager_MaxCount; i++) {
        meshCell = &mesh_Mass[i];
        meshCell->Free_GPU();
    }

    // free all constant buffers
    ID3D11Buffer* constantBuffer = nullptr;
    for (int i = 0; i < OPTadaE_ConstantBuffersList_ForResoursManager::OPTadaE_ConstantBuffersList_ForResoursManager_MaxCount; i++) {
        constantBuffer = constantBuffers_Mass[i];
        SafeRelease(constantBuffer);
    }

    // free all textures
    OPTadaS_TextureStructure* textureCell = nullptr;
    for (int i = 0; i < OPTadaE_TextureList_ForResoursManager::ENUM_TextureList_ForResoursManager_MaxCount; i++) {
        textureCell = &texture_Mass[i];
        textureCell->Free_GPU();
    }

    memManager.Free_Manager();
}


bool OPTadaC_ResourceManager::Create_PixelShader_FromBinaryFile(OPTadaE_PixelShaderList_ForResoursManager& shaderEnum_, const std::wstring& fileName_, ID3D11Device* gDevice_)
{
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

    hr = gDevice_->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &cell.pixelShader);
    if (FAILED(hr)) {
        SafeRelease(pixelShaderBlob);
        return false; // compile failed
    }

    SafeRelease(pixelShaderBlob);

    cell.shaderEnum = shaderEnum_;
    cell.isInGPUMemory = true;

    return true;
}

inline OPTadaS_PixelShaderStructure* OPTadaC_ResourceManager::Get_PixelShader_Cell(OPTadaE_PixelShaderList_ForResoursManager& shaderEnum_)
{
    return (shaderEnum_ != OPTadaE_PixelShaderList_ForResoursManager::ENUM_PixelShaderList_ForResoursManager_MaxCount)?(&PS_Mass[shaderEnum_]):(nullptr);
}

bool OPTadaC_ResourceManager::Use_PixelShader(OPTadaE_PixelShaderList_ForResoursManager& shaderEnum_, ID3D11DeviceContext* gDeviceContext_)
{
    OPTadaS_PixelShaderStructure& cell = PS_Mass[shaderEnum_];
    if (cell.isInGPUMemory) {
        gDeviceContext_->PSSetShader(cell.pixelShader, nullptr, 0);
        return true;
    }

    return false;
}

bool OPTadaC_ResourceManager::Delete_PixelShader(OPTadaE_PixelShaderList_ForResoursManager& shaderEnum_)
{
    OPTadaS_PixelShaderStructure& cell = PS_Mass[shaderEnum_];
    if (cell.shaderEnum != OPTadaE_PixelShaderList_ForResoursManager::ENUM_PixelShaderList_NONE) {
        cell.Free_GPU();
        cell.shaderEnum = ENUM_PixelShaderList_NONE;
        return true;
    }

    return false;
}


bool OPTadaC_ResourceManager::Create_VertexShader_FromBinaryFile(OPTadaE_VertexShaderList_ForResoursManager& shaderEnum_, const std::wstring& fileName_, ID3D11Device* gDevice_, D3D11_INPUT_ELEMENT_DESC* vertexLayoutDesc_, UINT countOfvertexLayoutDesc_)
{
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

    hr = gDevice_->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &cell.vertexShader);
    if (FAILED(hr)) {
        SafeRelease(vertexShaderBlob);
        return false; // compile failed
    }

    hr = gDevice_->CreateInputLayout(vertexLayoutDesc_, countOfvertexLayoutDesc_, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &cell.inputLayout);
    if (FAILED(hr)) {
        SafeRelease(vertexShaderBlob);
        cell.Free_GPU();
        return false; // can't create input layout for vertex buffer
    }

    SafeRelease(vertexShaderBlob);

    cell.shaderEnum = shaderEnum_;
    cell.isInGPUMemory = true;

    return true;

}

inline OPTadaS_VertexShaderStructure* OPTadaC_ResourceManager::Get_VertexShader_Cell(OPTadaE_VertexShaderList_ForResoursManager& shaderEnum_)
{
    return (shaderEnum_ != OPTadaE_VertexShaderList_ForResoursManager::ENUM_VertexShaderList_ForResoursManager_MaxCount) ? (&VS_Mass[shaderEnum_]) : (nullptr);
}

bool OPTadaC_ResourceManager::Use_VertexShader(OPTadaE_VertexShaderList_ForResoursManager& shaderEnum_, ID3D11DeviceContext* gDeviceContext_)
{
    OPTadaS_VertexShaderStructure& cell = VS_Mass[shaderEnum_];
    if (cell.isInGPUMemory) {
        gDeviceContext_->IASetInputLayout(cell.inputLayout);
        gDeviceContext_->VSSetShader(cell.vertexShader, nullptr, 0);
        return true;
    }

    return false;
}

bool OPTadaC_ResourceManager::Delete_VertexShader(OPTadaE_VertexShaderList_ForResoursManager& shaderEnum_)
{
    OPTadaS_VertexShaderStructure& cell = VS_Mass[shaderEnum_];
    if (cell.shaderEnum != OPTadaE_VertexShaderList_ForResoursManager::ENUM_VertexShaderList_NONE) {
        cell.Free_GPU();
        cell.shaderEnum = ENUM_VertexShaderList_NONE;
        return true;
    }

    return false;
}


bool OPTadaC_ResourceManager::Create_Mesh_FromFileToMem(OPTadaE_MeshList_ForResoursManager& meshName_, const std::string fileName_, ID3D11Device* gDevice_, UINT vertexStride_, UINT vertexOffset_, DXGI_FORMAT indexBufferFormat_)
{
    OPTadaS_MeshStructure* meshCell = &mesh_Mass[meshName_];

    if (vertexStride_ == 0 || !gDevice_ || meshCell->meshName != OPTadaE_MeshList_ForResoursManager::ENUM_MeshList_NONE) {
        return false;
    }

    std::vector<Vertex_F3Coord_F3Normal_F2TextCoord> vertexMass;
    std::vector<UINT> indexMass;

    if (!load_SimpleMesh_FromOBJFile_Vertex_CoordTextCoordNormal_Indexes_UINT(fileName_, vertexMass, indexMass)) {
        return false;
    }
    
    // load mesh to our memory buffer
    int    i   = 0;
    size_t maxVertex = vertexMass.size();
    size_t maxIndex = vertexMass.size();

    meshCell->vertexBuffer_InMEM = memManager.GetMemory(memKey1, sizeof(Vertex_F3Coord_F3Normal_F2TextCoord) * maxVertex);
    meshCell->indexBuffer_InMEM = memManager.GetMemory(memKey1, sizeof(WORD) * maxIndex);

    if (!meshCell->vertexBuffer_InMEM || !meshCell->indexBuffer_InMEM) { // get memory error
        memManager.ReturnMemory(memKey1, meshCell->vertexBuffer_InMEM);
        memManager.ReturnMemory(memKey1, meshCell->indexBuffer_InMEM);
        return false;
    }

    //copy vertex to mem
    Vertex_F3Coord_F3Normal_F2TextCoord* vertexMassPointer = (Vertex_F3Coord_F3Normal_F2TextCoord*) meshCell->vertexBuffer_InMEM;
    for (i = 0; i < maxVertex; i++) {
        vertexMassPointer[i].position     = vertexMass[i].position;
        vertexMassPointer[i].normal       = vertexMass[i].normal;
        vertexMassPointer[i].textureCoord = vertexMass[i].textureCoord;
    }

    //copy index to mem
    DWORD* indexMassPointer = (DWORD*)meshCell->indexBuffer_InMEM;
    for (i = 0; i < maxIndex; i++) {
        indexMassPointer[i] = indexMass[i];
    }

    meshCell->indexBufferCount   = maxIndex;
    meshCell->vertexStride       = vertexStride_;
    meshCell->vertexOffset       = vertexOffset_;
    meshCell->indexBufferFormat  = indexBufferFormat_;
    meshCell->isInGPUMemory      = false;

    return true;
}

void OPTadaC_ResourceManager::SetToDefault_MeshCell(OPTadaE_MeshList_ForResoursManager& meshName_)
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
    meshCell->vertexOffset      = 0;
    meshCell->isInGPUMemory     = false;
    meshCell->indexBufferFormat = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;

    meshCell->meshName          = ENUM_MeshList_NONE;
}

bool OPTadaC_ResourceManager::Load_ToGPU_Mesh(OPTadaE_MeshList_ForResoursManager& meshName_, ID3D11Device* device_d3d11_)
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

void OPTadaC_ResourceManager::Unload_FromGPU_Mesh(OPTadaE_MeshList_ForResoursManager& meshName_)
{
    OPTadaS_MeshStructure* meshCell = &mesh_Mass[meshName_];
    meshCell->Free_GPU();
}

bool OPTadaC_ResourceManager::Use_Mesh_WithIndexBuffer(OPTadaE_MeshList_ForResoursManager& meshName_, ID3D11DeviceContext* gDeviceContext_)
{
    OPTadaS_MeshStructure& cell = mesh_Mass[meshName_];
    if (cell.isInGPUMemory) {
        gDeviceContext_->IASetVertexBuffers(0, 1, &cell.vertexBuffer_GPU, &cell.vertexStride, &cell.vertexOffset);
        gDeviceContext_->IASetIndexBuffer(cell.indexBuffer_GPU, cell.indexBufferFormat, 0);
        return true;
    }

    return false;
}

inline OPTadaS_MeshStructure* OPTadaC_ResourceManager::Get_MeshCell(OPTadaE_MeshList_ForResoursManager& meshName_)
{
    OPTadaS_MeshStructure* meshCell = &mesh_Mass[meshName_];
    return (meshCell->meshName != ENUM_MeshList_NONE) ? (meshCell) : (nullptr);
}

inline OPTadaS_MeshStructure* OPTadaC_ResourceManager::Get_MeshCell_IfInGPU(OPTadaE_MeshList_ForResoursManager& meshName_)
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


bool OPTadaC_ResourceManager::Create_Texture_LoadFromFile(OPTadaE_TextureList_ForResoursManager& textureEnum_, const std::wstring& fileName_, ID3D11Device* gDevice_)
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

inline OPTadaS_TextureStructure* OPTadaC_ResourceManager::Get_Texture_Cell(OPTadaE_TextureList_ForResoursManager& textureEnum_)
{
    return (textureEnum_ != OPTadaE_TextureList_ForResoursManager::ENUM_TextureList_ForResoursManager_MaxCount) ? (&texture_Mass[textureEnum_]) : (nullptr);
}

inline OPTadaS_TextureStructure* OPTadaC_ResourceManager::Get_Texture_Cell_IfInGPU(OPTadaE_TextureList_ForResoursManager& textureEnum_)
{
    OPTadaS_TextureStructure* textureCell = &texture_Mass[textureEnum_];
    return (textureEnum_ != OPTadaE_TextureList_ForResoursManager::ENUM_TextureList_ForResoursManager_MaxCount && textureCell->isInGPUMemory) ? (textureCell) : (nullptr);;
}

inline bool OPTadaC_ResourceManager::Use_Texture(OPTadaE_TextureList_ForResoursManager& textureEnum_, ID3D11DeviceContext* gDeviceContext_, UINT resourceSlot_)
{
    OPTadaS_TextureStructure* cell = &texture_Mass[textureEnum_];
    if (cell->isInGPUMemory) {
        gDeviceContext_->CSSetShaderResources(resourceSlot_, 0, &cell->texture);
        gDeviceContext_->CSSetSamplers(resourceSlot_, 0, &cell->textureSamplerState);
    }

    return false;
}

bool OPTadaC_ResourceManager::Delete_Texture(OPTadaE_TextureList_ForResoursManager& textureEnum_)
{
    OPTadaS_TextureStructure& cell = texture_Mass[textureEnum_];
    if (cell.textureEnum != OPTadaE_TextureList_ForResoursManager::ENUM_TextureList_NONE) {
        cell.Free_GPU();
        cell.textureEnum = OPTadaE_TextureList_ForResoursManager::ENUM_TextureList_NONE;
        return true;
    }

    return false;
}



bool load_SimpleMesh_FromOBJFile_Vertex_CoordTextCoordNormal_Indexes_UINT(const std::string fileName_, std::vector<Vertex_F3Coord_F3Normal_F2TextCoord>& outputVertexMass_, std::vector<UINT>& outputIndexMass_)
{
    UINT numberOfVertices = 0, numberOfNormals = 0, numberOfUVs = 0, numberOfFaces = 0;
    std::vector<Vertex_F3Coord_F3Normal_F2TextCoord> outputDaMettere;
    std::vector< UINT> vertexIndices, uvIndices, normalIndices;
    std::vector< XMFLOAT3 > temp_vertices;
    std::vector< XMFLOAT2 > temp_uvs;
    std::vector< XMFLOAT3 > temp_normals;

    std::vector<XMFLOAT3> outVertices;
    std::vector<XMFLOAT3> outNormals;
    std::vector<XMFLOAT2> outUvs;

    FILE* file = fopen(fileName_.c_str(), "r");
    if (file == NULL) {
        MessageBox(0, L"Can't reade .OBJ file", 0, 0);
        return false;
    }
    while (1) {
        char lineHeader[128];
        int res = fscanf(file, "%s \n", lineHeader);
        if (res == EOF)
            break;

        if (strcmp(lineHeader, "v") == 0) {
            XMFLOAT3 vertex;
            int a = fscanf(file, "%f %f %f \n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
            numberOfVertices++;
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            XMFLOAT2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
            numberOfUVs++;
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            XMFLOAT3 normals;
            fscanf(file, "%f %f %f \n", &normals.x, &normals.y, &normals.z);
            temp_normals.push_back(normals);
            numberOfNormals++;
        }
        else if (strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            UINT vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%i %i %i %i %i %i %i %i %i \n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

            if (matches != 9) {
                MessageBox(0, L"File cant be read", 0, 0);
                break;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
            numberOfFaces++;
        }
    }

    numberOfFaces *= 3;
    int cont = 0;
    for (int i = 0; i < numberOfFaces; i++) {

        UINT vertexIndex = vertexIndices[i];
        UINT normalIndex = normalIndices[i];
        UINT uvIndex     = uvIndices[i];
        Vertex_F3Coord_F3Normal_F2TextCoord temp;
        temp.position      = temp_vertices[vertexIndex - 1];
        temp.normal   = temp_normals[normalIndex - 1];
        temp.textureCoord = temp_uvs[uvIndex - 1];

        outputVertexMass_.push_back(temp);

        cont++;
    }
    for (int i = 0; i < numberOfFaces; i++) {
        vertexIndices[i]--;  //Starts from 1, i need it to start from 0
    }
    int toAdd = 0;
    for (int i = 0; i < numberOfFaces; i += 3) { // Needed to get the indices working for my draw function
        vertexIndices[i] += toAdd;
        vertexIndices[++i] += toAdd;
        vertexIndices[++i] += toAdd;
        //vertexIndices[i + 1] += toAdd;
        //vertexIndices[i + 2] += toAdd;
        //toAdd += 2;
    }
    outputIndexMass_ = vertexIndices;

    return true;
}


