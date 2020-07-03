// Created by OPTada // Free for use //
// - - - - - - - - - - - - - - - - - //

Texture2D texture0 : register (t0);

SamplerState SampleLinear0 : register (s0);


// PS_IN
struct PixelShaderInput
{
    float4 normal :       NORMAL;
    float2 textureCoord : TEXCOORD;
};


float4 PS_Material_Default(PixelShaderInput IN) : SV_TARGET
{
    float4 finalColor = 0;

    finalColor = texture0.Sample(SampleLinear0, IN.textureCoord);
    //finalColor += IN.normal;
    //finalColor = textureColor.Sample(SampleLinear, IN.textureCoord);

    //finalColor = tex2D(SampleLinear, IN.textureCoord);

    /*if (finalColor.r == 0.0f && finalColor.g == 0.0f && finalColor.b == 0.0f && finalColor.a == 0.0f) {
        finalColor = float4(0.0f, 1.0f, 0.0f, 0.0f);
    }
    else {
        finalColor = float4(1.0f, 0.0f, 0.0f, 0.0f);
    }*/

    // складываем освещенность пикселя от всех источников света
    //for (int i = 0; i < 2; i++) {
        //finalColor += saturate(dot((float3)vLightDir[i], IN.normal) * vLightColor[i]);
    //}

    //finalColor = textureColor.Load(int3(8192 * textureCoord.x, 4096 * textureCoord.y, 0)); 

    //finalColor.a = 0.0f;
    //PIX = float4(IN.textureCoord, 1.0f, 1.0f);
    // Sample the pixel color from the texture using the sampler at this texture coordinate location. 

    return finalColor;
}
