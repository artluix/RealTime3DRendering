#include "include/Common.fxh"

#define MaxBones 60

/************* Resources *************/

cbuffer CBufferPerFrame
{
    float4 ambientColor = { 1.f, 1.f, 1.f, 0.f };
    float4 lightColor = { 1.f, 1.f, 1.f, 1.f };
    float3 lightPosition = { 0.f, 0.f, 0.f };
    float lightRadius = 10.f;
    float3 cameraPosition;
}

cbuffer CBufferPerObject
{
    float4x4 wvp : WORLDVIEWPROJECTION;
    float4x4 world : WORLD;

    float4 specularColor : SPECULAR = { 1.f, 1.f, 1.f, 1.f };
    float specularPower : SPECULARPOWER = 25.f;
}

cbuffer CBufferPerSkinning
{
    float4x4 boneTransforms[MaxBones];
}

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
    uint4 boneIndices : BONEINDICES;
    float4 boneWeights : WEIGHTS;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 textureCoordinate : TEXCOORD0;
    float3 worldPosition : TEXCOORD1;
    float attenuation : TEXCOORD2;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    float4x4 skinTransform = (float4x4)0;
    skinTransform += boneTransforms[IN.boneIndices.x] * IN.boneWeights.x;
    skinTransform += boneTransforms[IN.boneIndices.y] * IN.boneWeights.y;
    skinTransform += boneTransforms[IN.boneIndices.z] * IN.boneWeights.z;
    skinTransform += boneTransforms[IN.boneIndices.w] * IN.boneWeights.w;

    float4 position = mul(IN.objectPosition, skinTransform);
    OUT.position = mul(position, wvp);
    OUT.worldPosition = mul(position, world).xyz;

    float4 normal = mul(float4(IN.normal, 0.f), skinTransform);
    OUT.normal = normalize(mul(normal, world).xyz);

    OUT.textureCoordinate = IN.textureCoordinate;

    float3 lightDirection = lightPosition - OUT.worldPosition;
    OUT.attenuation = saturate(1.f - (length(lightDirection) / lightRadius));

    return OUT;
}

/************* Pixel Shader *************/


float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    float3 lightDirection = normalize(lightPosition - IN.worldPosition);
    float3 viewDirection = normalize(cameraPosition - IN.worldPosition);

    float3 normal = normalize(IN.normal);
    float n_dot_l = dot(normal, lightDirection);
    float3 halfVector = normalize(lightDirection + viewDirection);
    float n_dot_h = dot(normal, halfVector);

    float4 color = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);
    float4 lightCoefficients = lit(n_dot_l, n_dot_h, specularPower);

    float3 ambient = get_color_contribution(ambientColor, color.rgb);
    float3 diffuse = get_color_contribution(lightColor, lightCoefficients.y * color.rgb) * IN.attenuation;
    float3 specular = get_color_contribution(specularColor, min(lightCoefficients.z, color.w)) * IN.attenuation;

    OUT.rgb = ambient + diffuse + specular;
    OUT.a = 1.f;

    return OUT;
}

/************* Techniques *************/

technique11 main11
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
    }
}