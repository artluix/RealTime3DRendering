#include "include/Common.fxh"

// Resources
cbuffer CBufferPerFrame
{
    float4 ambientColor : AMBIENT <
        string UIName = "Ambient Light";
        string UIWidget = "Color";
    > = { 1.0f, 1.0f, 1.0f, 1.0f };

    float4 lightColor : COLOR <
        string Object = "LightColor0";
        string UIName = "Light Color";
        string UIWidget = "Color";
    > = { 1.0f, 1.0f, 1.0f, 1.0f };

    float3 lightDirection : DIRECTION <
        string Object = "DirectionalLight0";
        string UIName = "Light Direction";
        string Space = "World";
    > = { 0.0f, 0.0f, -1.0f };

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

Texture2D NormalMap <
    string ResourceName="default_bump_normal.dds";
    string UIName = "Normal Map";
    string ResourceType = "2D";
>;

SamplerState TrilinearSampler
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
    float3 tangent : TANGENT;
};

struct VS_OUTPUT
{
    float4 position: SV_Position;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float2 textureCoordinate : TEXCOORD0;
    float3 lightDirection : TEXCOORD1;
    float3 viewDirection : TEXCOORD2;
};

// Vertex Shader 
VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, wvp);
    OUT.normal = normalize(mul(float4(IN.normal, 0), world).xyz);
    OUT.tangent = normalize(mul(float4(IN.tangent, 0), world).xyz);
    OUT.binormal = cross(OUT.normal, OUT.tangent);
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.lightDirection = normalize(-lightDirection);
    
    float3 worldPosition = mul(IN.objectPosition, world).xyz;
    float3 viewDirection = cameraPosition - worldPosition;
    OUT.viewDirection = normalize(viewDirection);

    return OUT;
}

// Pixel Shader
float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    // Map normal from [0..1] to [-1..1]
    float3 sampledNormal = (2 * NormalMap.Sample(TrilinearSampler, IN.textureCoordinate).xyz) - 1.0;
    float3x3 tbn = float3x3(IN.tangent, IN.binormal, IN.normal);

    // Transform normal to world space
    sampledNormal = mul(sampledNormal, tbn);

    float3 viewDirection = normalize(IN.viewDirection);
    float4 color = ColorTexture.Sample(TrilinearSampler, IN.textureCoordinate);
    float3 ambient = get_color_contribution(ambientColor, color.rgb);

    LIGHT_CONTRIBUTION_DATA lightContributionData;
    lightContributionData.color = color;
    lightContributionData.normal = sampledNormal;
    lightContributionData.viewDirection = viewDirection;
    lightContributionData.lightDirection = float4(IN.lightDirection, 1);
    lightContributionData.specularColor = specularColor;
    lightContributionData.specularPower = specularPower;
    lightContributionData.lightColor = lightColor;
    float3 light_contribution = get_light_contribution(lightContributionData);

    OUT.rgb = ambient + light_contribution;
    OUT.a = 1.0f;
    
    return OUT;
}

// Techniques
technique10 main10
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, pixel_shader()));

        SetRasterizerState(DisableCulling);
    }
}
