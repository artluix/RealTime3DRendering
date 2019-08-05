#include "include/Common.fxh"

// Resources
cbuffer CBufferPerFrame
{
    float4 ambientColor : AMBIENT;
    float3 cameraPosition : CAMERAPOSITION;

    float4 lightColor : COLOR;
    float3 lightDirection : DIRECTION;
}

cbuffer CBufferPerObject
{
    float4x4 wvp : WORLDVIEWPROJECTION;
    float4x4 world : WORLD;

    float4 specularColor : SPECULAR;
    float specularPower : SPECULARPOWER;
}

Texture2D ColorTexture;
Texture2D NormalMap;
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
