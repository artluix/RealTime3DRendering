#ifndef _LIGHTS_FXH
#define _LIGHTS_FXH

//-------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------

#define MAX_LIGHTS_COUNT 4

//-------------------------------------------------------------------------
// Data Structures (must always be packed as 16 byte alignment!)
//-------------------------------------------------------------------------

struct DIRECTIONAL_LIGHT_DATA // 32 bytes
{
    float4 color;
    float3 direction;

    float _padding;
};

struct POINT_LIGHT_DATA // 32 bytes
{
    float4 color;
    float3 position;
    float radius;
};

struct SPOTLIGHT_DATA // 64 bytes
{
    float4 color;
    float3 direction;
    float radius;
    float3 position;
    float innerAngle;
    float outerAngle;

    float3 _padding;
};

struct LIGHTS_DATA
{
    DIRECTIONAL_LIGHT_DATA dirLights[MAX_LIGHTS_COUNT];
    POINT_LIGHT_DATA pointLights[MAX_LIGHTS_COUNT];
    SPOTLIGHT_DATA spotlights[MAX_LIGHTS_COUNT];

    uint dirLightsCount;
    uint pointLightsCount;
    uint spotlightsCount;

    uint _padding;
};

//-------------------------------------------------------------------------
// Constant buffers
//-------------------------------------------------------------------------

cbuffer CBufferLightsPerFrame // MAX_LIGHTS_COUNT * (128) + 12 bytes
{
    LIGHTS_DATA LightsData;

    float4 AmbientColor = { 1.0f, 1.0f, 1.0f, 0.0f };
}

cbuffer CBufferLightsPerObject
{
    float4 SpecularColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    float SpecularPower = 25.0f;
}

//-------------------------------------------------------------------------
// Params
//-------------------------------------------------------------------------

struct LIGHT_PARAMS
{
    // object params
    float4 color;
    float3 normal;
    float3 viewDirection;

    float specularPower;
    float4 specularColor;

    // light params
    float4 lightColor; // diffuseColor
    float4 lightData; // Light Direction(.xyz) + Attenuation(.w)
};

struct LIGHT_OBJECT_PARAMS
{
    float4 color;
    float3 normal;
    float3 worldPosition;
    float3 viewDirection;
};

struct LIGHT_OBJECT_PARAMS_EX
{
    float4 color;
    float3 normal;
    float3 worldPosition;
    float3 viewDirection;

    float specularPower;
    float4 specularColor;
};

//-------------------------------------------------------------------------
// Utilities
//-------------------------------------------------------------------------

float3 get_color_contribution(float4 light, float3 color)
{
    return light.rgb * light.a * color;
}

float3 get_color_contribution(float4 light, float color)
{
    return light.rgb * light.a * color;
}

// Fading lights should be culled on CPU side already
float4 get_light_data(float3 lightPosition, float lightRadius, float3 worldPosition)
{
    float4 lightData;

    float3 lightDirection = lightPosition - worldPosition;
    lightData.xyz = normalize(lightDirection);
    lightData.w = saturate(1.0f - length(lightDirection) / lightRadius); // attenuation

    return lightData;
}

float3 get_ambient(float3 color)
{
    return get_color_contribution(AmbientColor, color);
}

float3 get_specular_diffuse(LIGHT_PARAMS lightParams)
{
    float3 lightDirection = lightParams.lightData.xyz;
    float lightAttenuation = lightParams.lightData.w;

    float n_dot_l = dot(lightParams.normal, lightDirection);
    float3 halfVector = normalize(lightDirection + lightParams.viewDirection);
    float n_dot_h = dot(lightParams.normal, halfVector);

    float4 lightCoefficients = lit(n_dot_l, n_dot_h, lightParams.specularPower);
    float3 diffuse = get_color_contribution(lightParams.lightColor, lightCoefficients.y * lightParams.color.rgb);
    float3 specular = get_color_contribution(lightParams.specularColor, min(lightCoefficients.z, lightParams.color.w));

    return (diffuse + specular) * lightAttenuation;
}

float3 get_light_contribution(LIGHT_PARAMS lightParams)
{
    return get_ambient(lightParams.color.rgb) + get_specular_diffuse(lightParams);
}

//-------------------------------------------------------------------------
// multiple lights versions
//-------------------------------------------------------------------------

float3 get_specular_diffuse(LIGHT_OBJECT_PARAMS_EX lightObjectParamsEx)
{
    float3 specularDiffuse = (float3)0.f;

    LIGHT_PARAMS lightParams;
    lightParams.normal = lightObjectParamsEx.normal;
    lightParams.color = lightObjectParamsEx.color;
    lightParams.viewDirection = lightObjectParamsEx.viewDirection;
    lightParams.specularColor = lightObjectParamsEx.specularColor;
    lightParams.specularPower = lightObjectParamsEx.specularPower;

    // Directional lights
    [loop]
    for (uint i = 0; i < LightsData.dirLightsCount; i++)
    {
        lightParams.lightColor = LightsData.dirLights[i].color;
        lightParams.lightData = float4(LightsData.dirLights[i].direction, 1.0f);
        specularDiffuse += get_specular_diffuse(lightParams);
    }

    // Point lights
    [loop]
    for (i = 0; i < LightsData.pointLightsCount; i++)
    {
        lightParams.lightColor = LightsData.pointLights[i].color;
        lightParams.lightData = get_light_data(LightsData.pointLights[i].position, LightsData.pointLights[i].radius, lightObjectParamsEx.worldPosition);
        specularDiffuse += get_specular_diffuse(lightParams);
    }

    // Spotlights
    [loop]
    for (i = 0; i < LightsData.spotlightsCount; i++)
    {
        float spotFactor = 0.f;
        float4 lightData = get_light_data(LightsData.spotlights[i].position, LightsData.spotlights[i].radius, lightObjectParamsEx.worldPosition);
        float lightAngle = dot(lightData.xyz, -LightsData.spotlights[i].direction);

        if (lightAngle > 0.f)
        {
            spotFactor = smoothstep(LightsData.spotlights[i].outerAngle, LightsData.spotlights[i].innerAngle, lightAngle);

            lightParams.lightColor = LightsData.spotlights[i].color;
            lightParams.lightData = lightData;
            specularDiffuse += get_specular_diffuse(lightParams) * spotFactor;
        }
    }

    return specularDiffuse;
}

float3 get_light_contribution(LIGHT_OBJECT_PARAMS_EX lightObjectParamsEx)
{
    return get_ambient(lightObjectParamsEx.color.rgb) + get_specular_diffuse(lightObjectParamsEx);
}

//-------------------------------------------------------------------------

float3 get_specular_diffuse(LIGHT_OBJECT_PARAMS lightObjectParams)
{
    LIGHT_OBJECT_PARAMS_EX lightObjectParamsEx;

    lightObjectParamsEx.color = lightObjectParams.color;
    lightObjectParamsEx.normal = lightObjectParams.normal;
    lightObjectParamsEx.worldPosition = lightObjectParams.worldPosition;
    lightObjectParamsEx.viewDirection = lightObjectParams.viewDirection;
    lightObjectParamsEx.specularColor = SpecularColor;
    lightObjectParamsEx.specularPower = SpecularPower;

    return get_specular_diffuse(lightObjectParamsEx);
}

float3 get_light_contribution(LIGHT_OBJECT_PARAMS lightObjectParams)
{
    return get_ambient(lightObjectParams.color.rgb) + get_specular_diffuse(lightObjectParams);
}

#endif