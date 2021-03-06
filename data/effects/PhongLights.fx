#include "include/Common.fxh"
#include "include/PhongLighting.fxh"

/************* Resources *************/

cbuffer CBufferPerFrame
{
    float3 CameraPosition;
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
    AddressU = CLAMP;
    AddressV = CLAMP;
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
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 textureCoordinate : TEXCOORD0;
    float3 worldPosition : TEXCOORD1;
};

struct PSD_OUTPUT
{
    float4 color : SV_Target;
    float4 normal : SV_Target1;
    float4 worldPosition : SV_Target2;
};

struct VSL_INPUT
{
    float4 position : POSITION;
    float2 textureCoordinate : TEXCOORD;
};
struct VSL_OUTPUT
{
    float4 position : SV_Position;
    float2 textureCoordinate : TEXCOORD;
};
struct PSL_OUTPUT
{
    float4 color : SV_Target;
    float depth : SV_Depth;
};

/************* Shaders *************/

/* Common Pixel Shader Part */

float4 compute_light(float4 color, float3 normal, float3 worldPosition)
{
    float4 outColor = (float4)0;

    float3 N = normalize(normal);
    float3 V = normalize(CameraPosition - worldPosition);

    outColor.rgb = get_light_contribution(phong_lighting_create_object_params(color, N, V, worldPosition));
    outColor.a = 1.0f;

    return outColor;
}

/* Forward */

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, WVP);
    OUT.worldPosition = mul(IN.objectPosition, World).xyz;
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.normal = normalize(mul(float4(IN.normal, 0), World).xyz);

    return OUT;
}

float4 pixel_shader_forward(VS_OUTPUT IN) : SV_Target
{
    float4 color = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);
    return compute_light(color, IN.normal, IN.worldPosition);
}

/* Geometry Pass (uses Vertex Shader from Forward) */

PSD_OUTPUT pixel_shader_geometry(VS_OUTPUT IN)
{
    PSD_OUTPUT OUT = (PSD_OUTPUT)0;

    OUT.color = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);
    OUT.normal = float4(normalize(IN.normal), 1.f);
    OUT.worldPosition = float4(IN.worldPosition, 1.f);

    return OUT;
}

/* Light Pass of deferred */

VSL_OUTPUT vertex_shader_light(VSL_INPUT IN)
{
    VSL_OUTPUT OUT = (VSL_OUTPUT)0;

    OUT.position = IN.position;
    OUT.textureCoordinate = IN.textureCoordinate;

    return OUT;
}

PSL_OUTPUT pixel_shader_light(VSL_OUTPUT IN)
{
    PSL_OUTPUT OUT = (PSL_OUTPUT)0;

    float4 color = ColorBufferTexture.Sample(PointSampler, IN.textureCoordinate);
    float3 normal = NormalBufferTexture.Sample(PointSampler, IN.textureCoordinate).xyz;
    float3 worldPosition = WorldPositionBufferTexture.Sample(PointSampler, IN.textureCoordinate).xyz;

    OUT.color = compute_light(color, normal, worldPosition);
    OUT.depth = DepthBufferTexture.Sample(PointSampler, IN.textureCoordinate).x;

    return OUT;
}

/************* Techniques *************/

technique11 forward
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader_forward()));

        SetRasterizerState(DisableCulling);
    }
}

technique11 deferred
{
    pass geometry
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader_geometry()));

        SetRasterizerState(DisableCulling);
    }
    pass light
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader_light()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader_light()));

        SetRasterizerState(DisableCulling);
    }
}