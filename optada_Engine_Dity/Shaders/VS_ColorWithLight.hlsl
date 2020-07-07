// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

struct Light
{
    float3 pos;
    float  range;
    float3 dir;
    float  cone;
    float3 att;
    float4 ambient;
    float4 diffuse;
};



cbuffer Global : register(b0)
{
    matrix projectionMatrix;
}

cbuffer Frame : register(b1)
{
    matrix viewMatrix;
}

cbuffer Object : register(b2)
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
    OUT.position = mul(mvp, float4(IN.position, 1.0f));

    OUT.normal = float4(IN.normal, 1.0f);
    OUT.textureCoord = IN.textureCoord;

    return OUT;
}
