// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //


cbuffer PerApplication : register(b0)
{
    matrix projectionMatrix;
}

cbuffer PerFrame : register(b1)
{
    matrix viewMatrix;
}

cbuffer PerObject : register(b2)
{
    matrix worldMatrix;
}

// VS_IN
struct AppData 
{
    float3 position :     POSITION;

    float3 normal :       NORMAL;
    float2 textureCoord : TEXCOORD;
};

// VS_OUT
struct VertexShaderOutput 
{
    float4 normal :       NORMAL;
    float2 textureCoord : TEXCOORD;

    float4 position :     SV_POSITION;
};


// Default vertex shader
VertexShaderOutput VS_Material_Default(AppData IN)
{
    VertexShaderOutput OUT;

    matrix mvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));
    OUT.position     = mul(mvp, float4(IN.position, 1.0f));

    OUT.normal       = float4(IN.normal, 1.0f);
    OUT.textureCoord = IN.textureCoord;

    return OUT;
}