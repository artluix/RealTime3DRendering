#include "include/Common.fxh"
#include "include/Lights.fxh"

// Resources
cbuffer CBufferPerFrame
{
    float3 CameraPosition;
}

cbuffer CBufferPerObject
{
    float4x4 WVP;
    float4x4 World;
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
    float3 worldPosition : TEXCOORD1;
    float3 viewDirection : TEXCOORD2;
};

// Vertex Shader
VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, WVP);
    OUT.normal = normalize(mul(float4(IN.normal, 0), World).xyz);
    OUT.tangent = normalize(mul(float4(IN.tangent, 0), World).xyz);
    OUT.binormal = cross(OUT.normal, OUT.tangent);
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.worldPosition = mul(IN.objectPosition, World).xyz;
    OUT.viewDirection = normalize(CameraPosition - OUT.worldPosition);

    return OUT;
}

// Pixel Shader
float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    // Map normal from [0..1] to [-1..1]
    float3 sampledNormal = (2 * NormalMap.Sample(TrilinearSampler, IN.textureCoordinate).xyz) - 1.0;
    float3x3 tbn = float3x3(IN.tangent, IN.binormal, IN.normal);

    // Transform normal to World space
    sampledNormal = mul(sampledNormal, tbn);

    float4 color = ColorTexture.Sample(TrilinearSampler, IN.textureCoordinate);

    LIGHT_OBJECT_PARAMS lightObjectParams;
    lightObjectParams.normal = sampledNormal;
    lightObjectParams.viewDirection = normalize(IN.viewDirection);
    lightObjectParams.worldPosition = IN.worldPosition;
    lightObjectParams.color = color;

    OUT.rgb = get_light_contribution(lightObjectParams);
    OUT.a = 1.0f;

    return OUT;
}

// Techniques
technique11 main11
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader()));

        SetRasterizerState(DisableCulling);
    }
}
