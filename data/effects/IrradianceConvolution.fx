#include "include/Common.fxh"

/************* Resources *************/

cbuffer CBufferPerObject
{
    float4x4 ViewProjection;
    float SampleDelta = 0.025;
}

TextureCube EnvironmentMap;

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

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    // The world vector acts as the normal of a tangent surface
    // from the origin, aligned to WorldPosition. Given this normal, calculate all
    // incoming radiance of the environment. The result of this radiance
    // is the radiance of light coming from -Normal direction, which is what
    // we use in the PBR shader to sample irradiance.

    float3 N = normalize(IN.WorldPosition);

    float3 irradiance = (float3)0;

    // tangent space calculation from origin point
    float3 up = float3(0.0, 1.0, 0.0);
    float3 right = cross(up, N);
    up = cross(N, right);

    int samplesCount = 0;
    [loop]
    for (float phi = 0.0; phi < 2 * PI; phi += SampleDelta)
    {
        for (float theta = 0; theta < 0.5 * PI; theta += SampleDelta)
        {
            // spherical to cartesian (in tangent space)
            float3 tangentSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            // tangent space to world
            float3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

            irradiance += EnvironmentMap.Sample(TrilinearSampler, sampleVec).rgb * cos(theta) * sin(theta);
            samplesCount++;
        }
    }

    irradiance *= (PI / float(samplesCount));

    return float4(irradiance, 1.f);
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
