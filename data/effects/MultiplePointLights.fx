#include "include/Common.fxh"

/************* Resources *************/

struct POINT_LIGHT
{
    float3 position;
    float lightRadius;
    float4 color;
};

#define MAX_LIGHTS_COUNT 4

cbuffer CBufferPerFrame
{
    POINT_LIGHT pointLights[MAX_LIGHTS_COUNT];
    uint lightsCount;
    float4 ambientColor : AMBIENT;
    float3 cameraPosition : CAMERAPOSITION;
}

cbuffer CBufferPerObject
{
    float4x4 wvp : WORLDVIEWPROJECTION;
    float4x4 world : WORLD;

    float4 specularColor : SPECULAR;
    float specularPower : SPECULARPOWER = 25.0f;
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

/************* Data Structures *************/

struct VS_INPUT
{
    float4 objectPosition : POSITION;
    float2 textureCoordinate : TEXCOORD;
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 worldPosition : POSITION;
    float3 normal : NORMAL;
    float2 textureCoordinate : TEXCOORD0;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, wvp);
    OUT.worldPosition = mul(IN.objectPosition, world).xyz;
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.normal = normalize(mul(float4(IN.normal, 0), world).xyz);

    return OUT;
}

/************* Pixel Shader *************/

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    float3 normal = normalize(IN.normal);
    float3 viewDirection = normalize(cameraPosition - IN.worldPosition);
    float4 color = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);
    float3 ambient = get_color_contribution(ambientColor, color.rgb);

    LIGHT_CONTRIBUTION_DATA lightContributionData;
    lightContributionData.color = color;
    lightContributionData.normal = normal;
    lightContributionData.viewDirection = viewDirection;
    lightContributionData.specularColor = specularColor;
    lightContributionData.specularPower = specularPower;

    float3 totalLightContribution = (float3)0;

    [loop]
    for (uint i = 0; i < lightsCount; i++)
    {
        lightContributionData.lightDirection = get_light_data(pointLights[i].position, IN.worldPosition, pointLights[i].lightRadius);
        lightContributionData.lightColor = pointLights[i].color;
        totalLightContribution += get_light_contribution(lightContributionData);
    }

    OUT.rgb = ambient + totalLightContribution;
    OUT.a = 1.0f;

    return OUT;
}

/************* Techniques *************/

technique10 main11
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, pixel_shader()));

        SetRasterizerState(DisableCulling);
    }
}