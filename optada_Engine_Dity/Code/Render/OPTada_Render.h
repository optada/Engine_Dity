// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

// DirectX includes
#include <dxgi.h> 
#include <d3dcommon.h> 

#include <d3dx11.h>
#include <d3d11.h>
#include <d3dx10.h>

#include <dinput.h>

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <xnamath.h>

#include <d3dcompiler.h>


// macros for building 32 - bit color A.8.8.8 format(8 - bit alpha mode)
#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))
//#define _RGB24BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16))


// Safely release a COM object.
template<typename T>
inline void SafeRelease(T& ptr) 
{
	if (ptr != nullptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}


// Renger
class OPTada_Render
{
private:

	ID3D11Device*           g_Device_d3d11        = nullptr; // device
	ID3D11DeviceContext*    g_DeviceContext_d3d11 = nullptr; // device context
	IDXGISwapChain*         g_SwapChain           = nullptr; // swap chain

	ID3D11RenderTargetView* g_RenderTargetView_d3d   = nullptr; // Render target view for the back buffer of the swap chain.	
	ID3D11DepthStencilView* g_DepthStencilView_d3d   = nullptr; // Depth/stencil view for use as a depth buffer.
	ID3D11Texture2D*        g_DepthStencilBuffer_d3d = nullptr; // A texture to associate to the depth stencil view.

	ID3D11DepthStencilState* g_DepthStencilState_d3d = nullptr; // Define the functionality of the depth/stencil stages.
	ID3D11RasterizerState*   g_RasterizerState_d3d   = nullptr; // Define the functionality of the rasterizer stage.

	D3D11_VIEWPORT g_Viewport = { 0 };

public:

	// initialithation of render (DirectX)
	// [in] HWND hwnd_              // descriptor of window
	// [in] int countOfBackBuffers_ // count of back buffers (1 - double bufferization) (2 - tripple bufferization)
	// [in] int workspaceWidth_     // workspace Width
	// [in] int workspaceHeight_    // workspace Height
	// [in] bool vSinc_             // true - enable | false - disable
	// [in] bool isWindowedMode_    // true - windowed mode | false - fullscreen mode
	// return = true - done | false - error
	bool Initialization(HWND hwnd_, int countOfBackBuffers_, int workspaceWidth_, int workspaceHeight_, bool vSinc_, bool isWindowedMode_);

	// Use this for free all resourses
	void ShuttingDown();

	// 
	//bool Setup_NewSettingsFor();



	bool testedDraw() {

		//Clear our backbuffer to the updated color
		D3DXCOLOR bgColor(1.0f, 0.5f, 0.5f, 1.0f);

		g_DeviceContext_d3d11->ClearRenderTargetView(g_RenderTargetView_d3d, bgColor);

		//Present the backbuffer to the screen
		g_SwapChain->Present(0, 0);

		return true;
	}

};


// --------------------------------------------------------------------------------------------


// Created class OPTada_Render for render
extern OPTada_Render global_Render;