static const float3 k_grayScaleIntensity = { 0.299f, 0.587f, 0.114f };

/************* Resources *************/

Texture2D ColorTexture;

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
    float4 color = ColorTexture.Sample(TrilinearSampler, IN.textureCoordinate);
    float intensity = dot(color.rgb, k_grayScaleIntensity);

    return float4(intensity.rrr, color.a);
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