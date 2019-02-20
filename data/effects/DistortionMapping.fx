/************* Resources *************/

Texture2D SceneTexture;
Texture2D DistortionMapTexture;

cbuffer CBufferPerObject
{
    float displacementScale = 1.f;
};

cbuffer CBufferPerObjectCutout
{
    float4x4 wvp : WORLDVIEWPROJECTION;
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

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = IN.position;
    OUT.textureCoordinate = IN.textureCoordinate;

    return OUT;
}

VS_OUTPUT cutout_vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT= (VS_OUTPUT)0;

    OUT.position = mul(IN.position, wvp);
    OUT.textureCoordinate = IN.textureCoordinate;

    return OUT;
}

/************* Pixel Shader *************/

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    static const float k_zeroCorrection = 0.5f / 255.0f;

    float2 displacement = DistortionMapTexture.Sample(TrilinearSampler, IN.textureCoordinate).xy;
    if (all(displacement))
    {
        return SceneTexture.Sample(TrilinearSampler, IN.textureCoordinate);
    }
    else
    {
        displacement -= (0.5f + k_zeroCorrection);
        return SceneTexture.Sample(TrilinearSampler, IN.textureCoordinate + (displacement * displacementScale));
    }
}

float4 cutout_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float2 displacement = DistortionMapTexture.Sample(TrilinearSampler, IN.textureCoordinate).xy;
    return float4(displacement.xy, 0, 1);
}

/************* Techniques *************/

technique11 distortion
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
    }
}

technique11 distortion_cutout
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, cutout_vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, cutout_pixel_shader()));
    }
}

