// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "Window\OPTada_Window.h" 
#include "Render\OPTada_Render.h"
#include "Input\OPTada_Input.h"
//#include "Physics\q3.h" // Physics lib by RandyGaul https://github.com/RandyGaul/qu3e  qu3e.lib

#include "Game_Level.h"


GameLevel lvl;


// use this instance for controll all project logic
// create and destroy all global modules here
class OPTada_Instance
{
public:


	// Do all initialization here (init all global modules)
	// [in] HINSTANCE hinstance_ // hinstance_ of process
	// [in] WNDPROC& windowProc_ // event structure
	// return = true - done | false - error
	static bool Global_InitProject(HINSTANCE hinstance_, WNDPROC windowProc_) {

		// create a standard window
		if (!global_Window.InitAndCreateStandartWindow(hinstance_, windowProc_)) {
			MessageBox(NULL, L"main hange error", L"wind", NULL);
			return false;
		}

		// change window size and mode (define)
		OPTadaS_Window_Size newWindowSize;

		// create and init render class
		HWND hwnd = global_Window.Get_MainWindowHandle(); // get handle
		global_Window.Get_WorkplaceSize(newWindowSize); // get workspace size
		if (!global_Render.Initialization(hwnd, OPTADA_CLASSRENDER_DEFAULT_COUNT_OF_BACKBUFFERS, newWindowSize.width, newWindowSize.height, OPTADA_CLASSRENDER_DEFAULT_VSINC, true)) {
			MessageBox(NULL, L"Can't init render", L"Instance", NULL);
			return false;
		}

		// create input
		OPTadaS_Window_Size workspaceWindow;
		global_Window.Get_WorkplaceSize(workspaceWindow);
		if (!global_Input.Initialization(hinstance_, hwnd, workspaceWindow.width, workspaceWindow.height, true, false)) {
			MessageBox(NULL, L"Can't init input", L"Instance", NULL);
			return false;
		}


		return true;
	}

	// Do start setup here (Calling after Global_InitProject)
	// return = true - done | false - error
	static bool Global_SetupProject() {

		// chage window size and mode (define)
		OPTadaS_Window_Size newWindowSize;
		newWindowSize.width = 1280;
		newWindowSize.height = 720;
		if (!Do_Change_WindowSettings(OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_Windowed, newWindowSize, true, 1)) {
			MessageBox(NULL, L"Can't change window size or state", L"Instance", NULL);
			return false;
		}

		// init level params
		lvl.Init();


		return true;
	}

	// shutdown project project (free all global modules)
	static void Global_ShutdownProject() {
		
		// free input
		global_Input.ShuttingDown();;

		// free level data
		lvl.Free();

		// close DirectX
		global_Render.ShuttingDown();

	}


	// -----------------------------------------------------------------------------------------------------------------------------


	// Global tick
	static int Tick(float deltaTime_) {

		// input
		POINT mouseCoordinate;
		POINT mouseDelta;
		unsigned char* kb_Mass = nullptr;
		unsigned char* m_Mass = nullptr;
		global_Input.Update();
		global_Input.Get_Input_8Mouse_256Keyboard(mouseCoordinate, mouseDelta, &m_Mass, &kb_Mass);

		if (_B_PRESSED(kb_Mass[DIK_F])) {
			//lvl.body->ApplyLinearForce(q3Vec3(0.0f, 0.0f, -10.0f));
			lvl.body->ApplyTorque(q3Vec3(0.0f, 0.0f, -10.0f));
		}


		if (_B_DOWN(kb_Mass[DIK_F])) {
		
		}
		if (_B_UP(kb_Mass[DIK_F])) {

		}
		if (_B_DOWN(kb_Mass[DIK_A])) {

		}
		if (_B_UP(kb_Mass[DIK_A])) {

		}
		if (_B_DOWN(kb_Mass[DIK_D])) {

		}
		if (_B_UP(kb_Mass[DIK_D])) {

		}

		// prepare render
		global_Render.PrepareBuffersForNewFrame(DirectX::Colors::CornflowerBlue, 1.0f, 0);

		// game tick
		lvl.Tick(deltaTime_); //TODO add input data

		// draw
		DrawScene(deltaTime_);

		//lvl.DrawText();

		global_Render.PresentFrame();

		return 0;
	}

