#include "include/Common.fxh"

/************* Resources *************/

cbuffer CBufferPerObject
{
    float4x4 ViewProjection;
}

Texture2D EquirectangularMap;

SamplerState TrilinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
};

/************* Data Structures *************/

struct VS_INPUT
{
    float4 ObjectPosition : POSITION;
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float3 WorldPosition : POSITION;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.Position = mul(IN.ObjectPosition, ViewProjection);
    OUT.WorldPosition = IN.ObjectPosition.xyz;

    return OUT;
}

/************* Pixel Shader *************/

float2 sample_sphere(float3 v)
{
    static const float2 k_invAtan = float2(0.1591, 0.3183);

    float2 uv = float2(atan2(v.z, v.x), asin(v.y));
    uv *= k_invAtan;
    uv += 0.5;
    return uv;
}

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float2 uv = sample_sphere(normalize(IN.WorldPosition));
    float3 color = EquirectangularMap.Sample(TrilinearSampler, uv).rgb;

    return float4(color, 1.f);
}

/************* Techniques *************/

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
