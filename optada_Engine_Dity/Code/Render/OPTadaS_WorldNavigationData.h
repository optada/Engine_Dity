// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include <DirectXMath.h>
#include <xnamath.h>


// 3Float vector
struct FVec3
{
	float X;
	float Y;
	float Z;
};


// support struct for game objects
struct OPTadaS_WorldNavigationData
{
public:

	XMMATRIX matrix; // matrix
	
	FVec3 position; // position
	FVec3 rotation; // rotation
	FVec3 scaling;  // scaling


	// zero constructor
	OPTadaS_WorldNavigationData() {

		matrix = XMMatrixIdentity();

		// zero local
		position = { 0.0f, 0.0f, 0.0f};
		rotation = { 0.0f, 0.0f, 0.0f};
		scaling  = { 1.0f, 1.0f, 1.0f};
	}

	void calcPosition(XMMATRIX& matrixOut_) {
		matrixOut_ = XMMatrixTranslation(position.X, position.Y, position.Z);
	}

	void calcRotation(XMMATRIX& matrixOut_) {
		XMMATRIX rot_X = XMMatrixRotationX(XMConvertToRadians(rotation.X));
		XMMATRIX rot_Y = XMMatrixRotationY(XMConvertToRadians(rotation.Y));
		XMMATRIX rot_Z = XMMatrixRotationZ(XMConvertToRadians(rotation.Z));
		matrixOut_ = rot_X * rot_Y * rot_Z;
	}

	void calcScaling(XMMATRIX& matrixOut_) {
		matrixOut_ = XMMatrixScaling(scaling.X, scaling.Y, scaling.Z);
	}

	void calcMatrix_SRT(XMMATRIX& matrixScale_, XMMATRIX& matrixRotate_, XMMATRIX& matrixTranslation_) {
		matrix = matrixScale_ * matrixRotate_ * matrixTranslation_;
	}

	void calcMatrix_SR(XMMATRIX& matrixScale_, XMMATRIX& matrixRotate_) {
		matrix = matrixScale_ * matrixRotate_;
	}

	void calcMatrix_ST(XMMATRIX& matrixScale_, XMMATRIX& matrixTranslation_) {
		matrix = matrixScale_ * matrixTranslation_;
	}

	void calcMatrix_RT(XMMATRIX& matrixRotate_, XMMATRIX& matrixTranslation_) {
		matrix = matrixRotate_ * matrixTranslation_;
	}
};



//XMVECTOR;
//XMFLOAT3;
//XMMATRIX;