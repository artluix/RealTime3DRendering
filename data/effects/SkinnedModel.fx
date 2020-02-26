#include "include/Common.fxh"
#include "include/Lights.fxh"

#define MaxBones 60

/************* Resources *************/

cbuffer CBufferPerFrame
{
    float3 CameraPosition;

    POINT_LIGHT_DATA LightData; // TODO: remove
}

cbuffer CBufferPerObject
{
    float4x4 WVP;
    float4x4 World;
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
    float3 viewDirection : TEXCOORD2;
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

    OUT.viewDirection = normalize(CameraPosition - OUT.worldPosition);

    float4 normal = mul(float4(IN.normal, 0.f), skinTransform);
    OUT.normal = normalize(mul(normal, World).xyz);

    OUT.textureCoordinate = IN.textureCoordinate;

    return OUT;
}

/************* Pixel Shader *************/


float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    float4 color = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);

    LIGHT_OBJECT_PARAMS lightObjectParams;
    lightObjectParams.normal = normalize(IN.normal);
    lightObjectParams.viewDirection = normalize(IN.viewDirection);
    lightObjectParams.worldPosition = IN.worldPosition;
    lightObjectParams.color = color;

    OUT.rgb = get_light_contribution(lightObjectParams);
    OUT.a = 1.0f;

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