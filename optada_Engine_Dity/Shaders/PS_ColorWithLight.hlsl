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
    float4 normal :       NORMAL;
    float2 textureCoord : TEXCOORD;
};


float4 PS_Material_Default(PixelShaderInput IN) : SV_TARGET
{
    float4 finalColor = 0;

    finalColor = texture0.Sample(SampleLinear0, IN.textureCoord);


    //PixelShaderInput.normal = normalize(PixelShaderInput.normal);

    //float4 diffuse = ObjTexture.Sample(ObjSamplerState, input.TexCoord);

    //float3 finalColor = float3(0.0f, 0.0f, 0.0f);

    ////Create the vector between light position and pixels position
    //float3 lightToPixelVec = light.pos - input.worldPos;

    ////Find the distance between the light pos and pixel pos
    //float d = length(lightToPixelVec);

    ////Create the ambient light
    //float3 finalAmbient = diffuse * light.ambient;

    ////If pixel is too far, return pixel color with ambient light
    //if (d > light.range)
    //    return float4(finalAmbient, diffuse.a);

    ////Turn lightToPixelVec into a unit length vector describing
    ////the pixels direction from the lights position
    //lightToPixelVec /= d;

    ////Calculate how much light the pixel gets by the angle
    ////in which the light strikes the pixels surface
    //float howMuchLight = dot(lightToPixelVec, input.normal);

    ////If light is striking the front side of the pixel
    //if (howMuchLight > 0.0f)
    //{
    //    //Add light to the finalColor of the pixel
    //    finalColor += howMuchLight * diffuse * light.diffuse;

    //    //Calculate Light's Falloff factor
    //    finalColor /= light.att[0] + (light.att[1] * d) + (light.att[2] * (d * d));
    //}

    ////make sure the values are between 1 and 0, and add the ambient
    //finalColor = saturate(finalColor + finalAmbient);

    ////Return Final Color
    //return float4(finalColor, diffuse.a);


    //clip(finalColor.b - 0.90f);

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
