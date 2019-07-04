#include "include/Common.fxh"

// Resources
cbuffer CBufferPerFrame
{
    float4 ambientColor : AMBIENT;
    float4 lightColor : COLOR;
    float3 lightPosition : POSITION;
    float lightRadius = 10.0f;

    float3 cameraPosition : CAMERAPOSITION;
}

cbuffer CBufferPerObject
{
    float4x4 wvp : WORLDVIEWPROJECTION;
    float4x4 world : WORLD;

    float4 specularColor : SPECULAR;
    float specularPower : SPECULARPOWER = 25.0f;
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
    float3 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position: SV_Position;
    float3 normal : NORMAL;
    float2 textureCoordinate : TEXCOORD0;
    float3 worldPosition : TEXCOORD1;
    float attenuation : TEXCOORD2;
};

// Vertex Shader
VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, wvp);
    OUT.worldPosition = mul(IN.objectPosition, world).xyz;
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.normal = normalize(mul(float4(IN.normal, 0), world).xyz);

    float3 lightDirection = lightPosition - OUT.worldPosition;
    OUT.attenuation = saturate(1.0f - (length(lightDirection) / lightRadius));

    return OUT;
}

// Pixel Shader
float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    float3 lightDirection = normalize(lightPosition - IN.worldPosition);
    float3 viewDirection = normalize(cameraPosition - IN.worldPosition);

    float3 normal = normalize(IN.normal);
    float n_dot_l = dot(normal, lightDirection);
    float3 halfVector = normalize(lightDirection + viewDirection);
    float n_dot_h = dot(normal, halfVector);

    float4 color = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);
    float4 lightCoefficients = lit(n_dot_l, n_dot_h, specularPower);

    float3 ambient = get_color_contribution(ambientColor, color.rgb);
    float3 diffuse = get_color_contribution(lightColor, lightCoefficients.y * color.rgb) * IN.attenuation;
    float3 specular = get_color_contribution(specularColor, min(lightCoefficients.z, color.w)) * IN.attenuation;

    OUT.rgb = ambient + diffuse + specular;
    OUT.a = 1.0f;

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
