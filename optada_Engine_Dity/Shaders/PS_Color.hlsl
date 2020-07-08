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
    float4 diffuse = texture0.Sample(SampleLinear0, IN.textureCoord);
    IN.Wnormal = normalize(IN.Wnormal);
    IN.normal = normalize(IN.normal);;

    float3 finalAmbient;
    float3 finalColor = float3(0.0f, 0.0f, 0.0f);

    finalAmbient = diffuse * light_mass[0].ambient; // create ambient
    //finalAmbient = float3(0.0f, 0.0f, 0.0f);

    // for cicle optimization //
    float3 lightToPixelVec;
    float d;
    int i;
    float howMuchLight;
    Light light;
    // for cicle optimization //


    // global light
    finalColor += dot(float4(light_mass[0].dir, 1.0f), IN.normal) * diffuse * light_mass[0].diffuse; // global light


    // point light calculate
    for (i = 1; i < 2; i++) {
        light = light_mass[i];

        lightToPixelVec = light.pos - IN.worldPos; //Create the vector between light position and pixels position    
        d = length(lightToPixelVec); //Find the distance between the light pos and pixel pos       
        if (d > light.range) {
            continue; //If pixel is too far, try found next light
        }

        //Turn lightToPixelVec into a unit length vector describing
        //the pixels direction from the lights position
        lightToPixelVec /= d;

        //Calculate how much light the pixel gets by the angle
        //in which the light strikes the pixels surface
        howMuchLight = dot(lightToPixelVec, IN.Wnormal);

        //If light is striking the front side of the pixel
        if (howMuchLight > 0.0f) {     
            finalColor += howMuchLight * diffuse * light.diffuse; //Add light to the finalColor of the pixel   
            finalColor /= light.att[0] + (light.att[1] * d) + (light.att[2] * (d * d)); //Calculate Light's Falloff factor
        }
    }


    // SpotLight light calculate
    for (i = 2; i < 3; i++) {
        light = light_mass[i];

        lightToPixelVec = light.pos - IN.worldPos; //Create the vector between light position and pixels position    
        d = length(lightToPixelVec); //Find the distance between the light pos and pixel pos       
        if (d > light.range) {
            continue; //If pixel is too far, try found next light
        }

        //Turn lightToPixelVec into a unit length vector describing
        //the pixels direction from the lights position
        lightToPixelVec /= d;

        //Calculate how much light the pixel gets by the angle
        //in which the light strikes the pixels surface
        howMuchLight = dot(lightToPixelVec, IN.Wnormal);

        //If light is striking the front side of the pixel
        if (howMuchLight > 0.0f) {            
            finalColor += diffuse * light.diffuse; //Add light to the finalColor of the pixel           
            finalColor /= (light.att[0] + (light.att[1] * d)) + (light.att[2] * (d * d)); //Calculate Light's Distance Falloff factor          
            finalColor *= pow(max(dot(-lightToPixelVec, light.dir), 0.0f), light.cone); //Calculate falloff from center to edge of pointlight cone
        }
    }



    finalColor = saturate(finalColor + finalAmbient); //make sure the values are between 1 and 0, and add the ambient

    return float4(finalColor, diffuse.a); //Return Final Color
}
