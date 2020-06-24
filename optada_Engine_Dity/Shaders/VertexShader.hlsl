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
    float3 position : POSITION;
    float3 color: COLOR;
};

// VS_OUT
struct VertexShaderOutput 
{
    float4 color : COLOR;
    float4 position : SV_POSITION;
};


// Default vertex shader
VertexShaderOutput VS_Material_Default(AppData IN)
{
    VertexShaderOutput OUT;

    matrix mvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));
    OUT.position = mul(mvp, float4(IN.position, 1.0f));
    OUT.color = float4(IN.color, 1.0f);

    return OUT;
}