// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#include "OPTada_Render.h"


bool OPTada_Render::Initialization(HWND hwnd_, int countOfBackBuffers_, int workspaceWidth_, int workspaceHeight_, bool vSinc_, bool isWindowedMode_)
{
    vSinc = vSinc_;

    // ---- Create DirectX 11 basic ----

    HRESULT hr;

    //Describe our Buffer
    DXGI_MODE_DESC bufferDesc;

    ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

    // get display FPS
    DEVMODE dm;
    dm.dmSize = sizeof(DEVMODE);
    EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm);

    bufferDesc.Width                   = workspaceWidth_;
    bufferDesc.Height                  = workspaceHeight_;
    bufferDesc.RefreshRate.Numerator   = (vSinc_)?(dm.dmDisplayFrequency):(0); // set display FPS if vSinc is true, or max FPS
    bufferDesc.RefreshRate.Denominator = 1;
    bufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;           // set format
    bufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // rasterizator settings
    bufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;        // rasterizator settings

    //Describe our SwapChain
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
    swapChainDesc.BufferDesc         = bufferDesc;
    swapChainDesc.SampleDesc.Count   = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount        = countOfBackBuffers_;
    swapChainDesc.OutputWindow       = hwnd_;
    swapChainDesc.Windowed           = isWindowedMode_;
    swapChainDesc.SwapEffect         = DXGI_SWAP_EFFECT_SEQUENTIAL; // - just swap buffer // DXGI_SWAP_EFFECT_DISCARD; - discard all information in buffer

    // create device flags
    UINT createDeviceFlags = NULL;
#if _DEBUG
    createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

    // These are the feature levels that we will accept.
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        //D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };

    // This will be the feature level that 
    // is used to create our device and swap chain.
    // D3D_FEATURE_LEVEL featureLevel;

    //Create our Device and SwapChain
    if (S_OK != D3D11CreateDeviceAndSwapChain(
        nullptr,                  // default adapter
        D3D_DRIVER_TYPE_HARDWARE, // use harware DirectX realization
        nullptr,
        createDeviceFlags,
        featureLevels,            // feature levels list
        _countof(featureLevels),  // count of futer levels
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &g_SwapChain,
        &g_Device_d3d11,
        nullptr,
        &g_DeviceContext_d3d11)) 
    {
        return false;
    }

    if (!InitializeSecondaryResources(workspaceWidth_, workspaceHeight_)) {
        return false;
    }

    // off ALT+ENTER reaction for DXGI
    IDXGIDevice* pDXGIDevice;
    IDXGIAdapter* pDXGIAdapter;
    IDXGIFactory* pIDXGIFactory;
    g_Device_d3d11->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);
    pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pDXGIAdapter);
    pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pIDXGIFactory);
    pIDXGIFactory->MakeWindowAssociation(hwnd_, DXGI_MWA_NO_ALT_ENTER);  
    pIDXGIFactory->Release(); // free DXGI factory.
    pIDXGIFactory = 0;   
    pDXGIAdapter->Release(); // free DXGI adapter.
    pDXGIAdapter = 0;  
    pDXGIDevice->Release(); // free DXGI device.
    pDXGIDevice = 0;

    // ---- Setup rasterizer ----

    if (!Init_AllRasterizerState()) {
        return false;
    }

    // ---- Setup Blend State ----

    if (!Init_AllBlendState()) {
        return false;
    }

    // ---- ALL initialization done ----
    
    if (!resourceManager.Init_ResourceManager(g_Device_d3d11, { sizeof(OPTadaS_CB_GlobalData), sizeof(OPTadaS_CB_FrameData), sizeof(OPTadaS_CB_ObjectData) })) {
        return false;
    }

    return true;
}

