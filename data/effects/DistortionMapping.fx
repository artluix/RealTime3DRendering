/************* Resources *************/

Texture2D SceneTexture;
Texture2D DistortionMapTexture;

cbuffer CBufferPerObject
{
    float DisplacementScale = 1.f;
};

cbuffer CBufferPerObjectCutout
{
    float4x4 WVP : WORLDVIEWPROJECTION;
};

SamplerState TrilinearSampler
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

/************* Vertex Shader *************/

VS_OUTPUT distortion_vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = IN.position;
    OUT.textureCoordinate = IN.textureCoordinate;

    return OUT;
}

VS_OUTPUT distortion_cutout_vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT= (VS_OUTPUT)0;

    OUT.position = mul(IN.position, WVP);
    OUT.textureCoordinate = IN.textureCoordinate;

    return OUT;
}

/************* Pixel Shader *************/

float4 distortion_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    static const float k_zeroCorrection = 0.5f / 255.0f;

    float2 displacement = DistortionMapTexture.Sample(TrilinearSampler, IN.textureCoordinate).xy;
    if (displacement.x == 0.f && displacement.y == 0.f)
    {
        return SceneTexture.Sample(TrilinearSampler, IN.textureCoordinate);
    }
    else
    {
        displacement -= (0.5f + k_zeroCorrection);
        return SceneTexture.Sample(TrilinearSampler, IN.textureCoordinate + (displacement * DisplacementScale));
    }
}

float4 distortion_cutout_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float2 displacement = DistortionMapTexture.Sample(TrilinearSampler, IN.textureCoordinate).xy;
    return float4(displacement.xy, 0, 1);
}

float4 no_distortion_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    return SceneTexture.Sample(TrilinearSampler, IN.textureCoordinate);
}

/************* Techniques *************/

technique11 distortion
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, distortion_vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, distortion_pixel_shader()));
    }
}

technique11 no_distortion
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, distortion_vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, no_distortion_pixel_shader()));
    }
}


technique11 distortion_cutout
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, distortion_cutout_vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, distortion_cutout_pixel_shader()));
    }
}

