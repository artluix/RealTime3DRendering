/************* Resources *************/

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

/************* Vertex Shaders *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = IN.position;
    OUT.textureCoordinate = IN.textureCoordinate;

    return OUT;
}

/************* Pixel Shaders *************/

float4 default_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 color = SceneTexture.Sample(TrilinearSampler, IN.textureCoordinate);
    return color;
}

float4 reinhard_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 color = SceneTexture.Sample(TrilinearSampler, IN.textureCoordinate);
    color.rgb /= (color.rgb + float3(1.f, 1.f, 1.f));
    return color;
}

/************* Techniques *************/

technique11 no_tone_mapping // default
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, default_pixel_shader()));
    }
}

technique11 tone_mapping_reinhard
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, reinhard_pixel_shader()));
    }
}