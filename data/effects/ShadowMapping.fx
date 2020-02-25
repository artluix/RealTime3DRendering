#include "include/Common.fxh"
#include "include/Lights.fxh"

/************* Resources *************/
static const float4 WhiteColor = { 1.f, 1.f, 1.f, 1.f };
static const float3 BlackColor = { 0.f, 0.f, 0.f };
static const float DepthBias = 0.005f;

cbuffer CBufferPerFrame
{
    float3 CameraPosition;

    float2 ShadowMapSize = { 1024.f, 1024.f };

    POINT_LIGHT_DATA LightData; // TODO: remove
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
Texture2D ShadowMapTexture;

SamplerState ShadowMapSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = BORDER;
    AddressV = BORDER;
    BorderColor = WhiteColor;
};

SamplerComparisonState PcfShadowMapSampler
{
    Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
    AddressU = BORDER;
    AddressV = BORDER;
    BorderColor = WhiteColor;
    ComparisonFunc = LESS_EQUAL;
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
    float4 shadowTextureCoordinate : TEXCOORD3;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, WVP);
    OUT.normal = normalize(mul(float4(IN.normal, 0), World).xyz);
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.worldPosition = mul(IN.objectPosition, World).xyz;
    OUT.viewDirection = normalize(CameraPosition - OUT.worldPosition);
    OUT.shadowTextureCoordinate = mul(IN.objectPosition, ProjectiveTextureMatrix);

    return OUT;
}

/************* Pixel Shader *************/

float3 compute_specular_diffuse(VS_OUTPUT IN)
{
    float3 viewDirection = normalize(IN.viewDirection);
    float4 color = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);

    LIGHTS_COMMON_PARAMS lightsCommonParams;
    lightsCommonParams.normal = normalize(IN.normal);
    lightsCommonParams.viewDirection = viewDirection;
    lightsCommonParams.worldPosition = IN.worldPosition;
    lightsCommonParams.color = color;

    return get_specular_diffuse(lightsCommonParams);
}

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    float3 specularDiffuse = compute_specular_diffuse(IN);

    if (IN.shadowTextureCoordinate.w >= 0.f)
    {
        IN.shadowTextureCoordinate.xyz /= IN.shadowTextureCoordinate.w;
        float pixelDepth = IN.shadowTextureCoordinate.z;
        float sampledDepth = ShadowMapTexture.Sample(ShadowMapSampler, IN.shadowTextureCoordinate.xy).x + DepthBias;

        // Shadow applied in a boolean fashion -- either shadow or not
        float3 shadow = (pixelDepth > sampledDepth ? BlackColor : WhiteColor.rgb);

        specularDiffuse *= shadow;
    }

    OUT.rgb = get_ambient(color.rgb) + specularDiffuse;
    OUT.a = 1.0f;

    return OUT;
}

// pcf = percentage closer filtering
float4 manual_pcf_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    float3 specularDiffuse = compute_specular_diffuse(IN);

    if (IN.shadowTextureCoordinate.w >= 0.f)
    {
        IN.shadowTextureCoordinate.xyz /= IN.shadowTextureCoordinate.w;

        float2 texelSize = 1.f / ShadowMapSize;

        float sampledDepths[4];
        sampledDepths[0] = ShadowMapTexture.Sample(ShadowMapSampler, IN.shadowTextureCoordinate.xy).x + DepthBias;
        sampledDepths[1] = ShadowMapTexture.Sample(ShadowMapSampler, IN.shadowTextureCoordinate.xy + float2(texelSize.x, 0)).x + DepthBias;
        sampledDepths[2] = ShadowMapTexture.Sample(ShadowMapSampler, IN.shadowTextureCoordinate.xy + float2(0, texelSize.y)).x + DepthBias;
        sampledDepths[3] = ShadowMapTexture.Sample(ShadowMapSampler, IN.shadowTextureCoordinate.xy + texelSize).x + DepthBias;

        float pixelDepth = IN.shadowTextureCoordinate.z;

        float shadowFactors[4];
        [unroll(4)] for (uint i = 0; i < 4; i++)
        {
            shadowFactors[i] = (pixelDepth > sampledDepths[i] ? 0.f : 1.f);
        }

        float2 lerpValues = frac(IN.shadowTextureCoordinate.xy * ShadowMapSize);

        float shadow = lerp(
            lerp(shadowFactors[0], shadowFactors[1], lerpValues.x),
            lerp(shadowFactors[2], shadowFactors[3], lerpValues.x),
            lerpValues.y
        );

        specularDiffuse *= shadow;
    }

    OUT.rgb = get_ambient(color.rgb) + specularDiffuse;
    OUT.a = 1.0f;

    return OUT;
}

// pcf = percentage closer filtering
float4 pcf_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    float3 specularDiffuse = compute_specular_diffuse(IN);

    if (IN.shadowTextureCoordinate.w >= 0.f)
    {
        IN.shadowTextureCoordinate.xyz /= IN.shadowTextureCoordinate.w;
        float pixelDepth = IN.shadowTextureCoordinate.z;
        float shadow = ShadowMapTexture.SampleCmpLevelZero(PcfShadowMapSampler, IN.shadowTextureCoordinate.xy, pixelDepth).x;

        specularDiffuse *= shadow;
    }

    OUT.rgb = get_ambient(color.rgb) + specularDiffuse;
    OUT.a = 1.0f;

    return OUT;
}

/************* Techniques *************/

technique11 shadow_mapping
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pixel_shader()));

        SetRasterizerState(BackCulling);
    }
}

technique11 shadow_mapping_manual_pcf
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, manual_pcf_pixel_shader()));

        SetRasterizerState(BackCulling);
    }
}

technique11 shadow_mapping_pcf
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, pcf_pixel_shader()));

        SetRasterizerState(BackCulling);
    }
}
