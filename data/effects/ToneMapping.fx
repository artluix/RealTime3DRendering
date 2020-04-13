#include "include/Common.fxh"

/************* Resources *************/

Texture2D SceneTexture;
Texture2D LuminanceLogTexture;

cbuffer CBufferPerFrame
{
    uint LuminanceSamplingStepsCount;
    uint LuminanceSamplingMipsCount;
}

SamplerState ColorSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

SamplerState LuminanceSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = CLAMP;
    AddressV = CLAMP;
};


/************* Data Structures *************/

struct VS_INPUT
{
    float4 position : POSITION;
    float2 textureCoordinate : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 textureCoordinate : TEXCOORD;
};

/************* Shaders *************/

// Common

VS_OUTPUT vs_passthrough(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = IN.position;
    OUT.textureCoordinate = IN.textureCoordinate;

    return OUT;
}

// 1. Pass - Calc log of luminance of the screen point

float ps_calc_luminance_log(VS_OUTPUT IN) : SV_Target
{
    float3 color = SceneTexture.Sample(ColorSampler, IN.textureCoordinate).rgb;
    return log(get_luminance(color));
}

// 2. Pass - calculate average luminance, apply exposure, apply tone mapping operator and optional gamma correction

float4 ps_tone_mapping(VS_OUTPUT IN, uniform bool gammaCorrectionEnabled) : SV_Target
{
    float lumTotal = 0.f;

    uint lumSamplingMipStartIdx = LuminanceSamplingMipsCount - LuminanceSamplingStepsCount;
    [loop]
    for (uint i = lumSamplingMipStartIdx; i < LuminanceSamplingStepsCount; i++)
    {
        lumTotal += LuminanceLogTexture.SampleLevel(LuminanceSampler, IN.textureCoordinate, i).x;
    }

    float lumAvg = exp(lumTotal / LuminanceSamplingStepsCount);

    float3 color = SceneTexture.Sample(ColorSampler, IN.textureCoordinate).rgb;
    float lum = get_luminance(color);

    float exposure = lum / lumAvg;

    color *= exposure;

    // reinhard operator
    color = tonemap_reinhard(color);

    // gamma correction (if needed)
    if (gammaCorrectionEnabled)
    {
        color = gamma_correction(color);
    }

    return float4(color, 1.f);
}

technique11 calc_luminance_log
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vs_passthrough()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, ps_calc_luminance_log()));
    }
}

technique11 tone_mapping
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vs_passthrough()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, ps_tone_mapping(false)));
    }
}

technique11 tone_mapping_gamma
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vs_passthrough()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, ps_tone_mapping(true)));
    }
}