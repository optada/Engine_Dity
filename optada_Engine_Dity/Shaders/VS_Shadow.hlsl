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


cbuffer Application : register(b0)
{
    matrix NONE;
}

cbuffer Frame : register(b1)
{
    Light light_mass[100];
    float4 light_param;
    matrix WVPlight;
}

cbuffer Object : register(b2)
{
    matrix WVP;
    matrix World;
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
    float4 Wnormal :      WNORMAL;
    float4 normal :       NORMAL;
    float2 textureCoord : TEXCOORD;
    float4 worldPos :     POSITION;

    float4 position :     SV_POSITION;
};


// Default vertex shader
VertexShaderOutput VS_Material_Default(AppData IN)
{
    VertexShaderOutput OUT;

    //OUT.textureCoord = IN.textureCoord;
    //OUT.normal = float4(IN.normal, 1.0f);
    //OUT.Wnormal = OUT.normal;

    OUT.position = mul(float4(IN.position, 1.0f), WVPlight);
    // Пишем позицию в depthPosition
    OUT.worldPos = OUT.position;

   

    return OUT;
}