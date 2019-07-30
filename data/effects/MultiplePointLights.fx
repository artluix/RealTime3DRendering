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

Texture2D ModelTexture;
SamplerState ColorSampler
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

Texture2D ColorTexture;
Texture2D NormalTexture;
Texture2D WorldPositionTexture;
SamplerState PointSampler
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
    float2 textureCoordinate : TEXCOORD;
};

struct PSD_OUTPUT
{
    float4 color : SV_Target0;
    float4 normal : SV_Target1;
    float4 worldPosition : SV_Target2;
};

struct VSL_INPUT
{
    float4 position : POSITION;
    float2 textureCoordinate : TEXCOORD;
};
struct VSL_OUTPUT
{
    float4 position : SV_Position;
    float2 textureCoordinate : TEXCOORD;
};

/************* Shaders *************/

/* Forward */

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, wvp);
    OUT.worldPosition = mul(IN.objectPosition, world).xyz;
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.normal = normalize(mul(float4(IN.normal, 0), world).xyz);

    return OUT;
}

float4 pixel_shader_forward(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    float3 normal = normalize(IN.normal);
    float3 viewDirection = normalize(cameraPosition - IN.worldPosition);
    float4 color = ModelTexture.Sample(ColorSampler, IN.textureCoordinate);
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

/* Geometry Pass (uses Vertex Shader from Forward) */

PSD_OUTPUT geometry_pixel_shader(VS_OUTPUT IN)
{
    PSD_OUTPUT OUT = (PSD_OUTPUT)0;

    OUT.color = ModelTexture.Sample(ColorSampler, IN.textureCoordinate);
    OUT.normal = float4(normalize(IN.normal), 1.f);
    OUT.worldPosition = float4(IN.worldPosition, 1.f);

    return OUT;
}

/* Light Pass of deferred */

VSL_OUTPUT vertex_shader_light(VSL_INPUT IN)
{
    VSL_OUTPUT OUT = (VSL_OUTPUT)0;

    OUT.position = IN.position;
    OUT.textureCoordinate = IN.textureCoordinate;

    return OUT;
}

float4 pixel_shader_light(VSL_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    float3 normal = NormalTexture.Sample(PointSampler, IN.textureCoordinate).xyz;
    float3 worldPosition = WorldPositionTexture.Sample(PointSampler, IN.textureCoordinate).xyz;
    float4 color = ColorTexture.Sample(PointSampler, IN.textureCoordinate);

    float3 viewDirection = normalize(cameraPosition - worldPosition);
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
        lightContributionData.lightDirection = get_light_data(pointLights[i].position, worldPosition, pointLights[i].lightRadius);
        lightContributionData.lightColor = pointLights[i].color;
        totalLightContribution += get_light_contribution(lightContributionData);
    }

    OUT.rgb = ambient + totalLightContribution;
    OUT.a = 1.0f;

    return OUT;
}

/************* Techniques *************/

technique11 forward
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader_forward()));

        SetRasterizerState(DisableCulling);
    }
}

technique11 deferred
{
    pass geometry
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, geometry_pixel_shader()));

        SetRasterizerState(DisableCulling);
    }
    pass light
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader_light()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader_light()));

        SetRasterizerState(DisableCulling);
    }
}