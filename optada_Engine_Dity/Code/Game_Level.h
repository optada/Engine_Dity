// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "Render\OPTada_Render.h"
#include "Render\ResourceManager\OPTadaC_ResourceManager.h"


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





class GameLevel
{
public:
	
	// use it for initialize level and level's resourses
	bool Init()
	{
        assert(global_Render.g_Device_d3d11);

        HRESULT hr; 

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
        if (!global_Render.resourceManager.Create_Mesh_FromFileToMem(meshEnum, "mesh/monkey.obj", global_Render.g_Device_d3d11, sizeof(Vertex_F3Coord_F3Normal_F2TextCoord), 0, DXGI_FORMAT_R32_UINT)) {
            MessageBox(NULL, L"Create mesh failed", L"Game level", NULL);
        }
        global_Render.resourceManager.Load_ToGPU_Mesh(meshEnum, global_Render.g_Device_d3d11);

        OPTadaE_TextureList_ForResoursManager textureEnum = ENUM_TextureList_TextureForShare;
        if (!global_Render.resourceManager.Create_Texture_LoadFromFile(textureEnum, L"mesh/DC_monkeyA.bmp", global_Render.g_Device_d3d11)) {
            MessageBox(NULL, L"Create texture failed", L"Game level", NULL);
        }


        // Setup the projection matrix.
        RECT clientRect;
        GetClientRect(global_Window.Get_MainWindowHandle(), &clientRect);

        // Compute the exact client dimensions.
        // This is required for a correct projection matrix.
        float clientWidth = static_cast<float>(clientRect.right - clientRect.left);
        float clientHeight = static_cast<float>(clientRect.bottom - clientRect.top);

        g_ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), clientWidth / clientHeight, 0.1f, 100.0f);

        global_Render.g_DeviceContext_d3d11->UpdateSubresource(g_d3dConstantBuffers[CB_Appliation], 0, nullptr, &g_ProjectionMatrix, 0, 0);



        return true;

	}

	// use it for free all level's resourses
	void Free()
	{
        SafeRelease(g_d3dConstantBuffers[CB_Appliation]);
        SafeRelease(g_d3dConstantBuffers[CB_Frame]);
        SafeRelease(g_d3dConstantBuffers[CB_Object]);
	}

	// Tick function
	// [in] float deltaTime_ // delta time
	bool Tick(float deltaTime_)
	{
        static float posi = 0.0f;
        posi += 0.25f * deltaTime_;
        XMVECTOR eyePosition = XMVectorSet(0, posi, -4, 1); // camera position
        XMVECTOR focusPoint  = XMVectorSet(0, 0, 0, 1); // watch point (look at)
        XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0); // up direction (rotate)
        g_ViewMatrix = XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);
        global_Render.g_DeviceContext_d3d11->UpdateSubresource(g_d3dConstantBuffers[CB_Frame], 0, nullptr, &g_ViewMatrix, 0, 0);


        static float angle = 0.0f;
        angle += 0.0f * deltaTime_;
        XMVECTOR rotationAxis = XMVectorSet(1, 0, 0, 0);

        g_WorldMatrix = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(angle));
        global_Render.g_DeviceContext_d3d11->UpdateSubresource(g_d3dConstantBuffers[CB_Object], 0, nullptr, &g_WorldMatrix, 0, 0);

        return true;
	}


};
