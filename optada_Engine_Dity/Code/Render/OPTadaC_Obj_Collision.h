// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

#pragma once  

#include <DirectXMath.h>
#include <xnamath.h>

#include "OPTadaS_WorldNavigationData.h"
#include "ResourceManager\MeshList.h"


// static class with methods for check collision
class OPTadaC_Collision
{
public:

	static bool BoundingBoxCollision(
        XMVECTOR& firstObjBoundingBoxMinVertex,
        XMVECTOR& firstObjBoundingBoxMaxVertex,
        XMVECTOR& secondObjBoundingBoxMinVertex,
        XMVECTOR& secondObjBoundingBoxMaxVertex)
    {
        //Is obj1's max X greater than obj2's min X? If not, obj1 is to the LEFT of obj2
        if (XMVectorGetX(firstObjBoundingBoxMaxVertex) > XMVectorGetX(secondObjBoundingBoxMinVertex))

            //Is obj1's min X less than obj2's max X? If not, obj1 is to the RIGHT of obj2
            if (XMVectorGetX(firstObjBoundingBoxMinVertex) < XMVectorGetX(secondObjBoundingBoxMaxVertex))

                //Is obj1's max Y greater than obj2's min Y? If not, obj1 is UNDER obj2
                if (XMVectorGetY(firstObjBoundingBoxMaxVertex) > XMVectorGetY(secondObjBoundingBoxMinVertex))

                    //Is obj1's min Y less than obj2's max Y? If not, obj1 is ABOVE obj2
                    if (XMVectorGetY(firstObjBoundingBoxMinVertex) < XMVectorGetY(secondObjBoundingBoxMaxVertex))

                        //Is obj1's max Z greater than obj2's min Z? If not, obj1 is IN FRONT OF obj2
                        if (XMVectorGetZ(firstObjBoundingBoxMaxVertex) > XMVectorGetZ(secondObjBoundingBoxMinVertex))

                            //Is obj1's min Z less than obj2's max Z? If not, obj1 is BEHIND obj2
                            if (XMVectorGetZ(firstObjBoundingBoxMinVertex) < XMVectorGetZ(secondObjBoundingBoxMaxVertex))

                                //If we've made it this far, then the two bounding boxes are colliding
                                return true;

        //If the two bounding boxes are not colliding, then return false
        return false;
    }

    static void CalculateAABB(
        XMFLOAT3* boundingBoxVerts,
        XMMATRIX& worldSpace,
        XMVECTOR& boundingBoxMin,
        XMVECTOR& boundingBoxMax)
    {
        XMFLOAT3 minVertex = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
        XMFLOAT3 maxVertex = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

        //Loop through the 8 vertices describing the bounding box
        for (UINT i = 0; i < 8; i++)
        {
            //Transform the bounding boxes vertices to the objects world space
            XMVECTOR Vert = XMVectorSet(boundingBoxVerts[i].x, boundingBoxVerts[i].y, boundingBoxVerts[i].z, 0.0f);
            Vert = XMVector3TransformCoord(Vert, worldSpace);

            //Get the smallest vertex 
            minVertex.x = min(minVertex.x, XMVectorGetX(Vert));    // Find smallest x value in model
            minVertex.y = min(minVertex.y, XMVectorGetY(Vert));    // Find smallest y value in model
            minVertex.z = min(minVertex.z, XMVectorGetZ(Vert));    // Find smallest z value in model

            //Get the largest vertex 
            maxVertex.x = max(maxVertex.x, XMVectorGetX(Vert));    // Find largest x value in model
            maxVertex.y = max(maxVertex.y, XMVectorGetY(Vert));    // Find largest y value in model
            maxVertex.z = max(maxVertex.z, XMVectorGetZ(Vert));    // Find largest z value in model
        }

        //Store Bounding Box's min and max vertices
        boundingBoxMin = XMVectorSet(minVertex.x, minVertex.y, minVertex.z, 0.0f);
        boundingBoxMax = XMVectorSet(maxVertex.x, maxVertex.y, maxVertex.z, 0.0f);
    }


