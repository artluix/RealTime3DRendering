#include "include/Common.fxh"
#include "include/PbrLighting.fxh"

/************* Resources *************/

cbuffer CBufferPerFrame
{
    float3 CameraPosition;
}

cbuffer CBufferPerObject
{
    float4x4 WVP;
    float4x4 World;

    float3 Albedo;
    float Metallic;
    float Roughness;
    float AO;
}

Texture2D AlbedoMap;
Texture2D MetallicMap;
Texture2D RoughnessMap;
Texture2D AOMap;

SamplerState TrilinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
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
    float3 viewDirection : TEXCOORD2;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, WVP);
    OUT.worldPosition = mul(IN.objectPosition, World).xyz;
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.normal = normalize(mul(float4(IN.normal, 0), World).xyz);
    OUT.viewDirection = normalize(CameraPosition - OUT.worldPosition);

    return OUT;
}

/************* Pixel Shader *************/

float4 pixel_shader_values(VS_OUTPUT IN) : SV_Target
{
    PBR_LIGHTING_OBJECT_PARAMS objectParams;
    objectParams.normal = normalize(IN.normal);
    objectParams.viewDirection = normalize(IN.viewDirection);
    objectParams.worldPosition = IN.worldPosition;
    objectParams.albedo = Albedo;
    objectParams.metallic = Metallic;
    objectParams.roughness = Roughness;
    objectParams.ao = AO;

    float3 color = get_light_contribution(objectParams);

    color = tonemap_reinhard(color);
    color = gamma_correction(color);

    return float4(color, 1.f);
}

float4 pixel_shader_maps(VS_OUTPUT IN) : SV_Target
{
    PBR_LIGHTING_OBJECT_PARAMS objectParams;
    objectParams.normal = normalize(IN.normal);
    objectParams.viewDirection = normalize(IN.viewDirection);
    objectParams.worldPosition = IN.worldPosition;
    objectParams.albedo = pow(AlbedoMap.Sample(TrilinearSampler, IN.textureCoordinate).rgb, 2.2); // convert to linear space
    objectParams.metallic = MetallicMap.Sample(TrilinearSampler, IN.textureCoordinate).r;
    objectParams.roughness = RoughnessMap.Sample(TrilinearSampler, IN.textureCoordinate).r;
    objectParams.ao = AO;
    // objectParams.ao = AOMap.Sample(TrilinearSampler, IN.textureCoordinate).r;

    float3 color = get_light_contribution(objectParams);

    color = tonemap_reinhard(color);
    color = gamma_correction(color);

    return float4(color, 1.f);
}

/************* Techniques *************/

technique11 values
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader_values()));

        SetRasterizerState(DisableCulling);
    }
}

technique11 maps
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader_maps()));

        SetRasterizerState(DisableCulling);
    }
}
