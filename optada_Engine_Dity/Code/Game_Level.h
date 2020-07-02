// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "Render\OPTada_Render.h"
#include "Render\ResourceManager\OPTadaC_ResourceManager.h"


// Vertex buffer data
ID3D11InputLayout* g_d3dInputLayout = nullptr;
ID3D11Buffer* g_d3dVertexBuffer = nullptr;
ID3D11Buffer* g_d3dIndexBuffer = nullptr;

// Shader data
ID3D11VertexShader* g_d3dVertexShader = nullptr;
ID3D11PixelShader* g_d3dPixelShader = nullptr;

// Shader resources
enum ConstanBuffer
{
    CB_Appliation,
    CB_Frame,
    CB_Object,
    NumConstantBuffers
};

ID3D11Buffer* g_d3dConstantBuffers[NumConstantBuffers];

// Demo parameters
XMMATRIX g_WorldMatrix;
XMMATRIX g_ViewMatrix;
XMMATRIX g_ProjectionMatrix;

// Vertex data for a colored cube.
struct VertexPosColor
{
    XMFLOAT3 Position;
    XMFLOAT3 Color;
};

Vertex_F3Coord_F3Normal_F2TextCoord g_Vertices[8] =
{
    { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) }, // 0
    { XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) }, // 1
    { XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) }, // 2
    { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) }, // 3
    { XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) }, // 4
    { XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) }, // 5
    { XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) }, // 6
    { XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) }  // 7
};

WORD g_Indicies[36] =
{
    0, 1, 2, 0, 2, 3,
    4, 6, 5, 4, 7, 6,
    4, 5, 1, 4, 1, 0,
    3, 2, 6, 3, 6, 7,
    1, 5, 6, 1, 6, 2,
    4, 0, 3, 4, 3, 7
};




class GameLevel
{
public:
	
