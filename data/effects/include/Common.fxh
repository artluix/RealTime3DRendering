#ifndef _COMMON_FXH
#define _COMMON_FXH

// Constants
#define FLIP_TEXTURE_Y true

// Data Structures

struct LIGHT_CONTRIBUTION_DATA
{
    float4 color;
    float3 normal;
    float3 viewDirection;
    float4 lightColor;
    float4 lightDirection;
    float4 specularColor;
    float specularPower;
};

// Utilities
float2 get_corrected_texture_coordinate(float2 textureCoordinate)
{
#if FLIP_TEXTURE_Y
    return float2(textureCoordinate.x, 1.0 - textureCoordinate.y);
#else
    return textureCoordinate;
#endif
}

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

float4 get_light_data(float3 lightPosition, float3 worldPosition, float lightRadius)
{
    float4 lightData;
    float3 lightDirection = lightPosition - worldPosition;
    lightData.xyz = normalize(lightDirection);
    lightData.w = saturate(1.0f - length(lightDirection) / lightRadius); // attenuation

    return lightData;
}

float3 get_light_contribution(LIGHT_CONTRIBUTION_DATA IN)
{
    float3 lightDirection = IN.lightDirection.xyz;
    float n_dot_l = dot(IN.normal, lightDirection);
    float3 halfVector = normalize(lightDirection + IN.viewDirection);
    float n_dot_h = dot(IN.normal, halfVector);

    float4 lightCoefficients = lit(n_dot_l, n_dot_h, IN.specularPower);
    float3 diffuse = get_color_contribution(IN.lightColor, lightCoefficients.y * IN.color.rgb) * IN.lightDirection.w;
    float3 specular = get_color_contribution(IN.lightColor, min(lightCoefficients.z, IN.color.w)) * IN.lightDirection.w;

    return (diffuse + specular);
}

float get_fog_amount(float3 viewDirection, float fogStart, float fogRange)
{
    return saturate((length(viewDirection) - fogStart) / fogRange);
}

#endif