#include "include/Common.fxh"

#define MaxBones 60

/************* Resources *************/

cbuffer CBufferPerFrame
{
    float4 AmbientColor = { 1.f, 1.f, 1.f, 0.f };
    float4 LightColor = { 1.f, 1.f, 1.f, 1.f };
    float3 LightPosition = { 0.f, 0.f, 0.f };
    float LightRadius = 10.f;
    float3 CameraPosition;
}

cbuffer CBufferPerObject
{
    float4x4 WVP : WORLDVIEWPROJECTION;
    float4x4 World : WORLD;

    float4 SpecularColor : SPECULAR = { 1.f, 1.f, 1.f, 1.f };
    float SpecularPower : SPECULARPOWER = 25.f;
}

cbuffer CBufferPerSkinning
{
    float4x4 BoneTransforms[MaxBones];
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
    skinTransform += BoneTransforms[IN.boneIndices.x] * IN.boneWeights.x;
    skinTransform += BoneTransforms[IN.boneIndices.y] * IN.boneWeights.y;
    skinTransform += BoneTransforms[IN.boneIndices.z] * IN.boneWeights.z;
    skinTransform += BoneTransforms[IN.boneIndices.w] * IN.boneWeights.w;

    float4 position = mul(IN.objectPosition, skinTransform);
    OUT.position = mul(position, WVP);
    OUT.worldPosition = mul(position, World).xyz;

    float4 normal = mul(float4(IN.normal, 0.f), skinTransform);
    OUT.normal = normalize(mul(normal, World).xyz);

    OUT.textureCoordinate = IN.textureCoordinate;

    float3 lightDirection = LightPosition - OUT.worldPosition;
    OUT.attenuation = saturate(1.f - (length(lightDirection) / LightRadius));

    return OUT;
}

/************* Pixel Shader *************/


float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    float3 lightDirection = normalize(LightPosition - IN.worldPosition);
    float3 viewDirection = normalize(CameraPosition - IN.worldPosition);

    float3 normal = normalize(IN.normal);
    float n_dot_l = dot(normal, lightDirection);
    float3 halfVector = normalize(lightDirection + viewDirection);
    float n_dot_h = dot(normal, halfVector);

    float4 color = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);
    float4 lightCoefficients = lit(n_dot_l, n_dot_h, SpecularPower);

    float3 ambient = get_color_contribution(AmbientColor, color.rgb);
    float3 diffuse = get_color_contribution(LightColor, lightCoefficients.y * color.rgb) * IN.attenuation;
    float3 specular = get_color_contribution(SpecularColor, min(lightCoefficients.z, color.w)) * IN.attenuation;

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