	// All draw logic here
	static int DrawScene(float deltaTime_) {

		float zeroBlend[] = { 0.0f, 0.0f, 0.0f, 0.0f }; // empty mass for zero blend

		// ----------- update and prepeir all resources

		OPTadaC_Obj_Draw* lastDrawObject = nullptr; // for cheking need to change resources
		OPTadaC_Obj_Draw* current_opaque = nullptr;
		int opaque_Count = 0;

		std::vector<OPTadaC_Obj_Draw*> d_opaque; // draw list

		//std::vector<OPTadaC_Obj_Draw*> d_clip;   // draw clip list
		//std::vector<OPTadaC_Obj_Draw*> d_blend;  // draw blend list

		int darwObjCount = 0;


		// -- sort and copy opaque objects

		// generate has for objects for opaque
		darwObjCount = lvl.draw_opaque.size();
		for (int i = 0; i < darwObjCount; i++) {
			OPTadaC_Obj_Draw* drawObj = lvl.draw_opaque[i];
			short int newHesh[4] = { drawObj->PS, drawObj->VS, drawObj->mesh, 0 }; // 2b 2b 2b 2b (double 8b)
			drawObj->hash = (double&)newHesh;
			d_opaque.push_back(drawObj); // copy form level.draw_opaque to d_opaque
		}
		darwObjCount -= 1; // minus last
		for (int i = 0; i < darwObjCount; i++) {
			OPTadaC_Obj_Draw* drawObj = d_opaque[i];
			OPTadaC_Obj_Draw* drawObjNext = d_opaque[i + 1];
			if (drawObj->hash > drawObjNext->hash) { // if [i] > [i+1] -> switch
				OPTadaC_Obj_Draw* drawObjSave = drawObjNext;
				d_opaque[i + 1] = drawObj;
				d_opaque[i] = drawObjSave;
			}
		}
		


		// -- sort clip objects


		// -- sort blend objects


		// -- sort and set light

		cb_FrameData.light_param = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f); // zero count
		int lightCount = lvl.draw_light.size();
		for (int i = 0; i < lightCount; i++) { // calculate light count
			OPTadaC_Obj_Light* pLight = lvl.draw_light[i]; // get light
			if (pLight->type == ENUM_Obj_Light_Global) { // this is global light
				cb_FrameData.light_param.x++;
			}
			if (pLight->type == ENUM_Obj_Light_Point) { // this is point light
				cb_FrameData.light_param.y++;
			}
			if (pLight->type == ENUM_Obj_Light_Spotlights) { // this is projector (spotLight)
				cb_FrameData.light_param.z++;
			}
		}
		// setup cb_data
		int i2 = 0; // counter for cb_buffer
		for (int i = 0; i < lightCount; i++) { // set global
			OPTadaC_Obj_Light* pLight = lvl.draw_light[i]; // get light
			if (pLight->type == ENUM_Obj_Light_Global) { // this is global light
				cb_FrameData.light_Mass[i2] = pLight->Light;
				i2++;
			}
		}
		for (int i = 0; i < lightCount; i++) { // set global
			OPTadaC_Obj_Light* pLight = lvl.draw_light[i]; // get light
			if (pLight->type == ENUM_Obj_Light_Point) { // this is global light
				cb_FrameData.light_Mass[i2] = pLight->Light;
				i2++;
			}
		}
		for (int i = 0; i < lightCount; i++) { // set global
			OPTadaC_Obj_Light* pLight = lvl.draw_light[i]; // get light
			if (pLight->type == ENUM_Obj_Light_Spotlights) { // this is global light
				cb_FrameData.light_Mass[i2] = pLight->Light;
				i2++;
			}
		}

		// setup viev matrix for shadow camera

		//global_Render.g_shadow.camera.Update_ViewMatrix();
		//cb_FrameData.wvplight =  global_Render.g_shadow.camera.view_Matrix * global_Render.g_shadow.camera.projection_Matrix; // save shadow wvp

		// -- set cb_Frame

		global_Render.resourceManager.UpdateSubresource(ENUM_ConstantBufferList_Frame, &cb_FrameData, global_Render.g_DeviceContext_d3d11); 


		// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------

		// ----------- set default parameters


		global_Render.Setup_NewRasterizer(ENUM_RasterizerMass_DrawFront_ClipBack); // he he classic
		global_Render.Setup_NewBlendState(ENUM_BlendStateMass_NONE, zeroBlend);    //Set the default blend state (no blending) for opaque objects

		global_Render.g_DeviceContext_d3d11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // draw triangles	

		//global_Render.g_DeviceContext_d3d11->RSSetViewports(1, &global_Render.g_Viewport); // viewport

		//global_Render.g_DeviceContext_d3d11->OMSetRenderTargets(1, &global_Render.g_RenderTargetView_d3d, global_Render.g_DepthStencilView_d3d); // set DepthStencil buffer
		//global_Render.g_DeviceContext_d3d11->OMSetDepthStencilState(global_Render.g_DepthStencilState_d3d, 1);                                   // set depth stencil parameters


