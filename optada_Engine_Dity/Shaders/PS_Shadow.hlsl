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

Texture2D texture0 : register (t0);
Texture2D texture1 : register (t1);
Texture2D texture2 : register (t2);
Texture2D texture3 : register (t3);

SamplerState SampleLinear0 : register (s0);
SamplerState SampleLinear1 : register (s1);
SamplerState SampleLinear2 : register (s2);
SamplerState SampleLinear3 : register (s3);


// PS_IN
struct PixelShaderInput
{
    float4 Wnormal :      WNORMAL;
    float4 normal :       NORMAL;
    float2 textureCoord : TEXCOORD;
    float4 worldPos :     POSITION;
};


float4 PS_Material_Default(PixelShaderInput IN) : SV_TARGET
{
    float depthValue = IN.worldPos.z / IN.worldPos.w;

    return float4(depthValue, depthValue, depthValue, 1.0f); //make sure the values are between 1 and 0, and add the ambient
}
