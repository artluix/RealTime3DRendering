#ifndef _PHONG_LIGHTING_FXH
#define _PHONG_LIGHTING_FXH

#include "LightsData.fxh"

//-------------------------------------------------------------------------
// Constant buffers
//-------------------------------------------------------------------------

cbuffer CBuffer_PerFrame_PhongLighting // MAX_LIGHTS_COUNT * (128) + 12 bytes
{
    float4 AmbientColor = { 1.0f, 1.0f, 1.0f, 0.0f };
}

cbuffer CBuffer_PerObject_PhongLighting
{
    float4 SpecularColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    float SpecularPower = 25.0f;
}

//-------------------------------------------------------------------------
// Params
//-------------------------------------------------------------------------

struct PHONG_LIGHTING_OBJECT_PARAMS
{
    float4 color;
    float3 normal;
    float3 worldPosition;
    float3 viewDirection;

    float specularPower;
    float4 specularColor;
};

PHONG_LIGHTING_OBJECT_PARAMS phong_lighting_create_object_params(float4 color, float3 normal, float3 worldPosition, float3 viewDirection, float specularPower, float4 specularColor)
{
    PHONG_LIGHTING_OBJECT_PARAMS objectParams;

    objectParams.color = color;
    objectParams.normal = normal;
    objectParams.worldPosition = worldPosition;
    objectParams.viewDirection = viewDirection;
    objectParams.specularPower = specularPower;
    objectParams.specularColor = specularColor;

    return objectParams;
}

PHONG_LIGHTING_OBJECT_PARAMS phong_lighting_create_object_params(float4 color, float3 normal, float3 worldPosition, float3 viewDirection)
{
    return phong_lighting_create_object_params(color, normal, worldPosition, viewDirection, SpecularPower, SpecularColor);
}

//-------------------------------------------------------------------------

struct PHONG_LIGHTING_LIGHT_PARAMS
{
    float4 color;
    float3 direction;
    float attenuation;
};

//-------------------------------------------------------------------------

PHONG_LIGHTING_LIGHT_PARAMS phong_lighting_create_light_params(DIRECTIONAL_LIGHT_DATA lightData)
{
    PHONG_LIGHTING_LIGHT_PARAMS lightParams;

    lightParams.color = lightData.color;
    lightParams.direction = lightData.direction;
    lightParams.attenuation = 1.f;

    return lightParams;
}

PHONG_LIGHTING_LIGHT_PARAMS phong_lighting_create_light_params(POINT_LIGHT_DATA lightData, float3 worldPosition)
{
    PHONG_LIGHTING_LIGHT_PARAMS lightParams;

    float3 lightDirection = lightData.position - worldPosition;
    lightParams.direction = normalize(lightDirection);
    lightParams.attenuation = saturate(1.0f - length(lightDirection) / lightData.radius);
    lightParams.color = lightData.color;

    return lightParams;
}

PHONG_LIGHTING_LIGHT_PARAMS phong_lighting_create_light_params(SPOTLIGHT_DATA lightData, float3 worldPosition)
{
    PHONG_LIGHTING_LIGHT_PARAMS lightParams;

    float3 lightDirection = lightData.position - worldPosition;
    lightParams.direction = normalize(lightDirection);

    lightParams.attenuation = 0.f;

    float lightAngle = dot(lightParams.direction, -lightData.direction);
    if (lightAngle > 0.f)
    {
        float spotFactor = smoothstep(lightData.outerAngle, lightData.innerAngle, lightAngle);
        lightParams.attenuation = saturate(1.0f - length(lightDirection) / lightData.radius) * spotFactor;

        lightParams.color = lightData.color;
    }

    return lightParams;
}

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

//-------------------------------------------------------------------------

float3 get_ambient(float3 color)
{
    return get_color_contribution(AmbientColor, color);
}

float3 get_specular_diffuse(PHONG_LIGHTING_OBJECT_PARAMS objectParams, PHONG_LIGHTING_LIGHT_PARAMS lightParams)
{
    if (lightParams.attenuation == 0.f)
        return float3(0, 0, 0);

    float n_dot_l = dot(objectParams.normal, lightParams.direction);
    float3 halfVector = normalize(lightParams.direction + objectParams.viewDirection);
    float n_dot_h = dot(objectParams.normal, halfVector);

    float4 lightCoefficients = lit(n_dot_l, n_dot_h, objectParams.specularPower);
    float3 diffuse = get_color_contribution(lightParams.color, lightCoefficients.y * objectParams.color.rgb);
    float3 specular = get_color_contribution(objectParams.specularColor, min(lightCoefficients.z, objectParams.color.w));

    return (diffuse + specular) * lightParams.attenuation;
}

//-------------------------------------------------------------------------
// all lights version
//-------------------------------------------------------------------------

float3 get_specular_diffuse(PHONG_LIGHTING_OBJECT_PARAMS objectParams)
{
    float3 specularDiffuse = (float3)0.f;

    PHONG_LIGHTING_LIGHT_PARAMS lightParams;

    // Directional lights
    [loop]
    for (uint i = 0; i < DirLightsCount; i++)
    {
        lightParams = phong_lighting_create_light_params(DirLights[i]);
        specularDiffuse += get_specular_diffuse(objectParams, lightParams);
    }

    // Point lights
    [loop]
    for (i = 0; i < PointLightsCount; i++)
    {
        lightParams = phong_lighting_create_light_params(PointLights[i], objectParams.worldPosition);
        specularDiffuse += get_specular_diffuse(objectParams, lightParams);
    }

    // Spotlights
    [loop]
    for (i = 0; i < SpotlightsCount; i++)
    {
        lightParams = phong_lighting_create_light_params(Spotlights[i], objectParams.worldPosition);
        specularDiffuse += get_specular_diffuse(objectParams, lightParams);
    }

    return specularDiffuse;
}

float3 get_light_contribution(PHONG_LIGHTING_OBJECT_PARAMS objectParams)
{
    return get_ambient(objectParams.color.rgb) + get_specular_diffuse(objectParams);
}

#endif