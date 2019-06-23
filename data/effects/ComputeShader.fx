/************* Compute Shader *************/

RWTexture2D<float4> OutputTexture;

cbuffer CBufferPerFrame
{
    float2 textureSize;
    float blueColor;
};

[numthreads(32, 32, 1)]
void compute_shader(uint3 threadID : SV_DispatchThreadID)
{
	OutputTexture[threadID.xy] = float4((threadID.xy / textureSize), blueColor, 1);
}

technique11 compute
{
    pass p0
    {
        SetVertexShader(NULL);
        SetGeometryShader(NULL);
        SetPixelShader(NULL);
        SetComputeShader(CompileShader(cs_5_0, compute_shader()));
    }
}

/************* Quad Rendering Shaders *************/

Texture2D ColorTexture;

SamplerState TrilinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
    AddressW = WRAP;
};

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

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = IN.position;
    OUT.textureCoordinate = IN.textureCoordinate;

    return OUT;
}

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    return ColorTexture.Sample(TrilinearSampler, IN.textureCoordinate);
}

technique11 render
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
        SetComputeShader(NULL);
    }
}