#include "include/Common.fxh"
#include "include/Lights.fxh"

// Resources
cbuffer CBufferPerFrame
{
    float4 AmbientColor : AMBIENT;
    float3 CameraPosition : CAMERAPOSITION;

    POINT_LIGHT_DATA LightData;

    float3 FogColor = { 0.5f, 0.5f, 0.5f };
    float FogStart = { 20.0f };
    float FogRange = { 40.0f };
}

cbuffer CBufferPerObject
{
    float4x4 WVP : WORLDVIEWPROJECTION;
    float4x4 World : WORLD;

    float4 SpecularColor : SPECULAR;
    float SpecularPower : SPECULARPOWER;
}

Texture2D ColorTexture;
Texture2D TransparencyMap;

SamplerState Sampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

RasterizerState DisableCulling
{
    CullMode = None;
};

BlendState EnableAlphaBlending
{
    BlendEnable[0] = True;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
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
    float4 lightDirection : TEXCOORD1;
    float3 viewDirection : TEXCOORD2;
    float fogAmount : TEXCOORD3;
};

// Vertex Shader
VS_OUTPUT vertex_shader(VS_INPUT IN, uniform bool fogEnabled)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, WVP);
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.normal = normalize(mul(float4(IN.normal, 0), World).xyz);

    float3 worldPosition = mul(IN.objectPosition, World).xyz;

    OUT.lightDirection = get_light_data(LightData.position, LightData.radius, worldPosition);
    float3 viewDirection = CameraPosition - worldPosition;

    if (fogEnabled)
    {
        OUT.fogAmount = get_fog_amount(viewDirection, FogStart, FogRange);
    }

    OUT.viewDirection = normalize(viewDirection);

    return OUT;
}

// Pixel Shader
float4 pixel_shader(VS_OUTPUT IN, uniform bool fogEnabled) : SV_Target
{
    float4 OUT = (float4)0;

    float3 normal = normalize(IN.normal);
    float3 viewDirection = normalize(IN.viewDirection);
    float4 color = ColorTexture.Sample(Sampler, IN.textureCoordinate);
    float3 ambient = get_color_contribution(AmbientColor, color.rgb);

    LIGHT_CONTRIBUTION_DATA lightContributionData;
    lightContributionData.color = color;
    lightContributionData.normal = normal;
    lightContributionData.viewDirection = viewDirection;
    lightContributionData.lightData = IN.lightDirection;
    lightContributionData.specularColor = SpecularColor;
    lightContributionData.specularPower = SpecularPower;
    lightContributionData.lightColor = LightData.color;
    float3 light_contribution = get_light_contribution(lightContributionData);

    OUT.rgb = ambient + light_contribution;
    OUT.a = TransparencyMap.Sample(Sampler, IN.textureCoordinate).a;

    if (fogEnabled)
    {
        OUT.rgb = lerp(OUT.rgb, FogColor, IN.fogAmount);
    }

    return OUT;
}

// Techniques
technique11 alphaBlendingWithFog
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader(true)));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader(true)));

        SetRasterizerState(DisableCulling);
        // SetBlendState(EnableAlphaBlending, (float4)0, 0xFFFFFFFF); not supported by Effects11
    }
}

technique11 alphaBlendingWithoutFog
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader(false)));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader(false)));

        SetRasterizerState(DisableCulling);
        // SetBlendState(EnableAlphaBlending, (float4)0, 0xFFFFFFFF); not supported by Effects11
    }
}

