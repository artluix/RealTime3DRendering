#include "include/Common.fxh"
#include "include/Lights.fxh"

// Resources
cbuffer CBufferPerFrame
{
    float4 AmbientColor : AMBIENT;
    float3 CameraPosition : CAMERAPOSITION;

    SPOTLIGHT_DATA LightData;
}

cbuffer CBufferPerObject
{
    float4x4 WVP : WORLDVIEWPROJECTION;
    float4x4 World : WORLD;

    float4 SpecularColor : SPECULAR;
    float SpecularPower : SPECULARPOWER;
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
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 textureCoordinate : TEXCOORD0;
    float3 worldPosition : TEXCOORD1;
    float3 viewDirection : TEXCOORD2;
};

// Vertex Shader
VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, WVP);
    OUT.normal = normalize(mul(float4(IN.normal, 0), World).xyz);
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.worldPosition = mul(IN.objectPosition, World).xyz;
    OUT.viewDirection = normalize(CameraPosition - OUT.worldPosition);

    return OUT;
}

// Pixel Shader
float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    float4 color = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);
    float3 viewDirection = normalize(IN.viewDirection);

    LIGHTS_COMMON_PARAMS lightsCommonParams;
    lightsCommonParams.normal = IN.normal;
    lightsCommonParams.viewDirection = viewDirection;
    lightsCommonParams.worldPosition = IN.worldPosition;
    lightsCommonParams.specularPower = SpecularPower;
    lightsCommonParams.specularColor = SpecularColor;
    lightsCommonParams.ambientColor = AmbientColor;
    lightsCommonParams.color = color;

    OUT.rgb = get_lights_contribution(lightsCommonParams);
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
