/************* Resources *************/

#define SAMPLE_COUNT 9

cbuffer CBufferPerFrame
{
    float2 SampleOffsets[SAMPLE_COUNT];
    float SampleWeights[SAMPLE_COUNT];
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

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 color = (float4)0;

    for (int i = 0; i < SAMPLE_COUNT; i++)
    {
        color += SceneTexture.Sample(TrilinearSampler, IN.textureCoordinate + SampleOffsets[i]) * SampleWeights[i];
    }

    return color;
}

/************* Techniques *************/

technique11 blur
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
    }
}