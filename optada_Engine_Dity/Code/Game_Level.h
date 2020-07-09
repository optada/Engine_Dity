// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "Render\OPTada_Render.h"
#include "Input\OPTada_Input.h"

// physic
#ifdef _DEBUG
    #pragma comment (lib, "dqu3e.lib")
#else
    #pragma comment (lib, "qu3e.lib")
#endif
#include "Physics\q3.h" // Physics lib by RandyGaul https://github.com/RandyGaul/qu3e
static q3Scene global_physics_scene(1.0 / 60.0); // physics scene

// sound
#include "Sound\include\soloud.h" // Sound lib by jarikomppa https://github.com/jarikomppa/soloud
#include "Sound\include\soloud_wav.h"

SoLoud::Soloud gSoloud; // SoLoud engine
SoLoud::Wav gWave;      // One wave file


class GameLevel
{
public:

    OPTadaC_Obj_Camera PrimaryCamera;

    std::vector<OPTadaC_Obj_Draw*> draw_opaque; // draw list
    std::vector<OPTadaC_Obj_Draw*> draw_clip;   // draw clip list
    std::vector<OPTadaC_Obj_Draw*> draw_blend;  // draw blend list

    std::vector<OPTadaC_Obj_Light*> draw_light; // draw light objects

    q3Body* fbody; // floor
    q3Body* body; // box

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

        OPTadaE_PixelShaderList_ForResourceManager pixEnum = ENUM_PixelShaderList_PS_Color;
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


        // -------------------------------- setup main camera ------------------------------------

        OPTadaS_Window_Size workspaceSize;
        global_Window.Get_WorkplaceSize(workspaceSize);
        PrimaryCamera.Setup_ProjectionMatrix(workspaceSize.width, workspaceSize.height, 60.0f);

        // update camera
        PrimaryCamera.position.X = 10;
        PrimaryCamera.position.Y = 2;
        PrimaryCamera.position.Z = 0;

        global_Render.resourceManager.UpdateSubresource(ENUM_ConstantBufferList_Application, &cb_ObjectData, global_Render.g_DeviceContext_d3d11);


        // -------------------------------- setup physic -------------------------------------------

        //q3BodyDef bodyDef;
        //q3Body* body = global_physics_scene.CreateBody(bodyDef);

        // Create the floor
        q3BodyDef bodyDef;
        fbody = global_physics_scene.CreateBody(bodyDef);

        q3BoxDef boxDef;
        boxDef.SetRestitution(0);
        q3Transform tx;
        q3Identity(tx);
        boxDef.Set(tx, q3Vec3(50.0f, 1.0f, 50.0f));
        fbody->AddBox(boxDef);


        q3BodyDef bodyDef2;
        bodyDef2.position.Set(0.0f, 3.0f, 0.0f);
        bodyDef2.axis.Set(q3RandomFloat(-1.0f, 1.0f), q3RandomFloat(-1.0f, 1.0f), q3RandomFloat(-1.0f, 1.0f));
        bodyDef2.angle = q3PI * q3RandomFloat(-1.0f, 1.0f);
        bodyDef2.bodyType = eDynamicBody;
        bodyDef2.angularVelocity.Set(q3RandomFloat(1.0f, 3.0f), q3RandomFloat(1.0f, 3.0f), q3RandomFloat(1.0f, 3.0f));
        bodyDef2.angularVelocity *= q3Sign(q3RandomFloat(-1.0f, 1.0f));
        bodyDef2.linearVelocity.Set(q3RandomFloat(1.0f, 3.0f), q3RandomFloat(1.0f, 3.0f), q3RandomFloat(1.0f, 3.0f));
        bodyDef2.linearVelocity *= q3Sign(q3RandomFloat(-1.0f, 1.0f));
        body = global_physics_scene.CreateBody(bodyDef2);

        q3Transform tx2;
        q3Identity(tx2);
        q3BoxDef boxDef2;
        boxDef2.Set(tx2, q3Vec3(1.0f, 1.0f, 1.0f));
        body->AddBox(boxDef2);


        // -------------------------------- setup sound -------------------------------------------

