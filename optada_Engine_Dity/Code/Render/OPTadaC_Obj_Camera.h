// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once

#include "OPTadaS_WorldNavigationData.h"


// class - camera
class OPTadaC_Obj_Camera
{
public:

    XMMATRIX view_Matrix       = XMMatrixIdentity(); // viev matrix
    XMMATRIX projection_Matrix = XMMatrixIdentity(); // projection matrix (FOV)

    FVec3 position = { 0.0f, 0.0f, 0.0f }; // camera position
    FVec3 lookAt   = { 0.0f, 0.0f, 0.0f }; // watch point (look at)
    FVec3 rotate   = { 0.0f, 0.0f, 0.0f }; // X (Pitch) | Y (Yaw) | Z (roll) ()
    
    XMVECTOR camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); // Y

    void Setup_ProjectionMatrix(float clientWidth_, float clientHeight_, float FOV_) {
        projection_Matrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(FOV_), clientWidth_ / clientHeight_, 0.1f, 100.0f);
    }

    void Update_ViewMatrix()
    {
        XMVECTOR pos    = XMVectorSet(position.X, position.Y, position.Z, 1); // camera position
        XMVECTOR target = XMVectorSet(lookAt.X, lookAt.Y, lookAt.Z, 1);       // watch point (look at)

        view_Matrix = XMMatrixLookAtLH(pos, target, camUp);

        XMMATRIX rotateMatrix;
        if (rotate.X) { // rotate x Pitch
            rotateMatrix = XMMatrixRotationX(XMConvertToRadians(rotate.X));
            view_Matrix *= rotateMatrix;
        }
        if (rotate.Y) { // rotate y Yaw
            rotateMatrix = XMMatrixRotationY(XMConvertToRadians(rotate.Y));
            view_Matrix *= rotateMatrix;
        }
        if (rotate.Z) { // rotate z roll
            rotateMatrix = XMMatrixRotationZ(XMConvertToRadians(rotate.Z));
            view_Matrix *= rotateMatrix;
        }
    }
};