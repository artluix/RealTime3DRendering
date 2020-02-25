#include "include/Common.fxh"
#include "include/Lights.fxh"

// Resources
cbuffer CBufferPerFrame
{
    float4 AmbientColor : AMBIENT;

    DIRECTIONAL_LIGHT_DATA LightData;
}

cbuffer CBufferPerObject
{
    float4x4 WVP : WORLDVIEWPROJECTION;
    float4x4 World : WORLD;
}

RasterizerState DisableCulling
{
    CullMode = None;
};

Texture2D ColorTexture;
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
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position: SV_Position;
    float3 normal : NORMAL;
    float2 textureCoordinate : TEXCOORD0;
    float3 lightDirection : TEXCOORD1;
};

// Vertex Shader
VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, WVP);
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.normal = normalize(mul(float4(IN.normal, 0), World).xyz);
    OUT.lightDirection = normalize(-LightData.direction);

    return OUT;
}

// Pixel Shader
float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    float3 normal = normalize(IN.normal);
    float3 lightDirection = normalize(IN.lightDirection);
    float n_dot_l = dot(lightDirection, normal);

    float4 color = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);
    float3 ambient = AmbientColor.rgb * AmbientColor.a * color.rgb;

    float3 diffuse = (float3)0;

    if (n_dot_l > 0)
    {
        diffuse = LightData.color.rgb * LightData.color.a * n_dot_l * color.rgb;
    }

    OUT.rgb = ambient + diffuse;
    OUT.a = color.a;

    return OUT;
}

// Techniques
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