        gSoloud.init(); // Initialize SoLoud
        gWave.load("Sound/ch6_32bit.wav"); // Load a wave
        gSoloud.play(gWave);

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

        static f32 accumulator = 0;
        accumulator += deltaTime_;
        accumulator = q3Clamp01(accumulator);
        if (accumulator >= (1.0f / 60.0f))
        {
            global_physics_scene.Step();
            accumulator = 0;
        }

        
        

        // update camera
        if (PrimaryCamera.position.Z > 6) {
            PrimaryCamera.position.Z = -6;
        }

        //PrimaryCamera.position.X = 5;
        PrimaryCamera.position.X += 0.0 * deltaTime_;
        //PrimaryCamera.position.Z = 0;

        PrimaryCamera.Update_ViewMatrix();


        // update light

        static OPTadaC_Obj_Light light1;
        static OPTadaC_Obj_Light light2;
        static OPTadaC_Obj_Light light3;
        draw_light.clear();
        draw_light.push_back(&light1);
        draw_light.push_back(&light2);
        draw_light.push_back(&light3);

        draw_light[0]->type = ENUM_Obj_Light_Global;
        draw_light[0]->Light.dir     = XMFLOAT3(0.0f, 0.0f, -1.0f); // invert coordinate direction
        draw_light[0]->Light.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
        draw_light[0]->Light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

        draw_light[1]->type = ENUM_Obj_Light_Point;
        draw_light[1]->Light.pos     = XMFLOAT3(0.0f, 5.0f, 3.0f);
        draw_light[1]->Light.range   = 20.0f;
        draw_light[1]->Light.att     = XMFLOAT3(0.0f, 0.1f, 0.0f);
        draw_light[1]->Light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

        draw_light[2]->type = ENUM_Obj_Light_Spotlights;
        draw_light[2]->Light.pos     = XMFLOAT3(0.0f, 5.0f, 3.0f);
        draw_light[2]->Light.dir     = XMFLOAT3(0.0f, -1.0f, 0.0f);
        draw_light[2]->Light.range   = 0.0f;
        draw_light[2]->Light.cone    = 2.0f;
        draw_light[2]->Light.att     = XMFLOAT3(0.1f, 0.1f, 0.0f);
        draw_light[2]->Light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

        global_Render.resourceManager.UpdateSubresource(ENUM_ConstantBufferList_Frame, &cb_FrameData, global_Render.g_DeviceContext_d3d11);
        
        

        // update object
        
        static OPTadaC_Obj_Draw sobject;
        draw_opaque.clear();
        draw_opaque.push_back(&sobject);

        sobject.mesh = ENUM_MeshList_DefaultBox;
        sobject.PS = ENUM_PixelShaderList_PS_Color;
        sobject.VS = ENUM_VertexShaderList_VS_Color;
        sobject.texture_mass = { ENUM_TextureList_TextureForShare };
        

        static OPTadaS_WorldNavigationData test;
        //test.position.Z = 0;
        /*test.position.Z += 1.0f * deltaTime_; 
        test.rotation.Y += 0.0f * deltaTime_;
        test.scaling.X += 0.0f * deltaTime_;
        test.scaling.Y += 0.0f * deltaTime_;*/

        q3Vec3 Bposition;
        q3Transform trans;
        trans = body->GetTransform();

        

        test.position.X = trans.position.x;
        test.position.Y = trans.position.y;
        test.position.Z = trans.position.z;

        XMMATRIX position;
        XMMATRIX rotation;
        XMMATRIX scale;
        test.calcPosition(position);
        test.calcRotation(rotation);
        test.calcScaling(scale);


        rotation._11 = trans.rotation.ex.x;
        rotation._12 = trans.rotation.ex.y;
        rotation._13 = trans.rotation.ex.z;

        rotation._21 = trans.rotation.ey.x;
        rotation._22 = trans.rotation.ey.y;
        rotation._23 = trans.rotation.ey.z;

        rotation._31 = trans.rotation.ez.x;
        rotation._32 = trans.rotation.ez.y;
        rotation._33 = trans.rotation.ez.z;


        test.calcMatrix_SRT(scale, rotation, position);

        sobject.world = &test;




        return true;
	}


};
