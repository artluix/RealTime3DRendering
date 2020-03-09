/************* Resources *************/

Texture2D SceneTexture;

cbuffer CBufferPerObject
{
    float Exposure = 0.5f;
}

SamplerState TrilinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
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

/************* Vertex Shaders *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = IN.position;
    OUT.textureCoordinate = IN.textureCoordinate;

    return OUT;
}

/************* Pixel Shaders *************/

float4 default_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 color = SceneTexture.Sample(TrilinearSampler, IN.textureCoordinate);
    return color;
}

static const float3 k_colorIntensity = { 0.299f, 0.587f, 0.114f };
// static const float k_invGamma = 0.4545f; // = (1 / 2.2f)

float4 reinhard_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float3 color = SceneTexture.Sample(TrilinearSampler, IN.textureCoordinate).rgb;
    color /= (color + 1.f);
    return float4(color, 1.f);
}

float4 exposure_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float3 color = SceneTexture.Sample(TrilinearSampler, IN.textureCoordinate).rgb;
    color = 1.f - exp(-color * Exposure);
    return float4(color, 1.f);
}

/************* Techniques *************/

technique11 no_tone_mapping // default
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, default_pixel_shader()));
    }
}

technique11 tone_mapping_reinhard
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, reinhard_pixel_shader()));
    }
}

technique11 tone_mapping_gamma
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, exposure_pixel_shader()));
    }
}