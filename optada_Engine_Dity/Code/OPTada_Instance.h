// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "Window\OPTada_Window.h" 
#include "Render\OPTada_Render.h"

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

		return true;
	}


	// Do start setup here (Calling after Global_InitProject)
	// return = true - done | false - error
	static bool Global_SetupProject() {

		// chage window size and mode (define)
		OPTadaS_Window_Size newWindowSize;
		newWindowSize.width = 1280;
		newWindowSize.height = 720;
		if (!Do_Change_WindowSettings(OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_Windowed, newWindowSize, true, 1, D3D11_FILL_SOLID)) {
			MessageBox(NULL, L"Can't change window size or state", L"Instance", NULL);
			return false;
		}


		lvl.Init();


		return true;
	}

	// shutdown project project (free all global modules)
	static void Global_ShutdownProject() {
		
		lvl.Free();

		// close DirectX
		global_Render.ShuttingDown();

	}


	// -----------------------------------------------------------------------------------------------------------------------------


	// Global tick
	static int Tick(float deltaTime_) {

		global_Render.PrepareBuffersForNewFrame(DirectX::Colors::CornflowerBlue, 1.0f, 0);

		lvl.Tick(deltaTime_);

		DrawScene(deltaTime_);

		global_Render.PresentFrame();

		return 0;
	}

	// All draw logic here
	static int DrawScene(float deltaTime_) {

		const UINT vertexStride = sizeof(VertexPosColor);
		const UINT offset = 0;

		global_Render.g_DeviceContext_d3d11->IASetVertexBuffers(0, 1, &g_d3dVertexBuffer, &vertexStride, &offset);
		global_Render.g_DeviceContext_d3d11->IASetInputLayout(g_d3dInputLayout);
		global_Render.g_DeviceContext_d3d11->IASetIndexBuffer(g_d3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		global_Render.g_DeviceContext_d3d11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		global_Render.g_DeviceContext_d3d11->VSSetShader(g_d3dVertexShader, nullptr, 0);
		global_Render.g_DeviceContext_d3d11->VSSetConstantBuffers(0, 3, g_d3dConstantBuffers);

		global_Render.g_DeviceContext_d3d11->RSSetState(global_Render.g_RasterizerState_d3d);
		global_Render.g_DeviceContext_d3d11->RSSetViewports(1, &global_Render.g_Viewport);

		global_Render.g_DeviceContext_d3d11->PSSetShader(g_d3dPixelShader, nullptr, 0);

		global_Render.g_DeviceContext_d3d11->OMSetRenderTargets(1, &global_Render.g_RenderTargetView_d3d, global_Render.g_DepthStencilView_d3d);
		global_Render.g_DeviceContext_d3d11->OMSetDepthStencilState(global_Render.g_DepthStencilState_d3d, 1);

		global_Render.g_DeviceContext_d3d11->DrawIndexed(_countof(g_Indicies), 0, 0);

		return 1;
	}


	// -----------------------------------------------------------------------------------------------------------------------------


	// call this when you need to change window size and state
	// [in] OPTadaE_WindowState_ForClassWindow newWindowState_ // new window state
	// [in] OPTadaS_Window_Size& newWindowSize_                // new window size
	// [in] bool vSinc_                                        // true - enable vSinc | false - disable vSinc
	// [in] int countOfBackBuffers_                            // count of back buffers (1 - double bufferization) (2 - tripple bufferization)
	// [in] D3D11_FILL_MODE fillMode_                          // D3D11_FILL_SOLID - draw triangles formed // D3D11_FILL_WIREFRAME - darw lines
	// return = true - done | false - error
	static bool Do_Change_WindowSettings(OPTadaE_WindowState_ForClassWindow newWindowState_, OPTadaS_Window_Size& newWindowSize_, bool vSinc_, int countOfBackBuffers_, D3D11_FILL_MODE fillMode_) {

		if (!global_Window.Change_DisplayOfWindow(newWindowState_, newWindowSize_)) {
			return false;
		};

		global_Render.Setup_FullScreenMode(newWindowState_ == OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_FullScreen);

		if (!global_Render.Setup_NewSettingsForRender(newWindowSize_.width, newWindowSize_.height, vSinc_, countOfBackBuffers_, fillMode_)) {
			return false;
		}

		return true;
	}


	// call this when you window is loosing focus
	static void Do_Reaction_LooseFocus() {
		global_Render.Setup_FullScreenMode(false);
		global_Window.Do_LooseFocusInFullscreenMode();
	}

	// call this when your window is taking focus
	static void Do_Reaction_TakeFocus() {
		global_Window.Do_RestoreFocusInFullscreenMode();
		OPTadaE_WindowState_ForClassWindow newWindowState_;
		global_Window.Get_WindowState(newWindowState_);
		global_Render.Setup_FullScreenMode(newWindowState_ == OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_FullScreen);
	}


	// call this for (ALT+TAB) reaction (hide window in fullscreen)
	static void Do_Reaction_AltTab() {
		global_Render.Setup_FullScreenMode(false);
		global_Window.Do_AltTabLooseFocusInFullscreenMode();
	}

	// call this for (ALT+ENTER) reaction (Swap window mode fullscreen|Windowed)
	static void Do_Reaction_AltEnter() {
		global_Window.Do_SwapMode_Fullscreen_LastWindowed();

		OPTadaE_WindowState_ForClassWindow newWindowState_;
		global_Window.Get_WindowState(newWindowState_);
		global_Render.Setup_FullScreenMode(newWindowState_ == OPTadaE_WindowState_ForClassWindow::ENUM_WindowState_FullScreen);
	}

};


