#ifndef _COMMON_FXH
#define _COMMON_FXH

//-------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------

#define FLIP_TEXTURE_Y true

//-------------------------------------------------------------------------
// Data Structures
//-------------------------------------------------------------------------

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

#endif