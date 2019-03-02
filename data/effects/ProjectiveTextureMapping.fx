#include "include/Common.fxh"

/************* Resources *************/
static const float4 WhiteColor = { 1.f, 1.f, 1.f, 1.f };


cbuffer CBufferPerFrame
{
    float4 ambientColor = { 1.0f, 1.0f, 1.0f, 0.0f };
    float4 lightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    float3 lightPosition = { 0.0f, 0.0f, 0.0f };
    float lightRadius = 10.0f;
    float3 cameraPosition;
}

cbuffer CBufferPerObject
{
    float4x4 wvp : WORLDVIEWPROJECTION;
    float4x4 world : WORLD;

    float4 specularColor : SPECULAR = { 1.0f, 1.0f, 1.0f, 1.0f };
    float specularPower : SPECULARPOWER = 25.0f;

    float4x4 projectiveTextureMatrix;
}

RasterizerState BackCulling
{
    CullMode = BACK;
};

Texture2D ColorTexture;
Texture2D ProjectedTexture;

SamplerState ProjectedTextureSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = BORDER;
    AddressV = BORDER;
    BorderColor = WhiteColor;
};

SamplerState ColorSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

/************* Data Structures *************/

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
    float4 projectedTextureCoordinate : TEXCOORD3;
};

/************* Vertex Shader *************/

VS_OUTPUT project_texture_vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, wvp);
    OUT.worldPosition = mul(IN.objectPosition, world).xyz;
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.normal = normalize(mul(float4(IN.normal, 0), world).xyz);
    
    float3 lightDirection = lightPosition - OUT.worldPosition;
    OUT.attenuation = saturate(1.0f - (length(lightDirection) / lightRadius));

    OUT.projectedTextureCoordinate = mul(IN.objectPosition, projectiveTextureMatrix);

    return OUT;
}

/************* Pixel Shader *************/

float4 project_texture_pixel_shader(VS_OUTPUT IN) : SV_Target
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

    IN.projectedTextureCoordinate.xy /= IN.projectedTextureCoordinate.w;
    float3 projectedColor = ProjectedTexture.Sample(ProjectedTextureSampler, IN.projectedTextureCoordinate.xy).rgb;

    OUT.rgb *= projectedColor;

    return OUT;
}

/************* Techniques *************/

technique11 project_texture
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, project_texture_vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, project_texture_pixel_shader()));

        SetRasterizerState(BackCulling);
    }
}
