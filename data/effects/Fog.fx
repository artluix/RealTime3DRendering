#include "include/Common.fxh"
#include "include/Lights.fxh"

// Resources
cbuffer CBufferPerFrame
{
    float3 CameraPosition;

    float3 FogColor = { 0.5f, 0.5f, 0.5f };
    float FogStart = { 20.0f };
    float FogRange = { 40.0f };
}

cbuffer CBufferPerObject
{
    float4x4 WVP;
    float4x4 World;
}

Texture2D ColorTexture;
SamplerState ColorSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

RasterizerState DisableCulling
{
    CullMode = None;
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
    float3 viewDirection : TEXCOORD2;
    float fogAmount : TEXCOORD3;
};

// Vertex Shader
VS_OUTPUT vertex_shader(VS_INPUT IN, uniform bool fogEnabled)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, WVP);
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.normal = normalize(mul(float4(IN.normal, 0), World).xyz);

    OUT.worldPosition = mul(IN.objectPosition, World).xyz;
    float3 viewDirection = CameraPosition - OUT.worldPosition;

    if (fogEnabled)
    {
        OUT.fogAmount = get_fog_amount(viewDirection, FogStart, FogRange);
    }

    OUT.viewDirection = normalize(viewDirection);

    return OUT;
}

// Pixel Shader
float4 pixel_shader(VS_OUTPUT IN, uniform bool fogEnabled) : SV_Target
{
    float4 OUT = (float4)0;

    float4 color = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);

    LIGHT_OBJECT_PARAMS lightObjectParams;
    lightObjectParams.normal = normalize(IN.normal);
    lightObjectParams.viewDirection = normalize(IN.viewDirection);
    lightObjectParams.worldPosition = IN.worldPosition;
    lightObjectParams.color = color;

    OUT.rgb = get_light_contribution(lightObjectParams);
    OUT.a = 1.0f;

    if (fogEnabled)
    {
        OUT.rgb = lerp(OUT.rgb, FogColor, IN.fogAmount);
    }

    return OUT;
}

// Techniques
technique11 fogEnabled
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader(true)));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader(true)));

        SetRasterizerState(DisableCulling);
    }
}

technique11 fogDisabled
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader(false)));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader(false)));

        SetRasterizerState(DisableCulling);
    }
}
