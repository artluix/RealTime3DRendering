#include "include/Common.fxh"
#include "include/Lights.fxh"

/************* Resources *************/

cbuffer CBufferPerFrame
{
    float3 CameraPosition;
}

cbuffer CBufferPerObject
{
    float4x4 ViewProjection;
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
    float4 specularColor : SPECULAR;
    float specularPower : SPECULARPOWER;
};

struct VS_OUTPUT
{
    float4 position: SV_Position;
    float3 normal : NORMAL;
    float2 textureCoordinate : TEXCOORD0;
    float3 worldPosition : TEXCOORD1;
    float3 viewDirection : TEXCOORD2;
    float4 specularColor : TEXCOORD3;
    float specularPower : TEXCOORD4;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.worldPosition = mul(IN.objectPosition, IN.World).xyz;
    OUT.viewDirection = normalize(CameraPosition - OUT.worldPosition);
    OUT.position = mul(float4(OUT.worldPosition, 1.f), ViewProjection);
    OUT.normal = normalize(mul(float4(IN.normal, 0.f), IN.World).xyz);
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate); // ?

    OUT.specularColor = IN.specularColor;
    OUT.specularPower = IN.specularPower;

    return OUT;
}

/************* Pixel Shader *************/

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    float4 color = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);

    LIGHT_OBJECT_PARAMS_EX lightObjectParamsEx;
    lightObjectParamsEx.normal = normalize(IN.normal);
    lightObjectParamsEx.viewDirection = normalize(IN.viewDirection);
    lightObjectParamsEx.worldPosition = IN.worldPosition;
    lightObjectParamsEx.color = color;
    lightObjectParamsEx.specularColor = IN.specularColor;
    lightObjectParamsEx.specularPower = IN.specularPower;

    OUT.rgb = get_light_contribution(lightObjectParamsEx);
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
