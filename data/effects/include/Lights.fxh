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

struct SPOT_LIGHT_DATA // 64 bytes
{
    float4 color;
    float3 direction;
    float radius;
    float3 position;
    float innerAngle;
    float outerAngle;

    float3 _padding;
};

struct LIGHTS_COMMON_PARAMS
{
    float3 normal;
    float3 viewDirection;
    float3 worldPosition;

    float specularPower;
    float4 specularColor;

    float4 ambientColor;
    float4 color;
};

struct LIGHT_CONTRIBUTION_DATA
{
    float4 color;
    float3 normal;
    float3 viewDirection;

    float specularPower;
    float4 specularColor;

    float4 lightColor;
    float4 lightData; // Light Direction(.xyz) + Attenuation(.w)
};

struct LIGHTS_DATA
{
    DIRECTIONAL_LIGHT_DATA dirLights[MAX_LIGHTS_COUNT];
    POINT_LIGHT_DATA pointLights[MAX_LIGHTS_COUNT];
    SPOT_LIGHT_DATA spotlights[MAX_LIGHTS_COUNT];

    uint dirLightsCount;
    uint pointLightsCount;
    uint spotlightsCount;

    uint _padding;
};

//-------------------------------------------------------------------------
// Constant buffers
//-------------------------------------------------------------------------

cbuffer CBufferLights // MAX_LIGHTS_COUNT * (128) + 12 bytes
{
    LIGHTS_DATA lightsData;
}

//-------------------------------------------------------------------------
// Utilities
//-------------------------------------------------------------------------

float3 get_color_contribution(float4 light, float3 color)
{
    // Color (.rgb) * Intensity (.a)
    return light.rgb * light.a * color;
}

float3 get_color_contribution(float4 light, float color)
{
    // Color * Intensity (.a)
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

float3 get_light_contribution(LIGHT_CONTRIBUTION_DATA IN)
{
    float3 lightDirection = IN.lightData.xyz;
    float lightAttenuation = IN.lightData.w;

    float n_dot_l = dot(IN.normal, lightDirection);
    float3 halfVector = normalize(lightDirection + IN.viewDirection);
    float n_dot_h = dot(IN.normal, halfVector);

    float4 lightCoefficients = lit(n_dot_l, n_dot_h, IN.specularPower);
    float3 diffuse = get_color_contribution(IN.lightColor, lightCoefficients.y * IN.color.rgb);
    float3 specular = get_color_contribution(IN.specularColor, min(lightCoefficients.z, IN.color.w));

    return (diffuse + specular) * lightAttenuation;
}

// float3 get_lights_contribution(LIGHTS_DATA lightsData, LIGHTS_COMMON_PARAMS lightsCommonParams)
float3 get_lights_contribution(LIGHTS_COMMON_PARAMS lightsCommonParams)
{
    float3 lightsContribution = (float3)0.f;

    float3 ambient = get_color_contribution(lightsCommonParams.ambientColor, lightsCommonParams.color.rgb);
    lightsContribution += ambient;

    LIGHT_CONTRIBUTION_DATA lightContributionData;
    lightContributionData.normal = lightsCommonParams.normal;
    lightContributionData.color = lightsCommonParams.color;
    lightContributionData.viewDirection = lightsCommonParams.viewDirection;
    lightContributionData.specularColor = lightsCommonParams.specularColor;
    lightContributionData.specularPower = lightsCommonParams.specularPower;

    // Directional lights
    [loop]
    for (uint i = 0; i < lightsData.dirLightsCount; i++)
    {
        lightContributionData.lightColor = lightsData.dirLights[i].color;
        lightContributionData.lightData = float4(lightsData.dirLights[i].direction, 1.0f);
        lightsContribution += get_light_contribution(lightContributionData);
    }

    // Point lights
    [loop]
    for (uint i = 0; i < lightsData.pointLightsCount; i++)
    {
        lightContributionData.lightColor = lightsData.pointLights[i].color;
        lightContributionData.lightData = get_light_data(lightsData.pointLights[i].position, lightsData.pointLights[i].radius, lightsCommonParams.worldPosition);
        lightsContribution += get_light_contribution(lightContributionData);
    }

    // Spotlights
    [loop]
    for (uint i = 0; i < lightsData.spotlightsCount; i++)
    {
        float spotFactor = 0.f;
        float4 lightData = get_light_data(lightsData.spotlights[i].position, lightsData.spotlights[i].radius, lightsCommonParams.worldPosition);
        float lightAngle = dot(lightData.xyz, -lightsData.spotlights[i].direction);

        if (lightAngle > 0.f)
        {
            spotFactor = smoothstep(lightsData.spotlights[i].outerAngle, lightsData.spotlights[i].innerAngle, lightAngle);

            lightContributionData.lightColor = lightsData.spotlights[i].color;
            lightContributionData.lightData = lightData;
            lightsContribution += get_light_contribution(lightContributionData) * spotFactor;
        }
    }

    return lightsContribution;
}

#endif