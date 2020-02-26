#include "include/Common.fxh"
#include "include/Lights.fxh"

// Resources
cbuffer CBufferPerFrame
{
    float3 CameraPosition;

    POINT_LIGHT_DATA LightData; // TODO: remove

    float DisplacementScale;
}

cbuffer CBufferPerObject
{
    float4x4 WVP;
    float4x4 World;
}

Texture2D ColorTexture;
Texture2D DisplacementMap;
SamplerState TrilinearSampler
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
};

// Vertex Shader
VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    float2 textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);

    if (DisplacementScale > 0.0f)
    {
        float displacement = DisplacementMap.SampleLevel(TrilinearSampler, textureCoordinate, 0).x;
        IN.objectPosition.xyz += IN.normal * DisplacementScale * (displacement - 1);
    }

    OUT.position = mul(IN.objectPosition, WVP);
    OUT.textureCoordinate = textureCoordinate;
    OUT.normal = normalize(mul(float4(IN.normal, 0), World).xyz);

    OUT.worldPosition = mul(IN.objectPosition, World).xyz;
    OUT.viewDirection = normalize(CameraPosition - OUT.worldPosition);

    return OUT;
}

// Pixel Shader
float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    float4 color = ColorTexture.Sample(TrilinearSampler, IN.textureCoordinate);

    LIGHT_OBJECT_PARAMS lightObjectParams;
    lightObjectParams.normal = normalize(IN.normal);
    lightObjectParams.viewDirection = normalize(IN.viewDirection);
    lightObjectParams.worldPosition = IN.worldPosition;
    lightObjectParams.color = color;

    OUT.rgb = get_light_contribution(lightObjectParams);
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