bool OPTada_Render::Init_AllRasterizerState()
{ 
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ID3D11RasterizerState* g_RasterizerState_d3d = nullptr; // Define the functionality of the rasterizer stage.

    // Setup rasterizer state default ENUM_RasterizerMass_Default_DrawFront
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc.AntialiasedLineEnable = TRUE;
    rasterizerDesc.CullMode              = D3D11_CULL_BACK;  // do not draw backfront triangles
    rasterizerDesc.DepthBias             = 0;
    rasterizerDesc.DepthBiasClamp        = 0.0f;
    rasterizerDesc.DepthClipEnable       = TRUE;             // clip z coord
    rasterizerDesc.FillMode              = D3D11_FILL_SOLID; // draw triangles formed // D3D11_FILL_WIREFRAME - darw lines
    rasterizerDesc.FrontCounterClockwise = FALSE;            // a triangle will be considered front-facing if its vertices are counter-clockwise on the render target
    rasterizerDesc.MultisampleEnable     = FALSE;
    rasterizerDesc.ScissorEnable         = FALSE;            // is scissor-rectangle culling. TRUE - enable
    rasterizerDesc.SlopeScaledDepthBias  = 0.0f;

    // Create the rasterizer state object.
    if (S_OK != g_Device_d3d11->CreateRasterizerState(&rasterizerDesc, &g_RasterizerState_d3d)) {
        return false;
    }
    g_RasterizerMass.push_back(g_RasterizerState_d3d);
    g_RasterizerState_d3d = nullptr;


    // Setup rasterizer state default ENUM_RasterizerMass_Debug_DrawAllLines
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc.AntialiasedLineEnable = TRUE;
    rasterizerDesc.CullMode              = D3D11_CULL_NONE;      // draw all triangles
    rasterizerDesc.DepthBias             = 0;
    rasterizerDesc.DepthBiasClamp        = 0.0f;
    rasterizerDesc.DepthClipEnable       = FALSE;                // no clip z coord
    rasterizerDesc.FillMode              = D3D11_FILL_WIREFRAME; // darw lines
    rasterizerDesc.FrontCounterClockwise = FALSE;                // a triangle will be considered front-facing if its vertices are counter-clockwise on the render target
    rasterizerDesc.MultisampleEnable     = FALSE;
    rasterizerDesc.ScissorEnable         = FALSE;                // is scissor-rectangle culling. TRUE - enable
    rasterizerDesc.SlopeScaledDepthBias  = 0.0f;

    // Create the rasterizer state object.
    if (S_OK != g_Device_d3d11->CreateRasterizerState(&rasterizerDesc, &g_RasterizerState_d3d)) {
        return false;
    }
    g_RasterizerMass.push_back(g_RasterizerState_d3d);
    g_RasterizerState_d3d = nullptr;


    // Setup rasterizer state default ENUM_RasterizerMass_Debug_DrawFrontLines
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc.AntialiasedLineEnable = TRUE;
    rasterizerDesc.CullMode              = D3D11_CULL_BACK;      // draw backfront triangles
    rasterizerDesc.DepthBias             = 0;
    rasterizerDesc.DepthBiasClamp        = 0.0f;
    rasterizerDesc.DepthClipEnable       = FALSE;                // clip z coord
    rasterizerDesc.FillMode              = D3D11_FILL_WIREFRAME; // darw lines
    rasterizerDesc.FrontCounterClockwise = FALSE;                // a triangle will be considered front-facing if its vertices are counter-clockwise on the render target
    rasterizerDesc.MultisampleEnable     = FALSE;
    rasterizerDesc.ScissorEnable         = FALSE;                // is scissor-rectangle culling. TRUE - enable
    rasterizerDesc.SlopeScaledDepthBias  = 0.0f;

    // Create the rasterizer state object.
    if (S_OK != g_Device_d3d11->CreateRasterizerState(&rasterizerDesc, &g_RasterizerState_d3d)) {
        return false;
    }
    g_RasterizerMass.push_back(g_RasterizerState_d3d);
    g_RasterizerState_d3d = nullptr;


    // Setup rasterizer state default ENUM_RasterizerMass_DrawBack_ClipFront
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc.AntialiasedLineEnable = TRUE;
    rasterizerDesc.CullMode              = D3D11_CULL_BACK;  // draw backfront triangles
    rasterizerDesc.DepthBias             = 0;
    rasterizerDesc.DepthBiasClamp        = 0.0f;
    rasterizerDesc.DepthClipEnable       = FALSE;            // clip z coord
    rasterizerDesc.FillMode              = D3D11_FILL_SOLID; // darw lines
    rasterizerDesc.FrontCounterClockwise = TRUE;             // a triangle will be considered front-facing if its vertices are counter-clockwise on the render target
    rasterizerDesc.MultisampleEnable     = FALSE;
    rasterizerDesc.ScissorEnable         = FALSE;            // is scissor-rectangle culling. TRUE - enable
    rasterizerDesc.SlopeScaledDepthBias  = 0.0f;

    // Create the rasterizer state object.
    if (S_OK != g_Device_d3d11->CreateRasterizerState(&rasterizerDesc, &g_RasterizerState_d3d)) {
        return false;
    }
    g_RasterizerMass.push_back(g_RasterizerState_d3d);
    g_RasterizerState_d3d = nullptr;


    // Setup rasterizer state default ENUM_RasterizerMass_DrawAll
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc.AntialiasedLineEnable = TRUE;
    rasterizerDesc.CullMode              = D3D11_CULL_NONE;  // draw all triangles
    rasterizerDesc.DepthBias             = 0;
    rasterizerDesc.DepthBiasClamp        = 0.0f;
    rasterizerDesc.DepthClipEnable       = FALSE;            // clip z coord
    rasterizerDesc.FillMode              = D3D11_FILL_SOLID; // darw lines
    rasterizerDesc.FrontCounterClockwise = FALSE;            // a triangle will be considered front-facing if its vertices are counter-clockwise on the render target
    rasterizerDesc.MultisampleEnable     = FALSE;
    rasterizerDesc.ScissorEnable         = FALSE;            // is scissor-rectangle culling. TRUE - enable
    rasterizerDesc.SlopeScaledDepthBias  = 0.0f;

    // Create the rasterizer state object.
    if (S_OK != g_Device_d3d11->CreateRasterizerState(&rasterizerDesc, &g_RasterizerState_d3d)) {
        return false;
    }
    g_RasterizerMass.push_back(g_RasterizerState_d3d);
    g_RasterizerState_d3d = nullptr;
}