		// ----------- create shadow data


		//global_Render.g_shadow.SetRenderTarget(global_Render.g_DeviceContext_d3d11);
		//global_Render.g_shadow.ClearRenderTarget(global_Render.g_DeviceContext_d3d11, 0.0f, 0.0f, 0.0f, 1.0f);
		//
		//// Set shaders
		//global_Render.resourceManager.Use_VertexShader(ENUM_VertexShaderList_VS_Shadow, global_Render.g_DeviceContext_d3d11);
		//global_Render.resourceManager.Use_PixelShader(ENUM_PixelShaderList_PS_Shadow, global_Render.g_DeviceContext_d3d11);

		//OPTadaC_Obj_Draw* current_opaque = nullptr;
		//int opaque_Count = d_opaque.size();
		//for (int i = 0; i < opaque_Count; i++) {
		//	current_opaque = d_opaque[i];

		//	if (i == 0) { // if first cycle -> just set all			

		//		// Set Mesh
		//		global_Render.resourceManager.Use_Mesh_WithIndexBuffer(current_opaque->mesh, global_Render.g_DeviceContext_d3d11);
		//		OPTadaS_MeshStructure* mesh = global_Render.resourceManager.Get_MeshCell_IfInGPU(current_opaque->mesh);

		//		// -- set cb_Object
		//		cb_ObjectData.WVP = current_opaque->world->matrix * lvl.PrimaryCamera.view_Matrix * lvl.PrimaryCamera.projection_Matrix;
		//		cb_ObjectData.World = current_opaque->world->matrix;
		//		global_Render.resourceManager.UpdateSubresource(ENUM_ConstantBufferList_ObjectData, &cb_ObjectData, global_Render.g_DeviceContext_d3d11);

		//		global_Render.g_DeviceContext_d3d11->DrawIndexed(mesh->indexBufferCount, 0, 0); // draw

		//		lastDrawObject = current_opaque;
		//	}
		//	else { // we have 

		//		// Set Mesh
		//		if (lastDrawObject->mesh != current_opaque->mesh) {
		//			global_Render.resourceManager.Use_Mesh_WithIndexBuffer(current_opaque->mesh, global_Render.g_DeviceContext_d3d11);
		//		}
		//		OPTadaS_MeshStructure* mesh = global_Render.resourceManager.Get_MeshCell_IfInGPU(current_opaque->mesh);

		//		// -- set cb_Object
		//		if (lastDrawObject->world != current_opaque->world) {
		//			cb_ObjectData.WVP = current_opaque->world->matrix * lvl.PrimaryCamera.view_Matrix * lvl.PrimaryCamera.projection_Matrix;
		//			cb_ObjectData.World = current_opaque->world->matrix;
		//			global_Render.resourceManager.UpdateSubresource(ENUM_ConstantBufferList_ObjectData, &cb_ObjectData, global_Render.g_DeviceContext_d3d11);
		//		}

		//		global_Render.g_DeviceContext_d3d11->DrawIndexed(mesh->indexBufferCount, 0, 0); // draw

		//		lastDrawObject = current_opaque;
		//	}
		//}


		// ----------- set default parameters again XD


		global_Render.g_DeviceContext_d3d11->RSSetViewports(1, &global_Render.g_Viewport); // viewport

		global_Render.g_DeviceContext_d3d11->OMSetRenderTargets(1, &global_Render.g_RenderTargetView_d3d, global_Render.g_DepthStencilView_d3d); // set DepthStencil buffer
		global_Render.g_DeviceContext_d3d11->OMSetDepthStencilState(global_Render.g_DepthStencilState_d3d, 1);                                   // set depth stencil parameters


		// ----------- draw opaque objects

		global_Render.g_DeviceContext_d3d11->PSSetShaderResources(3, 1, &global_Render.g_shadow.sh_SRV); // set shadow texture --> texture3 (3t)

