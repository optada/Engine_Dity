// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "Render\OPTada_Render.h"
#include "Render\ResourceManager\OPTadaC_ResourceManager.h"


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

        // Load the compiled vertex shader.
        LPCWSTR compiledVertexShaderObject = L"VS_Color.cso";

        // Create the input layout for the vertex shader.
        D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex_F3Coord_F3Normal_F2TextCoord, position),     D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex_F3Coord_F3Normal_F2TextCoord, normal),       D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, offsetof(Vertex_F3Coord_F3Normal_F2TextCoord, textureCoord), D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        OPTadaE_VertexShaderList_ForResourceManager verEnum = ENUM_VertexShaderList_VS_Color;
        global_Render.resourceManager.Create_VertexShader_FromBinaryFile(verEnum, compiledVertexShaderObject, global_Render.g_Device_d3d11, vertexLayoutDesc, _countof(vertexLayoutDesc));


        // Load the compiled pixel shader.
        LPCWSTR compiledPixelShaderObject = L"PS_Color.cso";

        OPTadaE_PixelShaderList_ForResourceManager pixEnum = ENUM_PixelShaderList_SimpleMaterial_01;
        global_Render.resourceManager.Create_PixelShader_FromBinaryFile(pixEnum, compiledPixelShaderObject, global_Render.g_Device_d3d11, { ENUM_SamplerStateList_Linear_16 } );

        OPTadaE_MeshList_ForResourceManager meshEnum = ENUM_MeshList_DefaultBox;
        if (!global_Render.resourceManager.Create_Mesh_FromFileToMem(meshEnum, "mesh/monkey.obj", global_Render.g_Device_d3d11, sizeof(Vertex_F3Coord_F3Normal_F2TextCoord), 0, DXGI_FORMAT_R32_UINT)) {
            MessageBox(NULL, L"Create mesh failed", L"Game level", NULL);
        }
        global_Render.resourceManager.Load_ToGPU_Mesh(meshEnum, global_Render.g_Device_d3d11);

        OPTadaE_TextureList_ForResourceManager textureEnum = ENUM_TextureList_TextureForShare;
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

        global_Render.resourceManager.UpdateSubresource(ENUM_ConstantBufferList_Application, &g_ProjectionMatrix, global_Render.g_DeviceContext_d3d11);

        return true;

	}

	// use it for free all level's resourses
	void Free()
	{

	}

	// Tick function
	// [in] float deltaTime_ // delta time
	bool Tick(float deltaTime_)
	{
        static float posi = 0.0f;
        posi += 0.0f * deltaTime_;
        XMVECTOR eyePosition = XMVectorSet(0, posi, -4, 1); // camera position
        XMVECTOR focusPoint  = XMVectorSet(0, 0, 0, 1); // watch point (look at)
        XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0); // up direction (rotate)
        g_ViewMatrix = XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);
        global_Render.resourceManager.UpdateSubresource(ENUM_ConstantBufferList_Frame, &g_ViewMatrix, global_Render.g_DeviceContext_d3d11);


        static float angle = 0.0f;
        angle += 0.1f * deltaTime_;
        XMVECTOR rotationAxis = XMVectorSet(1, 0, 0, 0);

        g_WorldMatrix = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(0.0f));
        g_WorldMatrix = XMMatrixTranslation(0.0f, 0.0f, angle);
        global_Render.resourceManager.UpdateSubresource(ENUM_ConstantBufferList_ObjectData, &g_WorldMatrix, global_Render.g_DeviceContext_d3d11);

        return true;
	}


};