bool OPTada_Render::Init_AllBlendState()
{
    ID3D11BlendState* newBlendState = nullptr;

    D3D11_BLEND_DESC blendDesc;
    ZeroMemory(&blendDesc, sizeof(blendDesc));

    D3D11_RENDER_TARGET_BLEND_DESC rtbd;
    ZeroMemory(&rtbd, sizeof(rtbd));

    rtbd.BlendEnable           = true;
    rtbd.SrcBlend              = D3D11_BLEND_SRC_COLOR;
    rtbd.DestBlend             = D3D11_BLEND_BLEND_FACTOR;
    rtbd.BlendOp               = D3D11_BLEND_OP_ADD;
    rtbd.SrcBlendAlpha         = D3D11_BLEND_ONE;
    rtbd.DestBlendAlpha        = D3D11_BLEND_ZERO;
    rtbd.BlendOpAlpha          = D3D11_BLEND_OP_ADD;
    rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.RenderTarget[0] = rtbd;

    // Create the blend state object
    if (S_OK != g_Device_d3d11->CreateBlendState(&blendDesc, &newBlendState)) {
        return false;
    }

    g_BlendStateMass.push_back(newBlendState);
    newBlendState = nullptr;

    return true;
}

bool OPTada_Render::InitializeSecondaryResources(int workspaceWidth_, int workspaceHeight_)
{
    //Create our BackBuffer
    ID3D11Texture2D* backBuffer;
    if (S_OK != g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer)) {
        return false;
    }

    // ---- Create our Render Target ----

    //Create our Render Target
    if (S_OK != g_Device_d3d11->CreateRenderTargetView(backBuffer, nullptr, &g_RenderTargetView_d3d)) {
        SafeRelease(backBuffer);
        return false;
    }
    SafeRelease(backBuffer);

    //Set our Render Target
    g_DeviceContext_d3d11->OMSetRenderTargets(1, &g_RenderTargetView_d3d, nullptr);

    // ---- Depth | stencil buffer ----

    // Create the depth buffer for use with the depth/stencil view.
    D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
    ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
    depthStencilBufferDesc.ArraySize          = 1;                             // count of textures
    depthStencilBufferDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;      // use like a Depth buffer
    depthStencilBufferDesc.CPUAccessFlags     = 0;                             // No CPU access required.
    depthStencilBufferDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilBufferDesc.Width              = workspaceWidth_;
    depthStencilBufferDesc.Height             = workspaceHeight_;
    depthStencilBufferDesc.MipLevels          = 1;                             // Use 1 for a multisampled texture; or 0 to generate a full set of subtextures.
    depthStencilBufferDesc.SampleDesc.Count   = 1;
    depthStencilBufferDesc.SampleDesc.Quality = 0;
    depthStencilBufferDesc.Usage              = D3D11_USAGE_DEFAULT;

    // create texture for depth buffer
    if (S_OK != g_Device_d3d11->CreateTexture2D(&depthStencilBufferDesc, nullptr, &g_DepthStencilBuffer_d3d)) {
        return false;
    }

    // create stencil view, for depth buffer
    if (S_OK != g_Device_d3d11->CreateDepthStencilView(g_DepthStencilBuffer_d3d, nullptr, &g_DepthStencilView_d3d)) {
        return false;
    }

    // Setup depth/stencil state.
    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
    ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

    depthStencilStateDesc.DepthEnable    = TRUE; // depth enable
    depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilStateDesc.DepthFunc      = D3D11_COMPARISON_LESS;

    depthStencilStateDesc.StencilEnable    = TRUE; // stencil enable
    depthStencilStateDesc.StencilReadMask  = D3D11_DEFAULT_STENCIL_READ_MASK;
    depthStencilStateDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

    // Stencil operations if pixel is front-facing
    depthStencilStateDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilStateDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing
    depthStencilStateDesc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilStateDesc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

    // create depth stencil state
    if (S_OK != g_Device_d3d11->CreateDepthStencilState(&depthStencilStateDesc, &g_DepthStencilState_d3d)) {
        return false;
    }

    // ---- Setup viewport ----

    // Initialize the viewport to occupy the entire client area.
    g_Viewport.Width    = static_cast<float>(workspaceWidth_);
    g_Viewport.Height   = static_cast<float>(workspaceHeight_);
    g_Viewport.TopLeftX = 0.0f;
    g_Viewport.TopLeftY = 0.0f;
    g_Viewport.MinDepth = 0.0f;
    g_Viewport.MaxDepth = 1.0f;

    //g_DeviceContext_d3d11->RSGetViewports(0, &g_Viewport);

    return true;
}

