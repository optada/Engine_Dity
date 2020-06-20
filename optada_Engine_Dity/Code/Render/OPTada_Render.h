// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include <dxgi.h> 
#include <d3dcommon.h> 
#include <d3dx11.h>
#include <d3dx10math.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <xnamath.h>
#include <dinput.h>

#include <d3dcompiler.h>


// Renger
class OPTada_Render
{
private:



public:

	// initialithation of render
	// return = true - done | false - error
	bool Initialization();

	// Use this for free all resourses
	void ShuttingDown();

	// 
	bool Setup_NewSettingsFor();


};