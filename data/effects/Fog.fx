#include "include/Common.fxh"

// Resources
cbuffer CBufferPerFrame
{
    float4 ambientColor : AMBIENT <
        string UIName = "Ambient Light";
        string UIWidget = "Color";
    > = { 1.0f, 1.0f, 1.0f, 0.0f };

    float4 lightColor : COLOR <
        string Object = "LightColor0";
        string UIName = "Light Color";
        string UIWidget = "Color";
    > = { 1.0f, 1.0f, 1.0f, 1.0f };

    float3 lightDirection : DIRECTION <
        string Object = "DirectionLight0";
        string UIName = "Light Direction";
        string Space = "World";
    > = { 0.0f, 0.0f, -1.0f };

    float3 fogColor <
        string UIName = "Fog Color";
        string UIWIdget = "Color";
    > = { 0.5f, 0.5f, 0.5f };

    float fogStart = { 20.0f };
    float fogRange = { 40.0f };

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

RasterizerState DisableCulling
{
    CullMode = None;
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
    float3 lightDirection : TEXCOORD1;
    float3 viewDirection : TEXCOORD2;
    float fogAmount : TEXCOORD3;
};

// Vertex Shader 
VS_OUTPUT vertex_shader(VS_INPUT IN, uniform bool fogEnabled)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, wvp);
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.normal = normalize(mul(float4(IN.normal, 0), world).xyz);

    float3 worldPosition = mul(IN.objectPosition, world).xyz;

    OUT.lightDirection = normalize(-lightDirection);
    float3 viewDirection = cameraPosition - worldPosition;

    if (fogEnabled)
    {
        OUT.fogAmount = get_fog_amount(viewDirection, fogStart, fogRange);
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
    float4 color = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);
    float3 ambient = get_color_contribution(ambientColor, color.rgb);

    LIGHT_CONTRIBUTION_DATA lightContributionData;
    lightContributionData.color = color;
    lightContributionData.normal = normal;
    lightContributionData.viewDirection = viewDirection;
    lightContributionData.lightDirection = float4(IN.lightDirection, 1.0f);
    lightContributionData.specularColor = specularColor;
    lightContributionData.specularPower = specularPower;
    lightContributionData.lightColor = lightColor;
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
technique10 fogEnabled
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, vertex_shader(true)));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, pixel_shader(true)));

        SetRasterizerState(DisableCulling);
    }
}

technique10 fogDisabled
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, vertex_shader(false)));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, pixel_shader(false)));

        SetRasterizerState(DisableCulling);
    }
}
