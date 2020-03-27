#ifndef _LIGHTS_DATA_FXH
#define _LIGHTS_DATA_FXH

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

//-------------------------------------------------------------------------
// Common constant buffers
//-------------------------------------------------------------------------

cbuffer CBuffer_PerFrame_LightsData // MAX_LIGHTS_COUNT * (128) + 12 bytes
{
    DIRECTIONAL_LIGHT_DATA DirLights[MAX_LIGHTS_COUNT];
    POINT_LIGHT_DATA PointLights[MAX_LIGHTS_COUNT];
    SPOTLIGHT_DATA Spotlights[MAX_LIGHTS_COUNT];

    uint DirLightsCount;
    uint PointLightsCount;
    uint SpotlightsCount;
}

#endif