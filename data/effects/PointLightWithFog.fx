#include "include/Common.fxh"

// Resources
cbuffer CBufferPerFrame
{
    float4 ambientColor : AMBIENT;
    float3 cameraPosition : CAMERAPOSITION;

    POINT_LIGHT_DATA lightData;

    float3 fogColor = { 0.5f, 0.5f, 0.5f };
    float fogStart = { 20.0f };
    float fogRange = { 40.0f };
}

cbuffer CBufferPerObject
{
    float4x4 wvp : WORLDVIEWPROJECTION;
    float4x4 world : WORLD;

    float4 specularColor : SPECULAR;
    float specularPower : SPECULARPOWER;
}

RasterizerState DisableCulling
{
    CullMode = None;
};

Texture2D ColorTexture;
SamplerState ColorSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

// Data Structures
struct VS_INPUT
{
    float4 objectPosition : POSITION;
    float2 textureCoordinate : TEXCOORD;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position: SV_Position;
    float3 normal : NORMAL;
    float2 textureCoordinate : TEXCOORD0;
    float3 lightDirection : TEXCOORD2;
    float3 viewDirection : TEXCOORD3;
    float attenuation : TEXCOORD4;
    float fogAmount : TEXCOORD5;
};

// Vertex Shader
VS_OUTPUT vertex_shader(VS_INPUT IN, uniform bool fogEnabled)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, wvp);
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.normal = normalize(mul(float4(IN.normal, 0), world).xyz);

    float3 worldPosition = mul(IN.objectPosition, world).xyz;
    OUT.lightDirection = normalize(lightData.position - worldPosition);
    float3 viewDirection = cameraPosition - worldPosition;
    OUT.viewDirection = normalize(viewDirection);
    OUT.attenuation = saturate(1.0f - (length(OUT.lightDirection) / lightData.radius));

    if (fogEnabled)
    {
        OUT.fogAmount = get_fog_amount(viewDirection, fogStart, fogRange);
    }

    return OUT;
}

// Pixel Shader
float4 pixel_shader(VS_OUTPUT IN, uniform bool fogEnabled) : SV_Target
{
    float4 OUT = (float4)0;

    float3 normal = normalize(IN.normal);
    float3 lightDirection = normalize(IN.lightDirection);
    float3 viewDirection = normalize(IN.viewDirection);
    float4 color = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);
    float3 ambient = get_color_contribution(ambientColor, color.rgb);

    LIGHT_CONTRIBUTION_DATA lightContributionData;
    lightContributionData.color = color;
    lightContributionData.normal = normal;
    lightContributionData.viewDirection = viewDirection;
    lightContributionData.lightDirection = float4(lightDirection, 1);
    lightContributionData.specularColor = specularColor;
    lightContributionData.specularPower = specularPower;
    lightContributionData.lightColor = lightData.color;
    float3 light_contribution = get_light_contribution(lightContributionData);

    OUT.rgb = ambient + light_contribution;
    OUT.a = 1.0f;

    if (fogEnabled)
    {
        OUT.rgb = lerp(OUT.rgb, fogColor, IN.fogAmount);
    }

    return OUT;
}

// Techniques
technique11 fogEnabled
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader(true)));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader(true)));

        SetRasterizerState(DisableCulling);
    }
}

technique11 fogDisabled
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader(false)));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader(false)));

        SetRasterizerState(DisableCulling);
    }
}

