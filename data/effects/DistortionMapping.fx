/************* Resources *************/

Texture2D SceneTexture;
Texture2D DistortionMapTexture;

cbuffer CBufferPerObject
{
    float displacementScale = 1.f;
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

VS_OUTPUT distortion_map_vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = IN.position;
    OUT.textureCoordinate = IN.textureCoordinate;

    return OUT;
}

/************* Pixel Shader *************/

float4 distortion_map_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float2 displacement = DistortionMapTexture.Sample(TrilinearSampler, IN.textureCoordinate).xy - 0.5f;
    return SceneTexture.Sample(TrilinearSampler, IN.textureCoordinate + (displacement * displacementScale));
}

/************* Techniques *************/

technique11 distortion_map
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, distortion_map_vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, distortion_map_pixel_shader()));
    }
}
