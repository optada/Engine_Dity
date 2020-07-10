// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once


#include "ResourceManager\OPTadaC_ResourceManager.h" 
#include "OPTadaC_Obj_Camera.h"

class OPTadaC_ShadowClass
{
public:

	OPTadaC_Obj_Camera camera;

	D3D11_VIEWPORT m_viewport;

	ID3D11Texture2D*          sh_RTTexture;
	ID3D11RenderTargetView*   sh_RTV;
	ID3D11ShaderResourceView* sh_SRV;

	ID3D11Texture2D*        sh_DepthSTexture;
	ID3D11DepthStencilView* sh_DepthSV;


	~OPTadaC_ShadowClass() {
		SafeRelease(sh_RTTexture);
		SafeRelease(sh_RTV);
		SafeRelease(sh_SRV);
		SafeRelease(sh_DepthSTexture);
		SafeRelease(sh_DepthSV);
	}

	// init shadow class
	bool init(ID3D11Device* gDevice_, int size_, float screenNear, float screenDepth) {

		// create shadow map texture
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = size_;
		textureDesc.Height = size_;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		if (FAILED(gDevice_->CreateTexture2D(&textureDesc, NULL, &sh_RTTexture))) {
			return false;
		}

		// create render target view
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		if (FAILED(gDevice_->CreateRenderTargetView(sh_RTTexture, &renderTargetViewDesc, &sh_RTV))) {
			return false;
		}

		// create shader resource view for texture
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		if (FAILED(gDevice_->CreateShaderResourceView(sh_RTTexture, &shaderResourceViewDesc, &sh_SRV))) {
			return false;
		}


		// --- create Depth Stencil View

		// create depth texture
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
		depthBufferDesc.Width = size_;
		depthBufferDesc.Height = size_;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;
		if (FAILED(gDevice_->CreateTexture2D(&depthBufferDesc, NULL, &sh_DepthSTexture)))
			return false;

		// create depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.Format = depthBufferDesc.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		if (FAILED(gDevice_->CreateDepthStencilView(sh_DepthSTexture, &depthStencilViewDesc, &sh_DepthSV)))
			return false;

		
		// setup projection matrix

		m_viewport.Width = (float)size_;
		m_viewport.Height = (float)size_;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;

		// create projection matrix
		camera.projection_Matrix = XMMatrixPerspectiveFovLH(((float)XM_PI / 4.0f), ((float)size_ / (float)size_), screenNear, screenDepth);

		return true;
	}


	void ClearRenderTarget(ID3D11DeviceContext* gDeviceContext_, float red_, float green_, float blue_, float alpha_)
	{
		float color[4] = { red_, green_, blue_, alpha_ };
		gDeviceContext_->ClearRenderTargetView(sh_RTV, color);
		gDeviceContext_->ClearDepthStencilView(sh_DepthSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void SetRenderTarget(ID3D11DeviceContext* gDeviceContext_)
	{
		gDeviceContext_->OMSetRenderTargets(1, &sh_RTV, sh_DepthSV);
		gDeviceContext_->RSSetViewports(1, &m_viewport);
	}


	ID3D11ShaderResourceView* GetShaderResourceView()
	{
		return sh_SRV;
	}

};