	// use it for initialize level and level's resourses
	bool Init()
	{
        assert(global_Render.g_Device_d3d11);

        // Create an initialize the vertex buffer.

        D3D11_SUBRESOURCE_DATA resourceData;
        ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));

        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.ByteWidth = sizeof(Vertex_F3Coord_F3Normal_F2TextCoord) * _countof(g_Vertices);
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        resourceData.pSysMem = g_Vertices;

        HRESULT hr = global_Render.g_Device_d3d11->CreateBuffer(&vertexBufferDesc, &resourceData, &g_d3dVertexBuffer);
        if (FAILED(hr))
        {
            return false;
        }

        // Create and initialize the index buffer.
        D3D11_BUFFER_DESC indexBufferDesc;
        ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));

        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.ByteWidth = sizeof(WORD) * _countof(g_Indicies);
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        resourceData.pSysMem = g_Indicies;

        hr = global_Render.g_Device_d3d11->CreateBuffer(&indexBufferDesc, &resourceData, &g_d3dIndexBuffer);
        if (FAILED(hr))
        {
            return false;
        }









        // Create the constant buffers for the variables defined in the vertex shader.
        D3D11_BUFFER_DESC constantBufferDesc;
        ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));

        constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        constantBufferDesc.ByteWidth = sizeof(XMMATRIX);
        constantBufferDesc.CPUAccessFlags = 0;
        constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

        hr = global_Render.g_Device_d3d11->CreateBuffer(&constantBufferDesc, nullptr, &g_d3dConstantBuffers[CB_Appliation]);
        if (FAILED(hr))
        {
            return false;
        }
        hr = global_Render.g_Device_d3d11->CreateBuffer(&constantBufferDesc, nullptr, &g_d3dConstantBuffers[CB_Frame]);
        if (FAILED(hr))
        {
            return false;
        }
        hr = global_Render.g_Device_d3d11->CreateBuffer(&constantBufferDesc, nullptr, &g_d3dConstantBuffers[CB_Object]);
        if (FAILED(hr))
        {
            return false;
        }




        // Load the compiled vertex shader.
        LPCWSTR compiledVertexShaderObject = L"VertexShader.cso";

        // Create the input layout for the vertex shader.
        D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex_F3Coord_F3Normal_F2TextCoord, position),     D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex_F3Coord_F3Normal_F2TextCoord, normal),       D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, offsetof(Vertex_F3Coord_F3Normal_F2TextCoord, textureCoord), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        OPTadaE_VertexShaderList_ForResoursManager verEnum = ENUM_VertexShaderList_SimpleMaterial_01;
        global_Render.resourceManager.Create_VertexShader_FromBinaryFile(verEnum, compiledVertexShaderObject, global_Render.g_Device_d3d11, vertexLayoutDesc, _countof(vertexLayoutDesc));


        // Load the compiled pixel shader.
        LPCWSTR compiledPixelShaderObject = L"PixelShader.cso";

        OPTadaE_PixelShaderList_ForResoursManager pixEnum = ENUM_PixelShaderList_SimpleMaterial_01;
        global_Render.resourceManager.Create_PixelShader_FromBinaryFile(pixEnum, compiledPixelShaderObject, global_Render.g_Device_d3d11);

        OPTadaE_MeshList_ForResoursManager meshEnum = ENUM_MeshList_DefaultBox;
        if (!global_Render.resourceManager.Create_Mesh_FromFileToMem(meshEnum, "mesh/share.obj", global_Render.g_Device_d3d11, sizeof(Vertex_F3Coord_F3Normal_F2TextCoord), 0, DXGI_FORMAT_R32_UINT)) {
            MessageBox(NULL, L"Create mesh failed", L"Game level", NULL);
        }

        global_Render.resourceManager.Load_ToGPU_Mesh(meshEnum, global_Render.g_Device_d3d11);




        // Setup the projection matrix.
        RECT clientRect;
        GetClientRect(global_Window.Get_MainWindowHandle(), &clientRect);

        // Compute the exact client dimensions.
        // This is required for a correct projection matrix.
        float clientWidth = static_cast<float>(clientRect.right - clientRect.left);
        float clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);

        g_ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), clientWidth / clientHeight, 0.1f, 100.0f);

        global_Render.g_DeviceContext_d3d11->UpdateSubresource(g_d3dConstantBuffers[CB_Appliation], 0, nullptr, &g_ProjectionMatrix, 0, 0);


        // tested
        global_Render.resourceManager.Use_VertexShader(verEnum, global_Render.g_DeviceContext_d3d11);
        global_Render.resourceManager.Use_PixelShader(pixEnum, global_Render.g_DeviceContext_d3d11);

        return true;

	}

	// use it for free all level's resourses
	void Free()
	{
        SafeRelease(g_d3dConstantBuffers[CB_Appliation]);
        SafeRelease(g_d3dConstantBuffers[CB_Frame]);
        SafeRelease(g_d3dConstantBuffers[CB_Object]);
        SafeRelease(g_d3dIndexBuffer);
        SafeRelease(g_d3dVertexBuffer);
        SafeRelease(g_d3dInputLayout);
        SafeRelease(g_d3dVertexShader);
        SafeRelease(g_d3dPixelShader);
	}

	// Tick function
	// [in] float deltaTime_ // delta time
	bool Tick(float deltaTime_)
	{
        XMVECTOR eyePosition = XMVectorSet(0, 0, -10, 1);
        XMVECTOR focusPoint = XMVectorSet(0, 0, 0, 1);
        XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);
        g_ViewMatrix = XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);
        global_Render.g_DeviceContext_d3d11->UpdateSubresource(g_d3dConstantBuffers[CB_Frame], 0, nullptr, &g_ViewMatrix, 0, 0);


        static float angle = 0.0f;
        angle += 30.0f * deltaTime_;
        XMVECTOR rotationAxis = XMVectorSet(0, 1, 1, 0);

        g_WorldMatrix = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(angle));
        global_Render.g_DeviceContext_d3d11->UpdateSubresource(g_d3dConstantBuffers[CB_Object], 0, nullptr, &g_WorldMatrix, 0, 0);

        return true;
	}


};
