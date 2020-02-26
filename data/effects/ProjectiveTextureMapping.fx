#include "include/Common.fxh"
#include "include/Lights.fxh"

/************* Resources *************/
static const float4 WhiteColor = { 1.f, 1.f, 1.f, 1.f };

cbuffer CBufferPerFrame
{
    float3 CameraPosition;

    POINT_LIGHT_DATA LightData; // TODO: remove

    float DepthBias = 0.005f;
}

cbuffer CBufferPerObject
{
    float4x4 WVP;
    float4x4 World;

    float4x4 ProjectiveTextureMatrix;
}

RasterizerState BackCulling
{
    CullMode = BACK;
};

Texture2D ColorTexture;
Texture2D ProjectedTexture;
Texture2D DepthMapTexture;

SamplerState ProjectedTextureSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = BORDER;
    AddressV = BORDER;
    BorderColor = WhiteColor;
};

SamplerState DepthMapSampler
{
    Filter = MIN_MAG_MIP_POINT;
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
    float3 viewDirection : TEXCOORD2;
    float4 projectedTextureCoordinate : TEXCOORD3;
};

/************* Vertex Shader *************/

VS_OUTPUT project_texture_vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, WVP);
    OUT.worldPosition = mul(IN.objectPosition, World).xyz;
    OUT.viewDirection = normalize(CameraPosition - OUT.worldPosition);
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.normal = normalize(mul(float4(IN.normal, 0), World).xyz);
    OUT.projectedTextureCoordinate = mul(IN.objectPosition, ProjectiveTextureMatrix);

    return OUT;
}

/************* Pixel Shader *************/

float4 compute_light(VS_OUTPUT IN)
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

    return OUT;
}

float4 project_texture_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = compute_light(IN);

    if (IN.projectedTextureCoordinate.w >= 0.f)
    {
        IN.projectedTextureCoordinate.xy /= IN.projectedTextureCoordinate.w;
        float3 projectedColor = ProjectedTexture.Sample(ProjectedTextureSampler, IN.projectedTextureCoordinate.xy).rgb;

        OUT.rgb *= projectedColor;
    }

    return OUT;
}

float4 project_texture_depth_map_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = compute_light(IN);

    if (IN.projectedTextureCoordinate.w >= 0.f)
    {
        IN.projectedTextureCoordinate.xyz /= IN.projectedTextureCoordinate.w;
        float pixelDepth = IN.projectedTextureCoordinate.z;
        float sampledDepth = DepthMapTexture.Sample(DepthMapSampler, IN.projectedTextureCoordinate.xy).x + DepthBias;

        float3 projectedColor = (pixelDepth > sampledDepth ? WhiteColor.rgb : ProjectedTexture.Sample(ProjectedTextureSampler, IN.projectedTextureCoordinate.xy).rgb);
        OUT.rgb *= projectedColor;
    }

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

technique11 project_texture_depth_map
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, project_texture_vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, project_texture_depth_map_pixel_shader()));

        SetRasterizerState(BackCulling);
    }
}
