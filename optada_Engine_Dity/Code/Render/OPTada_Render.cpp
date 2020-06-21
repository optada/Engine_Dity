// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#include "OPTada_Render.h"


bool OPTada_Render::Initialization(HWND hwnd_, int countOfBackBuffers_, int workspaceWidth_, int workspaceHeight_, bool vSinc_, bool isWindowedMode_)
{

    HRESULT hr;

    //Describe our Buffer
    DXGI_MODE_DESC bufferDesc;

    ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

    // get display FPS
    DEVMODE dm;
    dm.dmSize = sizeof(DEVMODE);
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);

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
        NULL,                     // default adapter
        D3D_DRIVER_TYPE_HARDWARE, // use harware DirectX realization
        NULL,                      
        createDeviceFlags,
        featureLevels,            // feature levels list
        _countof(featureLevels),  // count of futer levels
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &g_SwapChain,
        &g_Device_d3d11,
        NULL,
        &g_DeviceContext_d3d11)) 
    {
        return false;
    }

    //Create our BackBuffer
    ID3D11Texture2D* backBuffer;
    if (S_OK != g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer)) {
        return false;
    }

    //Create our Render Target
    if (S_OK != g_Device_d3d11->CreateRenderTargetView(backBuffer, NULL, &g_RenderTargetView_d3d)) {
        SafeRelease(backBuffer);
        return false;
    }
    SafeRelease(backBuffer);

    //Set our Render Target
    g_DeviceContext_d3d11->OMSetRenderTargets(1, &g_RenderTargetView_d3d, NULL);

    return true;
}
    
void OPTada_Render::ShuttingDown()
{

    SafeRelease(g_RasterizerState_d3d);   // Define the functionality of the rasterizer stage.
    SafeRelease(g_DepthStencilState_d3d); // Define the functionality of the depth/stencil stages.
    
    SafeRelease(g_DepthStencilBuffer_d3d); // A texture to associate to the depth stencil view.
    SafeRelease(g_DepthStencilView_d3d);   // Depth/stencil view for use as a depth buffer.
    SafeRelease(g_RenderTargetView_d3d);   // Render target view for the back buffer of the swap chain.	

    SafeRelease(g_SwapChain);           // swap chain
    SafeRelease(g_DeviceContext_d3d11); // device context
    SafeRelease(g_Device_d3d11);        // device

}


// --------------------------------------------------------------------------------------------


OPTada_Render global_render;