		current_opaque = nullptr;
		opaque_Count = d_opaque.size();
		for (int i = 0; i < opaque_Count; i++) {
			current_opaque = d_opaque[i];

			if (i == 0) { // if first cycle -> just set all
				// Set shaders
				global_Render.resourceManager.Use_VertexShader(current_opaque->VS, global_Render.g_DeviceContext_d3d11);
				global_Render.resourceManager.Use_PixelShader(current_opaque->PS, global_Render.g_DeviceContext_d3d11);

				// Set Mesh
				global_Render.resourceManager.Use_Mesh_WithIndexBuffer(current_opaque->mesh, global_Render.g_DeviceContext_d3d11);
				OPTadaS_MeshStructure* mesh = global_Render.resourceManager.Get_MeshCell_IfInGPU(current_opaque->mesh);

				// Set textures
				for (int texNom = 0, texCount = current_opaque->texture_mass.size(); texNom < texCount; texNom++) {
					global_Render.resourceManager.Use_Texture(current_opaque->texture_mass[texNom], global_Render.g_DeviceContext_d3d11, texNom);
				}		

				// -- set cb_Object
				cb_ObjectData.WVP = current_opaque->world->matrix * lvl.PrimaryCamera.view_Matrix * lvl.PrimaryCamera.projection_Matrix;
				cb_ObjectData.World = current_opaque->world->matrix;
				global_Render.resourceManager.UpdateSubresource(ENUM_ConstantBufferList_ObjectData, &cb_ObjectData, global_Render.g_DeviceContext_d3d11);

				global_Render.g_DeviceContext_d3d11->DrawIndexed(mesh->indexBufferCount, 0, 0); // draw

				lastDrawObject = current_opaque;
			}
			else { // we have 
				// Set shaders
				if (lastDrawObject->VS != current_opaque->VS) {
					global_Render.resourceManager.Use_VertexShader(current_opaque->VS, global_Render.g_DeviceContext_d3d11);
				}
				if (lastDrawObject->PS != current_opaque->PS) {
					global_Render.resourceManager.Use_PixelShader(current_opaque->PS, global_Render.g_DeviceContext_d3d11);
				}

				// Set Mesh
				if (lastDrawObject->mesh != current_opaque->mesh) {
					global_Render.resourceManager.Use_Mesh_WithIndexBuffer(current_opaque->mesh, global_Render.g_DeviceContext_d3d11);
				}
				OPTadaS_MeshStructure* mesh = global_Render.resourceManager.Get_MeshCell_IfInGPU(current_opaque->mesh);

				// Set textures
				for (int texNom = 0, texCount = current_opaque->texture_mass.size(); texNom < texCount; texNom++) {
					global_Render.resourceManager.Use_Texture(current_opaque->texture_mass[texNom], global_Render.g_DeviceContext_d3d11, texNom);
				}

				// -- set cb_Object
				if (lastDrawObject->world != current_opaque->world) {					
					cb_ObjectData.WVP = current_opaque->world->matrix * lvl.PrimaryCamera.view_Matrix * lvl.PrimaryCamera.projection_Matrix;
					cb_ObjectData.World = current_opaque->world->matrix;
					global_Render.resourceManager.UpdateSubresource(ENUM_ConstantBufferList_ObjectData, &cb_ObjectData, global_Render.g_DeviceContext_d3d11);
				}

				global_Render.g_DeviceContext_d3d11->DrawIndexed(mesh->indexBufferCount, 0, 0); // draw

				lastDrawObject = current_opaque;
			}
		}


		// ----------- draw clip objects


		//global_Render.Setup_NewRasterizer(ENUM_RasterizerMass_DrawAll);

		//global_Render.resourceManager.Use_VertexShader(ENUM_VertexShaderList_VS_Color, global_Render.g_DeviceContext_d3d11);
		//global_Render.resourceManager.Use_PixelShader(ENUM_PixelShaderList_SimpleMaterial_01, global_Render.g_DeviceContext_d3d11);

		//// Set Mesh
		//global_Render.resourceManager.Use_Mesh_WithIndexBuffer(ENUM_MeshList_DefaultBox, global_Render.g_DeviceContext_d3d11);
		//OPTadaS_MeshStructure* mesh = global_Render.resourceManager.Get_MeshCell_IfInGPU(ENUM_MeshList_DefaultBox);

		//// Set texture
		//global_Render.resourceManager.Use_Texture(ENUM_TextureList_TextureForShare, global_Render.g_DeviceContext_d3d11, 0);

		//global_Render.g_DeviceContext_d3d11->DrawIndexed(/*_countof(g_Indicies)*/mesh->indexBufferCount, 0, 0);


		// ----------- draw blend objects
		

		//float blendFactor[] = { 0.75f, 0.75f, 0.75f, 1.0f }; //"fine-tune" the blending equation

		//global_Render.Setup_NewBlendState(ENUM_BlendStateMass_Transparent, blendFactor); // set blend state


		//// Set shaders
		//global_Render.resourceManager.Use_VertexShader(ENUM_VertexShaderList_SimpleMaterial_01, global_Render.g_DeviceContext_d3d11);
		//global_Render.resourceManager.Use_PixelShader(ENUM_PixelShaderList_SimpleMaterial_01, global_Render.g_DeviceContext_d3d11);

