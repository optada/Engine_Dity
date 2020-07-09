// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <strstream>

// DirectX includes
#include <dxgi.h> 
#include <d3dcommon.h> 
#include <d3dcompiler.h>

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <xnamath.h>

#include "OPTada_Render_Settings.h"
#include "ResourceManager\OPTadaC_ResourceManager.h"

#include "OPTada_ConstantBuffers.h"

#include "OPTadaS_WorldNavigationData.h"
#include "OPTadaC_Obj_Camera.h"
#include "OPTadaC_Obj_Draw.h"
#include "OPTadaC_Obj_Light.h"
#include "OPTadaC_Obj_Collision.h"



// macros for building 32 - bit color A.8.8.8 format(8 - bit alpha mode)
#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))
// macros for building 24 - bit color 8.8.8 
#define _RGB24BIT(r,g,b)   ((b) + ((g) << 8) + ((r) << 16))


// Enum for OPTada_Render Rasterizer list mass
enum OPTadaE_RasterizerMass_ForRender
{
	ENUM_RasterizerMass_DrawFront_ClipBack   = 0, // default rasteraizer - draw front triangles
	ENUM_RasterizerMass_Debug_DrawAllLines   = 1, // debug mode, will draw all lines
	ENUM_RasterizerMass_Debug_DrawFrontLines = 2, // debug mode, will draw front lines
	ENUM_RasterizerMass_DrawBack_ClipFront   = 3, // draw back triangles
	ENUM_RasterizerMass_DrawAll              = 4, // draw all tringles
};


// Enum for OPTada_Render Blend state list mass
enum OPTadaE_BlendStateMass_ForRender
{
	ENUM_BlendStateMass_NONE = 0,        // NONE - turn off bland state
	ENUM_BlendStateMass_Transparent = 1, // blend state - transparent
};

// Renger
class OPTada_Render
{
public:

	OPTadaC_ResourceManager resourceManager; // GPU resource manager


	bool vSinc = false; // vertycal sincronithzation

	ID3D11Device*           g_Device_d3d11        = nullptr; // device
	ID3D11DeviceContext*    g_DeviceContext_d3d11 = nullptr; // device context
	IDXGISwapChain*         g_SwapChain           = nullptr; // swap chain

	ID3D11RenderTargetView* g_RenderTargetView_d3d   = nullptr; // Render target view for the back buffer of the swap chain.	
	ID3D11DepthStencilView* g_DepthStencilView_d3d   = nullptr; // Depth/stencil view for use as a depth buffer.
	ID3D11Texture2D*        g_DepthStencilBuffer_d3d = nullptr; // A texture to associate to the depth stencil view.

	ID3D11DepthStencilState* g_DepthStencilState_d3d = nullptr; // Define the functionality of the depth/stencil stages.

	std::vector<ID3D11RasterizerState*> g_RasterizerMass; // rasterizer list (watch Init_AllRasterizerState()) (Use OPTadaE_RasterizerMass_ForRender)
	std::vector<ID3D11BlendState*> g_BlendStateMass = { nullptr }; // blend state list (watch Init_AllBlendState()) (Use OPTadaE_BlendStateMass_ForRender)

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

	// init rasterizer list here
	// return = true - done | false - error
	bool Init_AllRasterizerState();

	// init blend state list here
	// return = true - done | false - error
	bool Init_AllBlendState();

	// and create support render resourses (depth buffer, viewPort, rasterither....)
	// [in] int workspaceWidth_       // workspace Width
	// [in] int workspaceHeight_      // workspace Height
	// return = true - done | false - error
	bool InitializeSecondaryResources(int workspaceWidth_, int workspaceHeight_);

	// Use this for free all resourses
	void ShuttingDown();


	// setup new settings for render
	// [in] int workspaceWidth_       // workspace Width
	// [in] int workspaceHeight_      // workspace Height
	// [in] bool vSinc_               // true - enable | false - disable
	// [in] int countOfBackBuffers_   // count of back buffers (1 - double bufferization) (2 - tripple bufferization)
	// return = true - done | false - error
	bool Setup_NewSettingsForRender(int workspaceWidth_, int workspaceHeight_, bool vSinc_, int countOfBackBuffers_);

	// setup fullscreen or windowed mode for render
	// [in] bool isFullScreen_ // true - set fullScreen
	void Setup_FullScreenMode(bool isFullScreen_);


	// Clear the color and depth buffers.
	void PrepareBuffersForNewFrame(const FLOAT clearColor[4], FLOAT clearDepth, UINT8 clearStencil);

	// setup new rasterizer for darw machine
	// [in] OPTadaE_RasterizerMass_ForRender rasterizaerEnum_ // risterizer enum
	void Setup_NewRasterizer(OPTadaE_RasterizerMass_ForRender rasterizaerEnum_);

	// setup blend state for darw machine
	// [in] OPTadaE_BlendStateMass_ForRender blendStateEnum_ // blend state enum
	// [in] float* blendFactor_                              // blend factor
	void Setup_NewBlendState(OPTadaE_BlendStateMass_ForRender blendStateEnum_, float* blendFactor_);

	// Present frame
	void PresentFrame();


	// Get the latest profile for the specified shader type.
	// return = string - with lastest profile name for Pixel|Vertex shader
	template< class ShaderClass >
	std::string GetLatestShaderProfile();

};


// --------------------------------------------------------------------------------------------


// Created class OPTada_Render for render
extern OPTada_Render global_Render;
