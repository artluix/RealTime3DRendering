#ifndef _PBR_LIGHTING_FXH
#define _PBR_LIGHTING_FXH

#include "LightsData.fxh"

//-------------------------------------------------------------------------
// Constant buffers
//-------------------------------------------------------------------------

cbuffer CBuffer_PerFrame_PbrLighting
{
    float3 AmbientColor = { 0.3, 0.3, 0.3 };
}

//-------------------------------------------------------------------------
// Params
//-------------------------------------------------------------------------

struct PBR_LIGHTING_OBJECT_PARAMS
{
    float3 albedo;
    float metallic;
    float roughness;
    float ao;

    float3 normal;
    float3 viewDirection;
    float3 worldPosition;
};

struct PBR_LIGHTING_LIGHT_PARAMS
{
    float3 color;
    float3 direction;
    float attenuation;
};

struct PBR_LIGHTING_CACHED_PARAMS
{
    float n_dot_v;
};

//-------------------------------------------------------------------------

PBR_LIGHTING_LIGHT_PARAMS pbr_lighting_create_light_params(DIRECTIONAL_LIGHT_DATA lightData)
{
    PBR_LIGHTING_LIGHT_PARAMS lightParams;

    lightParams.direction = lightData.direction;
    lightParams.attenuation = 1.f;
    lightParams.color = lightData.color.rgb * lightData.color.w;

    return lightParams;
}

PBR_LIGHTING_LIGHT_PARAMS pbr_lighting_create_light_params(POINT_LIGHT_DATA lightData, float3 worldPosition)
{
    PBR_LIGHTING_LIGHT_PARAMS lightParams;

    float3 lightDirection = lightData.position - worldPosition;
    float distance = length(lightDirection);

    lightParams.direction = normalize(lightDirection);
    lightParams.attenuation = 1.f / (distance * distance);
    lightParams.color = lightData.color.rgb * lightData.color.w;

    return lightParams;
}

PBR_LIGHTING_LIGHT_PARAMS pbr_lighting_create_light_params(SPOTLIGHT_DATA lightData, float3 worldPosition)
{
    PBR_LIGHTING_LIGHT_PARAMS lightParams;

    float3 lightDirection = lightData.position - worldPosition;
    lightParams.direction = normalize(lightDirection);

    lightParams.attenuation = 0.f;

    float lightAngle = dot(lightParams.direction, -lightData.direction);
    if (lightAngle > 0.f)
    {
        float distance = length(lightDirection);
        float spotFactor = smoothstep(lightData.outerAngle, lightData.innerAngle, lightAngle);
        lightParams.attenuation = spotFactor / (distance * distance);

        lightParams.color = lightData.color.rgb * lightData.color.w;
    }

    return lightParams;
}

//-------------------------------------------------------------------------
// Utilities
//-------------------------------------------------------------------------

float3 fresnel_schlick(float cosTheta, float3 f0)
{
    return f0 + (1.0 - f0) * pow(1.0 - cosTheta, 5.0);
}

// float distribution_ggx(float3 N, float3 H, float roughness)
float distribution_ggx(float N_dot_H, float roughness)
{
    float a = roughness * roughness;
    float a_sq = a * a;

    // float N_dot_H = udot(N, H);
    float N_dot_H_sq = N_dot_H * N_dot_H;

    float num = a_sq;
    float denom = (N_dot_H_sq * (a_sq - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float geometry_schlick_ggx(float N_dot_V, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = N_dot_V;
    float denom = N_dot_V * (1.0 - k) + k;

    return num / denom;
}

// float geometry_smith(float3 N, float3 V, float3 L, float roughness)
float geometry_smith(float N_dot_V, float N_dot_L, float roughness)
{
    // float N_dot_V = udot(N, V);
    // float N_dot_L = udot(N, L);
    float ggx2 = geometry_schlick_ggx(N_dot_V, roughness);
    float ggx1 = geometry_schlick_ggx(N_dot_L, roughness);

    return ggx1 * ggx2;
}

//-------------------------------------------------------------------------
// Main functions
//-------------------------------------------------------------------------

float3 get_ambient(float3 albedo, float ao)
{
    return AmbientColor * albedo * ao;
}

float3 get_specular_diffuse(PBR_LIGHTING_OBJECT_PARAMS objectParams, PBR_LIGHTING_LIGHT_PARAMS lightParams, PBR_LIGHTING_CACHED_PARAMS cachedParams)
{
    if (lightParams.attenuation == 0.f)
        return float3(0, 0, 0);

    float3 N = objectParams.normal;
    float3 V = objectParams.viewDirection;
    float N_dot_V = cachedParams.n_dot_v;

    float3 L = lightParams.direction;
    float3 H = normalize(V + L);

    float N_dot_H = udot(N, H);
    float N_dot_L = udot(N, L);

    float3 radiance = lightParams.color * lightParams.attenuation;

    float3 F0 = float3(0.04, 0.04, 0.04); // for non-metallic surfaces it's constant
    F0 = lerp(F0, objectParams.albedo, objectParams.metallic);
    float3 F = fresnel_schlick(udot(H, V), F0);

    float NDF = distribution_ggx(N_dot_H, objectParams.roughness);
    float G = geometry_smith(N_dot_V, N_dot_L, objectParams.roughness);

    float3 num = NDF * F * G;
    float denom = 4.0 * N_dot_V * N_dot_L;
    float3 specular = num / max(denom, 0.001);

    float3 kS = F;
    float3 kD = float3(1.0, 1.0, 1.0) - kS;
    kD *= (1.0 - objectParams.metallic);

    float3 color = (kD * objectParams.albedo / PI + specular) * radiance * N_dot_L;
    return color;
}

float3 get_light_contribution(PBR_LIGHTING_OBJECT_PARAMS objectParams)
{
    float3 N = normalize(objectParams.normal);
    float3 V = normalize(objectParams.viewDirection);

    PBR_LIGHTING_CACHED_PARAMS cachedParams;
    cachedParams.n_dot_v = udot(N, V);

    float3 L0 = (float3)0;

    PBR_LIGHTING_LIGHT_PARAMS lightParams;

    // Directional lights
    [loop]
    for (uint i = 0; i < DirLightsCount; i++)
    {
        lightParams = pbr_lighting_create_light_params(DirLights[i]);
        L0 += get_specular_diffuse(objectParams, lightParams, cachedParams);
    }

    // Point lights
    [loop]
    for (i = 0; i < PointLightsCount; i++)
    {
        lightParams = pbr_lighting_create_light_params(PointLights[i], objectParams.worldPosition);
        L0 += get_specular_diffuse(objectParams, lightParams, cachedParams);
    }

    // Spotlights
    [loop]
    for (i = 0; i < SpotlightsCount; i++)
    {
        lightParams = pbr_lighting_create_light_params(Spotlights[i], objectParams.worldPosition);
        L0 += get_specular_diffuse(objectParams, lightParams, cachedParams);
    }

    return get_ambient(objectParams.albedo, objectParams.ao) + L0;
}

#endif