    //static bool cullAABB(std::vector<XMFLOAT4>& frustumPlanes)
    //{
    //    // This is where we will check all objects for culling. In this lesson, we are only culling the trees, so if the tree is culled,
    //    // we will not draw it OR it's leaves. You can add other objects in your scene, and check them for culling here

    //    // Initialize numTreesToDraw
    //    numTreesToDraw = 0;

    //    // Create a temporary array to get the tree instance data out
    //    std::vector<InstanceData> tempTreeInstDat(numTrees);

    //    bool cull = false;

    //    // We'll start by looping through each tree
    //    for (int i = 0; i < numTrees; ++i)
    //    {
    //        cull = false;
    //        // Loop through each frustum plane
    //        for (int planeID = 0; planeID < 6; ++planeID)
    //        {
    //            XMVECTOR planeNormal = XMVectorSet(frustumPlanes[planeID].x, frustumPlanes[planeID].y, frustumPlanes[planeID].z, 0.0f);
    //            float planeConstant = frustumPlanes[planeID].w;

    //            // Check each axis (x,y,z) to get the AABB vertex furthest away from the direction the plane is facing (plane normal)
    //            XMFLOAT3 axisVert;

    //            // x-axis
    //            if (frustumPlanes[planeID].x < 0.0f)    // Which AABB vertex is furthest down (plane normals direction) the x axis
    //                axisVert.x = treeAABB[0].x + treeInstanceData[i].pos.x; // min x plus tree positions x
    //            else
    //                axisVert.x = treeAABB[1].x + treeInstanceData[i].pos.x; // max x plus tree positions x

    //            // y-axis
    //            if (frustumPlanes[planeID].y < 0.0f)    // Which AABB vertex is furthest down (plane normals direction) the y axis
    //                axisVert.y = treeAABB[0].y + treeInstanceData[i].pos.y; // min y plus tree positions y
    //            else
    //                axisVert.y = treeAABB[1].y + treeInstanceData[i].pos.y; // max y plus tree positions y

    //            // z-axis
    //            if (frustumPlanes[planeID].z < 0.0f)    // Which AABB vertex is furthest down (plane normals direction) the z axis
    //                axisVert.z = treeAABB[0].z + treeInstanceData[i].pos.z; // min z plus tree positions z
    //            else
    //                axisVert.z = treeAABB[1].z + treeInstanceData[i].pos.z; // max z plus tree positions z

    //            // Now we get the signed distance from the AABB vertex that's furthest down the frustum planes normal,
    //            // and if the signed distance is negative, then the entire bounding box is behind the frustum plane, which means
    //            // that it should be culled
    //            if (XMVectorGetX(XMVector3Dot(planeNormal, XMLoadFloat3(&axisVert))) + planeConstant < 0.0f)
    //            {
    //                cull = true;
    //                // Skip remaining planes to check and move on to next tree
    //                break;
    //            }
    //        }

    //        if (!cull)    // If the object was not culled
    //        {
    //            // Set the treesToDrawIndex in the constant buffer. We are rearanging the tree instance positions, so that the trees
    //            // that will be drawn have their positions first. This way, when the GPU loops through the instances, it will first
    //            // get all the tree positions that we want to draw. We are not going to have the GPU draw all 4000 trees, only the couple
    //            // that are in the view frustum, so we want those tree positions to be the first ones in the instance buffer array
    //            tempTreeInstDat[numTreesToDraw].pos = treeInstanceData[i].pos;

    //            // Add one to the number of trees to draw
    //            numTreesToDraw++;
    //        }
    //    }


    //}



