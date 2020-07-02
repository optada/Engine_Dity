// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //


// PS_IN
struct PixelShaderInput
{
    float4 color : NORMAL;
};


float4 PS_Material_Default(PixelShaderInput IN) : SV_TARGET
{
    return IN.color;
}
