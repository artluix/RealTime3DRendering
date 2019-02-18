/************* Resources *************/

cbuffer CBufferPerObject
{
    float4x4 colorFilter = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
};

Texture2D SceneTexture;

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

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = IN.position;
    OUT.textureCoordinate = IN.textureCoordinate;

    return OUT;
}

/************* Pixel Shader *************/

float4 grayscale_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    static const float3 k_grayScaleIntensity = { 0.299f, 0.587f, 0.114f };

    float4 color = SceneTexture.Sample(TrilinearSampler, IN.textureCoordinate);
    float intensity = dot(color.rgb, k_grayScaleIntensity);

    return float4(intensity.rrr, color.a);
}

float4 inverse_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 color = SceneTexture.Sample(TrilinearSampler, IN.textureCoordinate);
    return float4(1.0f - color.rgb, color.a);
}

float4 sepia_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    static const float3x3 k_sepiaFilter = {
        0.393f, 0.769f, 0.189f,
        0.349f, 0.686f, 0.168f,
        0.272f, 0.534f, 0.131f,
    };

    float4 color = SceneTexture.Sample(TrilinearSampler, IN.textureCoordinate);
    return float4(mul(k_sepiaFilter, color.rgb), color.a);
}

float4 generic_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 color = SceneTexture.Sample(TrilinearSampler, IN.textureCoordinate);
    return float4(mul(colorFilter, color).rgb, color.a);
}

/************* Techniques *************/

technique11 grayscale_filter
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, grayscale_pixel_shader()));
    }
}

technique11 inverse_filter
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, inverse_pixel_shader()));
    }
}

technique11 sepia_filter
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, sepia_pixel_shader()));
    }
}

technique11 generic_filter
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, generic_pixel_shader()));
    }
}