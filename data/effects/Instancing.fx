#include "include/Common.fxh"
#include "include/Lights.fxh"

/************* Resources *************/

cbuffer CBufferPerFrame
{
    float3 CameraPosition : CAMERAPOSITION;

    POINT_LIGHT_DATA LightData; // TODO: remove
}

cbuffer CBufferPerObject
{
    float4x4 ViewProjection : WORLDVIEWPROJECTION;
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
    row_major float4x4 World : WORLD;
};

struct VS_OUTPUT
{
    float4 position: SV_Position;
    float3 normal : NORMAL;
    float2 textureCoordinate : TEXCOORD0;
    float3 worldPosition : TEXCOORD1;
    float3 viewDirection : TEXCOORD1;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.worldPosition = mul(IN.objectPosition, IN.World).xyz;
    OUT.position = mul(float4(OUT.worldPosition, 1.f), ViewProjection);
    OUT.normal = normalize(mul(float4(IN.normal, 0.f), IN.World).xyz);
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate); // ?

    float3 lightDirection = LightData.position - OUT.worldPosition;
    OUT.attenuation = saturate(1.0f - (length(lightDirection) / LightData.radius));
    OUT.specularColor = IN.specularColor;
    OUT.specularPower = IN.specularPower;

    return OUT;
}

/************* Pixel Shader *************/

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    float3 lightDirection = normalize(LightData.position - IN.worldPosition);
    float3 viewDirection = normalize(CameraPosition - IN.worldPosition);

    float3 normal = normalize(IN.normal);
    float n_dot_l = dot(normal, lightDirection);
    float3 halfVector = normalize(lightDirection + viewDirection);
    float n_dot_h = dot(normal, halfVector);

    float4 color = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);
    float4 lightCoefficients = lit(n_dot_l, n_dot_h, IN.specularPower);

    float3 ambient = get_color_contribution(AmbientColor, color.rgb);
    float3 diffuse = get_color_contribution(LightData.color, lightCoefficients.y * color.rgb) * IN.attenuation;
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