    static std::vector<XMFLOAT4> getFrustumPlanes(XMMATRIX& viewProj_) {

        // x, y, z, and w represent A, B, C and D in the plane equation
        // where ABC are the xyz of the planes normal, and D is the plane constant
        std::vector<XMFLOAT4> tempFrustumPlane(6);

        // Left Frustum Plane
        // Add first column of the matrix to the fourth column
        tempFrustumPlane[0].x = viewProj_._14 + viewProj_._11;
        tempFrustumPlane[0].y = viewProj_._24 + viewProj_._21;
        tempFrustumPlane[0].z = viewProj_._34 + viewProj_._31;
        tempFrustumPlane[0].w = viewProj_._44 + viewProj_._41;

        // Right Frustum Plane
        // Subtract first column of matrix from the fourth column
        tempFrustumPlane[1].x = viewProj_._14 - viewProj_._11;
        tempFrustumPlane[1].y = viewProj_._24 - viewProj_._21;
        tempFrustumPlane[1].z = viewProj_._34 - viewProj_._31;
        tempFrustumPlane[1].w = viewProj_._44 - viewProj_._41;

        // Top Frustum Plane
        // Subtract second column of matrix from the fourth column
        tempFrustumPlane[2].x = viewProj_._14 - viewProj_._12;
        tempFrustumPlane[2].y = viewProj_._24 - viewProj_._22;
        tempFrustumPlane[2].z = viewProj_._34 - viewProj_._32;
        tempFrustumPlane[2].w = viewProj_._44 - viewProj_._42;

        // Bottom Frustum Plane
        // Add second column of the matrix to the fourth column
        tempFrustumPlane[3].x = viewProj_._14 + viewProj_._12;
        tempFrustumPlane[3].y = viewProj_._24 + viewProj_._22;
        tempFrustumPlane[3].z = viewProj_._34 + viewProj_._32;
        tempFrustumPlane[3].w = viewProj_._44 + viewProj_._42;

        // Near Frustum Plane
        // We could add the third column to the fourth column to get the near plane,
        // but we don't have to do this because the third column IS the near plane
        tempFrustumPlane[4].x = viewProj_._13;
        tempFrustumPlane[4].y = viewProj_._23;
        tempFrustumPlane[4].z = viewProj_._33;
        tempFrustumPlane[4].w = viewProj_._43;

        // Far Frustum Plane
        // Subtract third column of matrix from the fourth column
        tempFrustumPlane[5].x = viewProj_._14 - viewProj_._13;
        tempFrustumPlane[5].y = viewProj_._24 - viewProj_._23;
        tempFrustumPlane[5].z = viewProj_._34 - viewProj_._33;
        tempFrustumPlane[5].w = viewProj_._44 - viewProj_._43;

        // Normalize plane normals (A, B and C (xyz))
        // Also take note that planes face inward
        for (int i = 0; i < 6; ++i)
        {
            float length = sqrt((tempFrustumPlane[i].x * tempFrustumPlane[i].x) + (tempFrustumPlane[i].y * tempFrustumPlane[i].y) + (tempFrustumPlane[i].z * tempFrustumPlane[i].z));
            tempFrustumPlane[i].x /= length;
            tempFrustumPlane[i].y /= length;
            tempFrustumPlane[i].z /= length;
            tempFrustumPlane[i].w /= length;
        }

        return tempFrustumPlane;
    }

};


// struct box for detectt collision
class OPTadaC_Obj_CollisionBox
{
public:

    XMVECTOR minVertex;
    XMVECTOR maxVertex;

    XMFLOAT3 box[8] = {
        XMFLOAT3(-1.0f, -1.0f, -1.0f),
        XMFLOAT3(-1.0f, +1.0f, -1.0f),
        XMFLOAT3(+1.0f, +1.0f, -1.0f),
        XMFLOAT3(+1.0f, -1.0f, -1.0f),
        XMFLOAT3(-1.0f, -1.0f, +1.0f),
        XMFLOAT3(-1.0f, +1.0f, +1.0f),
        XMFLOAT3(+1.0f, +1.0f, +1.0f),
        XMFLOAT3(+1.0f, -1.0f, +1.0f)
    };

    OPTadaE_MeshList_ForResourceManager meshEnum;

    void Create(OPTadaE_MeshList_ForResourceManager meshEnum_, XMMATRIX& worldSpace) {
        meshEnum = meshEnum_;
        OPTadaC_Collision::CalculateAABB(box, worldSpace, minVertex, maxVertex);
    }

    void Update(XMMATRIX& worldSpace) {
        OPTadaC_Collision::CalculateAABB(box, worldSpace, minVertex, maxVertex);
    }
};