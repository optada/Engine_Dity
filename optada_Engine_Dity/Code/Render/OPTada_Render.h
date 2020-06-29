// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "OPTada_Render_Settings.h"

#include <string>

// DirectX includes
#include <dxgi.h> 
#include <d3dcommon.h> 

#include <d3dx11.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3d10.h>

#include <dinput.h>

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <xnamath.h>

#include <d3dcompiler.h>


// macros for building 32 - bit color A.8.8.8 format(8 - bit alpha mode)
#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))
// macros for building 24 - bit color 8.8.8 
#define _RGB24BIT(r,g,b)   ((b) + ((g) << 8) + ((r) << 16))


// Safely release a COM object.
template<typename T>
inline void SafeRelease(T& ptr) 
{
	if (ptr != nullptr) {
		ptr->Release();
		ptr = nullptr;
	}
}


// Renger
class OPTada_Render
{
public:

	bool vSinc = false; // vertycal sincronithzation

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

	// and create support render resourses (depth buffer, viewPort, rasterither....)
	// [in] int workspaceWidth_       // workspace Width
	// [in] int workspaceHeight_      // workspace Height
	// [in] D3D11_FILL_MODE fillMode_ // D3D11_FILL_SOLID - draw triangles formed // D3D11_FILL_WIREFRAME - darw lines
	// return = true - done | false - error
	bool InitializeSecondaryResources(int workspaceWidth_, int workspaceHeight_, D3D11_FILL_MODE fillMode_);

	// Use this for free all resourses
	void ShuttingDown();



	// setup new settings for render
	// [in] int workspaceWidth_       // workspace Width
	// [in] int workspaceHeight_      // workspace Height
	// [in] bool vSinc_               // true - enable | false - disable
	// [in] int countOfBackBuffers_   // count of back buffers (1 - double bufferization) (2 - tripple bufferization)
	// [in] D3D11_FILL_MODE fillMode_ // D3D11_FILL_SOLID - draw triangles formed // D3D11_FILL_WIREFRAME - darw lines
	// return = true - done | false - error
	bool Setup_NewSettingsForRender(int workspaceWidth_, int workspaceHeight_, bool vSinc_, int countOfBackBuffers_, D3D11_FILL_MODE fillMode_);

	// setup fullscreen or windowed mode for render
	void Setup_FullScreenMode(bool isFullScreen_);


	// Get the latest profile for the specified shader type.
	// return = string - with lastest profile name for Pixel|Vertex shader
	template< class ShaderClass >
	std::string GetLatestShaderProfile();


	// Create Pixel shader from a binary file
	// [in] const std::wstring& fileName_  // filename
	// return = ID3D11PixelShader* - if done | nullptr - if failed
	ID3D11PixelShader* CreatePixelShaderFrom_BinaryFile(const std::wstring& fileName_);

	// Create Vertex shader from a binary file
	// [in] const std::wstring& fileName_               // filename
	// [in] D3D11_INPUT_ELEMENT_DESC* vertexLayoutDesc_ // link on Directx structure
	// [in] UINT countOfvertexLayoutDesc_,              // count of vertexLayoutDesc use _countof(vertexLayoutDesc_)
	// [out] ID3D11InputLayout** inputLayout_ // item on item for saving input layout of vertex shader
	// return = ID3D11VertexShader* - if done | nullptr - if failed
	ID3D11VertexShader* CreateVertexShaderFrom_BinaryFile(const std::wstring& fileName_, D3D11_INPUT_ELEMENT_DESC* vertexLayoutDesc_, UINT countOfvertexLayoutDesc_, ID3D11InputLayout** inputLayout_);

	// Create Vertex|Pixel shader from a binary object and the class linkage
	// [in] ID3DBlob* pShaderBlob_             // a binary object
	// [in] ID3D11ClassLinkage* pClassLinkage_ // a class linkage
	// return = ShaderClass - if done | nullptr - if failed
	template< class ShaderClass >
	ShaderClass* CreateShaderFrom_BinaryObject(ID3DBlob* pShaderBlob_, ID3D11ClassLinkage* pClassLinkage_);

	// create and compile shader from file 
	// [in] const std::wstring& fileName_  // filename
	// [in] const std::string& entryPoint_ // enteryPoint (function)
	// [in] const std::string& profile_    // shader profile
	// return = link on shader - done | false - error
	template< class ShaderClass >
	ShaderClass* LoadAndCompileShaderFrom_File(const std::wstring& fileName_, const std::string& entryPoint_, const std::string& profile_);


	// Clear the color and depth buffers.
	void PrepareBuffersForNewFrame(const FLOAT clearColor[4], FLOAT clearDepth, UINT8 clearStencil);

	// Present frame
	void PresentFrame();


	/*
	bool testedDraw() {

		//Clear our backbuffer to the updated color
		D3DXCOLOR bgColor(1.0f, 1.0f, 0.0f, 1.0f);

		g_DeviceContext_d3d11->ClearRenderTargetView(g_RenderTargetView_d3d, bgColor);

		//Present the backbuffer to the screen
		g_SwapChain->Present(0, 0);

		return true;
	}
	*/

};


// --------------------------------------------------------------------------------------------


// Created class OPTada_Render for render
extern OPTada_Render global_Render;
