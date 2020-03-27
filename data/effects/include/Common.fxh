#ifndef _COMMON_FXH
#define _COMMON_FXH

//-------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------

#define PI 3.141592654f
#define FLIP_TEXTURE_Y true

//-------------------------------------------------------------------------
// Data Structures
//-------------------------------------------------------------------------

RasterizerState DisableCulling
{
    CullMode = None;
};

//-------------------------------------------------------------------------
// Utilities
//-------------------------------------------------------------------------

float get_fog_amount(float3 viewDirection, float fogStart, float fogRange)
{
    return saturate((length(viewDirection) - fogStart) / fogRange);
}

float2 get_corrected_texture_coordinate(float2 textureCoordinate)
{
#if FLIP_TEXTURE_Y
    return float2(textureCoordinate.x, 1.0 - textureCoordinate.y);
#else
    return textureCoordinate;
#endif
}

float get_luminance(float3 color)
{
    // return max(dot(color, float3(0.299f, 0.587f, 0.114f)), 0.0001f);
    return max(dot(color, float3(0.2125f, 0.7154f, 0.0721f)), 0.0001f);
}

float3 gamma_correction(float3 color)
{
    static const float k_invGamma = 0.4545f; // = (1 / 2.2f)
    return pow(color, k_invGamma);
}

//-------------------------------------------------------------------------

// udot
#define DOT_SATURATE_TEMPLATE(type) \
float udot(type lhs, type rhs) { return saturate(dot(lhs, rhs)); }

DOT_SATURATE_TEMPLATE(float2)
DOT_SATURATE_TEMPLATE(float3)
DOT_SATURATE_TEMPLATE(float4)

#endif