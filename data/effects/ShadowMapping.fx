#include "include/Common.fxh"

/************* Resources *************/
static const float4 WhiteColor = { 1.f, 1.f, 1.f, 1.f };
static const float3 BlackColor = { 0.f, 0.f, 0.f };
static const float DepthBias = 0.005f;

cbuffer CBufferPerFrame
{
    float4 ambientColor = { 1.0f, 1.0f, 1.0f, 0.0f };
    float3 cameraPosition;

    float4 lightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    float3 lightPosition = { 0.0f, 0.0f, 0.0f };
    float lightRadius = 10.0f;

    float2 shadowMapSize = { 1024.f, 1024.f };
}

cbuffer CBufferPerObject
{
    float4x4 wvp : WORLDVIEWPROJECTION;
    float4x4 world : WORLD;

    float4 specularColor : SPECULAR = { 1.0f, 1.0f, 1.0f, 1.0f };
    float specularPower : SPECULARPOWER = 25.0f;

    float4x4 projectiveTextureMatrix;
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
    float attenuation : TEXCOORD2;
    float4 shadowTextureCoordinate : TEXCOORD3;
};

struct LIGHT_OUTPUT
{
    float3 ambient;
    float3 diffuse;
    float3 specular;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, wvp);
    OUT.worldPosition = mul(IN.objectPosition, world).xyz;
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.normal = normalize(mul(float4(IN.normal, 0), world).xyz);
    
    float3 lightDirection = lightPosition - OUT.worldPosition;
    OUT.attenuation = saturate(1.0f - (length(lightDirection) / lightRadius));

    OUT.shadowTextureCoordinate = mul(IN.objectPosition, projectiveTextureMatrix);

    return OUT;
}

/************* Pixel Shader *************/

LIGHT_OUTPUT compute_light(VS_OUTPUT IN)
{
    LIGHT_OUTPUT OUT = (LIGHT_OUTPUT)0;

    float3 lightDirection = normalize(lightPosition - IN.worldPosition);
    float3 viewDirection = normalize(cameraPosition - IN.worldPosition);

    float3 normal = normalize(IN.normal);
    float n_dot_l = dot(normal, lightDirection);
    float3 halfVector = normalize(lightDirection + viewDirection);
    float n_dot_h = dot(normal, halfVector);

    float4 color = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);
    float4 lightCoefficients = lit(n_dot_l, n_dot_h, specularPower);

    OUT.ambient = get_color_contribution(ambientColor, color.rgb);
    OUT.diffuse = get_color_contribution(lightColor, lightCoefficients.y * color.rgb) * IN.attenuation;
    OUT.specular = get_color_contribution(specularColor, min(lightCoefficients.z, color.w)) * IN.attenuation;

    return OUT;
}

float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    LIGHT_OUTPUT light = compute_light(IN);

    if (IN.shadowTextureCoordinate.w >= 0.f)
    {
        IN.shadowTextureCoordinate.xyz /= IN.shadowTextureCoordinate.w;
        float pixelDepth = IN.shadowTextureCoordinate.z;
        float sampledDepth = ShadowMapTexture.Sample(ShadowMapSampler, IN.shadowTextureCoordinate.xy).x + DepthBias;

        // Shadow applied in a boolean fashion -- either shadow or not
        float3 shadow = (pixelDepth > sampledDepth ? BlackColor : WhiteColor.rgb);

        light.diffuse *= shadow;
        light.specular *= shadow;
    }

    OUT.rgb = light.ambient + light.diffuse + light.specular;
    OUT.a = 1.0f;

    return OUT;
}

// pcf = percentage closer filtering
float4 manual_pcf_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    LIGHT_OUTPUT light = compute_light(IN);

    if (IN.shadowTextureCoordinate.w >= 0.f)
    {
        IN.shadowTextureCoordinate.xyz /= IN.shadowTextureCoordinate.w;

        float2 texelSize = 1.f / shadowMapSize;

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

        float2 lerpValues = frac(IN.shadowTextureCoordinate.xy * shadowMapSize);

        float shadow = lerp(
            lerp(shadowFactors[0], shadowFactors[1], lerpValues.x),
            lerp(shadowFactors[2], shadowFactors[3], lerpValues.x),
            lerpValues.y
        );

        light.diffuse *= shadow;
        light.specular *= shadow;
    }

    OUT.rgb = light.ambient + light.diffuse + light.specular;
    OUT.a = 1.0f;

    return OUT;
}

// pcf = percentage closer filtering
float4 pcf_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    LIGHT_OUTPUT light = compute_light(IN);

    if (IN.shadowTextureCoordinate.w >= 0.f)
    {
        IN.shadowTextureCoordinate.xyz /= IN.shadowTextureCoordinate.w;
        float pixelDepth = IN.shadowTextureCoordinate.z;
        float shadow = ShadowMapTexture.SampleCmpLevelZero(PcfShadowMapSampler, IN.shadowTextureCoordinate.xy, pixelDepth).x;

        light.diffuse *= shadow;
        light.specular *= shadow;
    }

    OUT.rgb = light.ambient + light.diffuse + light.specular;
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
