#include "include/Common.fxh"
#include "include/Lights.fxh"

/************* Resources *************/

#define MAX_LIGHTS_COUNT 4

cbuffer CBufferPerFrame
{
    float4 AmbientColor : AMBIENT;
    float3 CameraPosition : CAMERAPOSITION;

    POINT_LIGHT_DATA LightsData[MAX_LIGHTS_COUNT];
    uint LightsCount;
}

cbuffer CBufferPerObject
{
    float4x4 WVP : WORLDVIEWPROJECTION;
    float4x4 World : WORLD;

    float4 SpecularColor : SPECULAR;
    float SpecularPower : SPECULARPOWER = 25.0f;
}

RasterizerState DisableCulling
{
    CullMode = None;
};

Texture2D ColorTexture;
SamplerState ColorSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

/* deferred */

Texture2D ColorBufferTexture;
Texture2D NormalBufferTexture;
Texture2D WorldPositionBufferTexture;
Texture2D DepthBufferTexture;
SamplerState PointSampler
{
    Filter = MIN_MAG_MIP_POINT;
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
    float2 textureCoordinate : TEXCOORD;
    float3 normal : NORMAL;
};

struct PSD_OUTPUT
{
    float4 color : SV_Target;
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
struct PSL_OUTPUT
{
    float4 color : SV_Target;
    float depth : SV_Depth;
};

/************* Shaders *************/

/* Common Pixel Shader Part */

float4 pixel_shader_common(float4 color, float3 normal, float3 worldPosition)
{
    float4 outColor = (float4)0;

    float3 viewDirection = normalize(CameraPosition - worldPosition);
    float3 ambient = get_color_contribution(AmbientColor, color.rgb);

    LIGHT_CONTRIBUTION_DATA lightContributionData;
    lightContributionData.color = color;
    lightContributionData.normal = normal;
    lightContributionData.viewDirection = viewDirection;
    lightContributionData.specularColor = SpecularColor;
    lightContributionData.specularPower = SpecularPower;

    float3 totalLightContribution = (float3)0;

    [loop]
    for (uint i = 0; i < LightsCount; i++)
    {
        lightContributionData.lightData = get_light_data(LightsData[i].position, worldPosition, LightsData[i].radius);
        lightContributionData.lightColor = LightsData[i].color;
        totalLightContribution += get_light_contribution(lightContributionData);
    }

    outColor.rgb = ambient + totalLightContribution;
    outColor.a = 1.0f;

    return outColor;
}

/* Forward */

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, WVP);
    OUT.worldPosition = mul(IN.objectPosition, World).xyz;
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.normal = normalize(mul(float4(IN.normal, 0), World).xyz);

    return OUT;
}

float4 pixel_shader_forward(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    float3 normal = normalize(IN.normal);
    float4 color = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);

    OUT = pixel_shader_common(color, normal, IN.worldPosition);

    return OUT;
}

/* Geometry Pass (uses Vertex Shader from Forward) */

PSD_OUTPUT pixel_shader_geometry(VS_OUTPUT IN)
{
    PSD_OUTPUT OUT = (PSD_OUTPUT)0;

    OUT.color = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);
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

PSL_OUTPUT pixel_shader_light(VSL_OUTPUT IN)
{
    PSL_OUTPUT OUT = (PSL_OUTPUT)0;

    float4 color = ColorBufferTexture.Sample(PointSampler, IN.textureCoordinate);
    float3 normal = NormalBufferTexture.Sample(PointSampler, IN.textureCoordinate).xyz;
    float3 worldPosition = WorldPositionBufferTexture.Sample(PointSampler, IN.textureCoordinate).xyz;

    OUT.color = pixel_shader_common(color, normal, worldPosition);
    OUT.depth = DepthBufferTexture.Sample(PointSampler, IN.textureCoordinate).x;

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
        SetPixelShader(CompileShader(ps_5_0, pixel_shader_geometry()));

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