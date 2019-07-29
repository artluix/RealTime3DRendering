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

Texture2D
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
    float2 textureCoordinate : TEXCOORD0;
};

struct PSD_OUTPUT
{
    float4 color : SV_Target0;
    float4 normal : SV_Target1;
    float4 position : SV_Target2;
};

struct VSL_INPUT
{
    float2 positionTexCoord : TEXCOORD0;
    float2 normalTexCoord : TEXCOORD1;
};
struct VSL_OUTPUT
{
    float2 positionTexCoord : TEXCOORD0;
    float2 normalTexCoord : TEXCOORD1;
}

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

VS_OUTPUT vertex_shader_light(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, wvp);
    OUT.worldPosition = mul(IN.objectPosition, world).xyz;
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.normal = normalize(mul(float4(IN.normal, 0), world).xyz);

    return OUT;
}

/************* Pixel Shader *************/

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

PSD_OUTPUT pixel_shader_deferred(VS_OUTPUT IN) : SV_Target
{

}

float4 pixel_shader_light(VSL_OUTPUT IN) : SV_Target
{

}

/************* Techniques *************/

technique10 forward
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, pixel_shader_forward()));

        SetRasterizerState(DisableCulling);
    }
}

technique10 deferred
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, pixel_shader_deferred()));

        SetRasterizerState(DisableCulling);
    }
    pass p1
    {
        SetVertexShader(CompileShader(vs_4_0, vertex_shader_light()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, pixel_shader_light()));

        SetRasterizerState(DisableCulling);
    }
}