void OPTada_Render::ShuttingDown()
{
    resourceManager.FreeAll();

    g_SwapChain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

    for (int i = 0; i < g_RasterizerMass.size(); i++) {  // free rasterizer stage.
        SafeRelease(g_RasterizerMass[i]);
    }
   
    SafeRelease(g_DepthStencilState_d3d); // Define the functionality of the depth/stencil stages.

    SafeRelease(g_DepthStencilBuffer_d3d); // A texture to associate to the depth stencil view.
    SafeRelease(g_DepthStencilView_d3d);   // Depth/stencil view for use as a depth buffer.
    SafeRelease(g_RenderTargetView_d3d);   // Render target view for the back buffer of the swap chain.	

    SafeRelease(g_SwapChain);           // swap chain
    SafeRelease(g_DeviceContext_d3d11); // device context
    SafeRelease(g_Device_d3d11);        // device

}


bool OPTada_Render::Setup_NewSettingsForRender(int workspaceWidth_, int workspaceHeight_, bool vSinc_, int countOfBackBuffers_)
{
    vSinc = vSinc_;

    // free all resourses
    g_DeviceContext_d3d11->OMSetRenderTargets(0, 0, 0);
    //SafeRelease(g_RasterizerState_d3d);    // Define the functionality of the rasterizer stage.
    SafeRelease(g_DepthStencilState_d3d);  // Define the functionality of the depth/stencil stages.
    SafeRelease(g_DepthStencilBuffer_d3d); // A texture to associate to the depth stencil view.
    SafeRelease(g_DepthStencilView_d3d);   // Depth/stencil view for use as a depth buffer.
    SafeRelease(g_RenderTargetView_d3d);   // Render target view for the back buffer of the swap chain.	

    //Describe our Buffer
    DXGI_MODE_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

    // get display FPS
    DEVMODE dm;
    dm.dmSize = sizeof(DEVMODE);
    EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm);

    bufferDesc.Width                   = workspaceWidth_;
    bufferDesc.Height                  = workspaceHeight_;
    bufferDesc.RefreshRate.Numerator   = (vSinc_) ? (dm.dmDisplayFrequency) : (0); // set display FPS if vSinc is true, or max FPS
    bufferDesc.RefreshRate.Denominator = 1;
    bufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;           // set format
    bufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // rasterizator settings
    bufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;        // rasterizator settings

    // set new target parameters
    g_SwapChain->ResizeTarget(&bufferDesc);

    // resize settings for swapChain
    g_SwapChain->ResizeBuffers(countOfBackBuffers_, workspaceWidth_, workspaceHeight_, DXGI_FORMAT_UNKNOWN, 0);

    // create all support resouses again
    if (!InitializeSecondaryResources(workspaceWidth_, workspaceHeight_)) {
        return false;
    }

    return true;
}

