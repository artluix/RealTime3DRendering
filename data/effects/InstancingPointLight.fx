#include "include/Common.fxh"

/************* Resources *************/

cbuffer CBufferPerFrame
{
    float4 ambientColor = { 1.0f, 1.0f, 1.0f, 0.0f };
    float3 cameraPosition : CAMERAPOSITION;

    POINT_LIGHT_DATA lightData;
}

cbuffer CBufferPerObject
{
    float4x4 viewProjection : WORLDVIEWPROJECTION;
}

Texture2D ColorTexture;

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
    row_major float4x4 world : WORLD;
    float4 specularColor : SPECULAR;
    float specularPower : SPECULARPOWER;
};

struct VS_OUTPUT
{
    float4 position: SV_Position;
    float3 normal : NORMAL;
    float2 textureCoordinate : TEXCOORD0;
    float3 worldPosition : WORLD;
    float attenuation : TEXCOORD2;
    float4 specularColor : SPECULAR;
    float specularPower : SPECULARPOWER;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.worldPosition = mul(IN.objectPosition, IN.world).xyz;
    OUT.position = mul(float4(OUT.worldPosition, 1.f), viewProjection);
    OUT.normal = normalize(mul(float4(IN.normal, 0.f), IN.world).xyz);
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate); // ?

    float3 lightDirection = lightData.position - OUT.worldPosition;
    OUT.attenuation = saturate(1.0f - (length(lightDirection) / lightData.radius));
    OUT.specularColor = IN.specularColor;
    OUT.specularPower = IN.specularPower;

    return OUT;
}

/************* Pixel Shader *************/

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    float3 lightDirection = normalize(lightData.position - IN.worldPosition);
    float3 viewDirection = normalize(cameraPosition - IN.worldPosition);

    float3 normal = normalize(IN.normal);
    float n_dot_l = dot(normal, lightDirection);
    float3 halfVector = normalize(lightDirection + viewDirection);
    float n_dot_h = dot(normal, halfVector);

    float4 color = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);
    float4 lightCoefficients = lit(n_dot_l, n_dot_h, IN.specularPower);

    float3 ambient = get_color_contribution(ambientColor, color.rgb);
    float3 diffuse = get_color_contribution(lightData.color, lightCoefficients.y * color.rgb) * IN.attenuation;
    float3 specular = get_color_contribution(IN.specularColor, min(lightCoefficients.z, color.w)) * IN.attenuation;

    OUT.rgb = ambient + diffuse + specular;
    OUT.a = 1.0f;

    return OUT;
}

/************* Techniques *************/

technique11 main11
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
    }
}
