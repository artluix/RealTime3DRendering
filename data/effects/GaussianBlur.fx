/************* Resources *************/
#define SAMPLE_COUNT 9

cbuffer CBufferPerFrame
{
    float2 sampleOffsets[SAMPLE_COUNT];
    float sampleWeights[SAMPLE_COUNT];
};

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

float4 blur_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 color = (float4)0;

    for (int i = 0; i < SAMPLE_COUNT; i++)
    {
        color += ColorTexture.Sample(TrilinearSampler, IN.textureCoordinate + sampleOffsets[i]) * sampleWeights[i];
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
        SetPixelShader(CompileShader(ps_5_0, blur_pixel_shader()));
    }
}