void OPTada_Render::Setup_FullScreenMode(bool isFullScreen_)
{
    g_SwapChain->SetFullscreenState(isFullScreen_, NULL);
}


void OPTada_Render::PrepareBuffersForNewFrame(const FLOAT clearColor[4], FLOAT clearDepth, UINT8 clearStencil)
{
    g_DeviceContext_d3d11->ClearRenderTargetView(g_RenderTargetView_d3d, clearColor);
    g_DeviceContext_d3d11->ClearDepthStencilView(g_DepthStencilView_d3d, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, clearDepth, clearStencil);
}

void OPTada_Render::Setup_NewRasterizer(OPTadaE_RasterizerMass_ForRender rasterizaerEnum_)
{
    g_DeviceContext_d3d11->RSSetState(g_RasterizerMass[rasterizaerEnum_]);
}

void OPTada_Render::Setup_NewBlendState(OPTadaE_BlendStateMass_ForRender blendStateEnum_, float* blendFactor_)
{
    if (blendStateEnum_ == 0) { // turn off blend state
        g_DeviceContext_d3d11->OMSetBlendState(0, 0, 0xffffffff);
    }
    else { // set blend state
        global_Render.g_DeviceContext_d3d11->OMSetBlendState(g_BlendStateMass[blendStateEnum_], blendFactor_, 0xffffffff);
    }
}

void OPTada_Render::PresentFrame()
{
    if (vSinc) {
        g_SwapChain->Present(1, 0);
    }
    else {
        g_SwapChain->Present(0, 0);
    }
}


template<>
std::string OPTada_Render::GetLatestShaderProfile<ID3D11VertexShader>()
{
    assert(g_Device_d3d11);

    // Query the current feature level:
    D3D_FEATURE_LEVEL featureLevel = g_Device_d3d11->GetFeatureLevel();

    switch (featureLevel)
    {
        //case D3D_FEATURE_LEVEL_11_1:
    case D3D_FEATURE_LEVEL_11_0:
    {
        return "vs_5_0";
    }
    break;
    case D3D_FEATURE_LEVEL_10_1:
    {
        return "vs_4_1";
    }
    break;
    case D3D_FEATURE_LEVEL_10_0:
    {
        return "vs_4_0";
    }
    break;
    case D3D_FEATURE_LEVEL_9_3:
    {
        return "vs_4_0_level_9_3";
    }
    break;
    case D3D_FEATURE_LEVEL_9_2:
    case D3D_FEATURE_LEVEL_9_1:
    {
        return "vs_4_0_level_9_1";
    }
    break;
    } // switch( featureLevel )

    return "";
}

template<>
std::string OPTada_Render::GetLatestShaderProfile<ID3D11PixelShader>()
{
    assert(g_Device_d3d11);

    // Query the current feature level:
    D3D_FEATURE_LEVEL featureLevel = g_Device_d3d11->GetFeatureLevel();
    switch (featureLevel)
    {
        //case D3D_FEATURE_LEVEL_11_1:
    case D3D_FEATURE_LEVEL_11_0:
    {
        return "ps_5_0";
    }
    break;
    case D3D_FEATURE_LEVEL_10_1:
    {
        return "ps_4_1";
    }
    break;
    case D3D_FEATURE_LEVEL_10_0:
    {
        return "ps_4_0";
    }
    break;
    case D3D_FEATURE_LEVEL_9_3:
    {
        return "ps_4_0_level_9_3";
    }
    break;
    case D3D_FEATURE_LEVEL_9_2:
    case D3D_FEATURE_LEVEL_9_1:
    {
        return "ps_4_0_level_9_1";
    }
    break;
    }
    return "";
}

// --------------------------------------------------------------------------------------------


OPTada_Render global_Render;