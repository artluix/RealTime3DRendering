// Resources

#define FLIP_TEXTURE_Y true

cbuffer CBufferPerFrame
{
    float4 ambientColor : AMBIENT <
        string UIName = "Ambient Light";
        string UIWidget = "Color";
    > = { 1.0f, 1.0f, 1.0f, 1.0f };
}

cbuffer CBufferPerObject
{
    float4x4 wvp : WORLDVIEWPROJECTION <string UIWIdget="None";>;
}

RasterizerState DisableCulling
{
    CullMode = None;
};

Texture2D ColorTexture <
    string ResourceName="default_color.dds";
    string UIName="Color Texture";
    string ResourceType="2D";
>;

SamplerState ColorSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

// Data Structures 
struct VS_INPUT
{
    float4 objectPosition : POSITION;
    float2 textureCoordinate : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position: SV_Position;
    float2 textureCoordinate : TEXCOORD;
};

// Utilities 
float2 get_corrected_texture_coordinate(float2 textureCoordinate)
{
#if FLIP_TEXTURE_Y
    return float2(textureCoordinate.x, 1.0 - textureCoordinate.y);
#else
    return textureCoordinate; 
#endif
}

// Vertex Shader 
VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, wvp);
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);

    return OUT;
}

// Pixel Shader
float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    OUT = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);
    OUT.rgb *= ambientColor.rgb * ambientColor.a; // Color (.rgb) * Intensity (.a)

    return OUT;
}

// Techniques
technique10 main10
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, pixel_shader()));

        SetRasterizerState(DisableCulling);
    }
}
