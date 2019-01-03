#include "include/Common.fxh"

// Resources
#define LIGHTS_COUNT 4

cbuffer CBufferPerFrame
{
    POINT_LIGHT pointLights[LIGHTS_COUNT];

    float4 ambientColor : AMBIENT <
        string UIName = "Ambient Light";
        string UIWidget = "Color";
    > = { 1.0f, 1.0f, 1.0f, 0.0f };

    float3 cameraPosition : CAMERAPOSITION <string UIWIdget="None";>;
}

cbuffer CBufferPerObject
{
    float4x4 wvp : WORLDVIEWPROJECTION <string UIWIdget="None";>;
    float4x4 world : WORLD <string UIWIdget="None";>;

    float4 specularColor : SPECULAR <
        string UIName = "Specular Color";
        string UIWidget = "Color";
    > = { 1.0f, 1.0f, 1.0f, 1.0f };

    float specularPower : SPECULARPOWER <
        string UIName = "Specular Power";
        string UIWidget = "slider";
        float UIMin = 1.0;
        float UIMax = 255.0;
        float UIStep = 1.0;
    > = { 25.0f };
}

RasterizerState DisableCulling
{
    CullMode = None;
};

Texture2D ColorTexture <
    string ResourceName="default_color.dds";
    string UIName="Color Texture";
    string ResourceType="2D";
>;

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
    float4 position : SV_Position;
    float3 worldPosition : POSITION;
    float3 normal : NORMAL;
    float2 textureCoordinate : TEXCOORD0;
};

// Vertex Shader
VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, wvp);
    OUT.worldPosition = mul(IN.objectPosition, world);
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.normal = normalize(mul(float4(IN.normal, 0), world).xyz);

    return OUT;
}

// Pixel Shader
float4 pixel_shader(VS_OUTPUT IN, uniform int lightCount) : SV_Target
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

    [unroll]
    for (int i = 0; i < lightCount; i++)
    {
        lightContributionData.lightDirection = get_light_data(pointLights[i].position, IN.worldPosition, pointLights[i].lightRadius);
        lightContributionData.lightColor = pointLights[i].color;
        totalLightContribution += get_light_contribution(lightContributionData);
    }

    OUT.rgb = ambient + totalLightContribution;
    OUT.a = 1.0f;

    return OUT;
}

// Techniques
technique10 Lights1
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, pixel_shader(1)));

        SetRasterizerState(DisableCulling);
    }
}

technique10 Lights2
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, pixel_shader(2)));

        SetRasterizerState(DisableCulling);
    }
}

technique10 Lights3
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, pixel_shader(3)));

        SetRasterizerState(DisableCulling);
    }
}

technique10 Lights4
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, pixel_shader(4)));

        SetRasterizerState(DisableCulling);
    }
}