		//// Set Mesh
		//global_Render.resourceManager.Use_Mesh_WithIndexBuffer(ENUM_MeshList_DefaultBox, global_Render.g_DeviceContext_d3d11);
		//OPTadaS_MeshStructure* mesh = global_Render.resourceManager.Get_MeshCell_IfInGPU(ENUM_MeshList_DefaultBox);

		//// Set texture
		//global_Render.resourceManager.Use_Texture(ENUM_TextureList_TextureForShare, global_Render.g_DeviceContext_d3d11, 0);

		//global_Render.g_DeviceContext_d3d11->DrawIndexed(/*_countof(g_Indicies)*/mesh->indexBufferCount, 0, 0);


		// --------------------------------------------------------------------------------------------------------


		//global_Render.g_DeviceContext_d3d11->Draw(22, 0);
		//global_Render.g_DeviceContext_d3d11->DrawIndexed(/*_countof(g_Indicies)*/mesh->indexBufferCount, 0, 0);

		return 1;
	}


	// -----------------------------------------------------------------------------------------------------------------------------


	// call this when you need to change window size and state
	// [in] OPTadaE_WindowState_ForClassWindow newWindowState_ // new window state
	// [in] OPTadaS_Window_Size& newWindowSize_                // new window size
	// [in] bool vSinc_                                        // true - enable vSinc | false - disable vSinc
	// [in] int countOfBackBuffers_                            // count of back buffers (1 - double bufferization) (2 - tripple bufferization)
	// return = true - done | false - error
	static bool Do_Change_WindowSettings(OPTadaE_WindowState_ForClassWindow newWindowState_, OPTadaS_Window_Size& newWindowSize_, bool vSinc_, int countOfBackBuffers_) {

		if (!global_Window.Change_DisplayOfWindow(newWindowState_, newWindowSize_)) {
			return false;
		};

		global_Render.Setup_FullScreenMode(newWindowState_ == OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_FullScreen);

		if (!global_Render.Setup_NewSettingsForRender(newWindowSize_.width, newWindowSize_.height, vSinc_, countOfBackBuffers_)) {
			return false;
		}

		// update window size for input cursor
		OPTadaS_Window_Size workspaceWindow;
		global_Window.Get_WorkplaceSize(workspaceWindow);
		global_Input.Set_Workspace(workspaceWindow.width, workspaceWindow.height);

		return true;
	}


	// call this when you window is loosing focus
	static void Do_Reaction_LooseFocus() {
		global_Render.Setup_FullScreenMode(false);
		global_Window.Do_LooseFocusInFullscreenMode();
		global_Input.Set_HaveFocus(false);
	}

	// call this when your window is taking focus
	static void Do_Reaction_TakeFocus() {
		global_Window.Do_RestoreFocusInFullscreenMode();
		OPTadaE_WindowState_ForClassWindow newWindowState_;
		global_Window.Get_WindowState(newWindowState_);
		global_Render.Setup_FullScreenMode(newWindowState_ == OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_FullScreen);
		SetFocus(global_Window.Get_MainWindowHandle());
		global_Input.Set_HaveFocus(true);
	}


	// call this for (ALT+TAB) reaction (hide window in fullscreen)
	static void Do_Reaction_AltTab() {
		global_Render.Setup_FullScreenMode(false);
		global_Window.Do_AltTabLooseFocusInFullscreenMode();
	}

	// call this for (ALT+ENTER) reaction (Swap window mode fullscreen|Windowed)
	static void Do_Reaction_AltEnter() {
		OPTadaE_WindowState_ForClassWindow windowState_;
		OPTadaS_Window_Size worspaceSize;
		global_Window.Get_WorkplaceSize(worspaceSize);
		global_Window.Get_WindowState(windowState_);
		Do_Change_WindowSettings((windowState_ == ENUM_WindowState_Windowed) ? (ENUM_WindowState_WindowedWithNoBurders) : (ENUM_WindowState_Windowed), worspaceSize, true, 1);

		//	if (windowState_ != OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_FullScreen) { // set fullscreen
		//		global_Window.Do_SwapMode_Fullscreen_LastWindowed();
		//		global_Render.Setup_FullScreenMode(windowState_ == OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_FullScreen);
		//		//SetFocus(global_Window.Get_MainWindowHandle());
		//	}
		//	else { // set windowed
		//		global_Render.Setup_FullScreenMode(windowState_ == OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_FullScreen);
		//		global_Window.Do_SwapMode_Fullscreen_LastWindowed();
		//		global_Window.Update_WindowSizeWithBorders();
		//		//SetFocus(global_Window.Get_MainWindowHandle());
		//	}
	}

};


