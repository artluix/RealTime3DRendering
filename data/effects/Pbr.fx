/************* Resources *************/

cbuffer CBufferPerObject
{
    float4x4 WVP;
}

SamplerState TrilinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
};

RasterizerState DisableCulling
{
    CullMode = NONE;
};

/************* Data Structures *************/

struct VS_INPUT
{
    float4 ObjectPosition : POSITION;
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float3 TextureCoordinate : TEXCOORD;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    return OUT;
}

/************* Pixel Shader *************/

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    return float4(0.f, 0.f, 0.f, 1.f);
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
