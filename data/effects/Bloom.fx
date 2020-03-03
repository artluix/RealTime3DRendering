/************* Resources *************/

Texture2D SceneTexture;
Texture2D BloomTexture;

cbuffer CBufferPerFrame
{
    float BloomThreshold = 0.45f;
    float BloomIntensity = 1.25f;
    float bloomSaturation = 1.0f;
    float SceneIntensity = 1.0f;
    float SceneSaturation = 1.0f;
};

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

/************* Utility Functions *************/

float4 AdjustSaturation(float4 color, float saturation)
{
    static const float3 GrayScaleIntensity = { 0.299f, 0.587f, 0.114f };

    float intensity = dot(color.rgb, GrayScaleIntensity);
    return float4(lerp(intensity.rrr, color.rgb, saturation), color.a);
}

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = IN.position;
    OUT.textureCoordinate = IN.textureCoordinate;

    return OUT;
}

/************* Pixel Shader *************/

float4 bloom_extract_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 color = SceneTexture.Sample(TrilinearSampler, IN.textureCoordinate);
    return saturate((color - BloomThreshold) / (1.0f - BloomThreshold));
    // alternate method
    // static const float3 k_grayScaleIntensity = { 0.299f, 0.587f, 0.114f };
    // float intensity = dot(color.rgb, k_grayScaleIntensity);
    // return (intensity > BloomThreshold ? color :float4(0, 0, 0, 1));
}

float4 bloom_composite_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 sceneColor = SceneTexture.Sample(TrilinearSampler, IN.textureCoordinate);
    float4 bloomColor = BloomTexture.Sample(TrilinearSampler, IN.textureCoordinate);

    sceneColor = AdjustSaturation(sceneColor, SceneSaturation) * SceneIntensity;
    bloomColor = AdjustSaturation(bloomColor, bloomSaturation) * BloomIntensity;

    sceneColor *= (1 - saturate(bloomColor));

    return sceneColor + bloomColor;
}

/************* Techniques *************/

technique11 bloom_extract
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, bloom_extract_pixel_shader()));
    }
}

technique11 bloom_composite
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, bloom_composite_pixel_shader()));
    }
}