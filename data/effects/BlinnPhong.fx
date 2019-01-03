#include "include/Common.fxh"

// Resources
cbuffer CBufferPerFrame
{
    float4 ambientColor : AMBIENT <
        string UIName = "Ambient Light";
        string UIWidget = "Color";
    > = { 1.0f, 1.0f, 1.0f, 1.0f };

    float4 lightColor : COLOR <
        string Object = "LightColor0";
        string UIName = "Light Color";
        string UIWidget = "Color";
    > = { 1.0f, 1.0f, 1.0f, 1.0f };

    float3 lightDirection : DIRECTION <
        string Object = "DirectionLight0";
        string UIName = "Light Direction";
        string Space = "World";
    > = { 0.0f, 0.0f, -1.0f };

    float3 cameraPosition : CAMERAPOSITION <string UIWIdget="None";>;
}

cbuffer CBufferPerObject
{
    float4x4 wvp : WORLDVIEWPROJECTION <string UIWIdget="None";>;
    float4x4 world : WORLD <string UIWIdget="None";>;

    float4 specularColor : SPECULAR <
        string UIName = "Specular Color";
        string UIWidget = "Color";
    > = { 1.0f, 1.0f, 1.0f, 1.0f };

    float specularPower : SPECULARPOWER <
        string UIName = "Specular Power";
        string UIWidget = "slider";
        float UIMin = 1.0;
        float UIMax = 255.0;
        float UIStep = 1.0;
    > = { 25.0f };
}

RasterizerState DisableCulling
{
    CullMode = None;
};

Texture2D ColorTexture <
    string ResourceName="default_color.dds";
    string UIName="Color Texture";
    string ResourceType="2D";
>;

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
    float4 position: SV_Position;
    float3 normal : NORMAL;
    float2 textureCoordinate : TEXCOORD0;
    float3 lightDirection : TEXCOORD1;
    float3 viewDirection : TEXCOORD2;
};

// Vertex Shader 
VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, wvp);
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);
    OUT.normal = normalize(mul(float4(IN.normal, 0), world).xyz);
    OUT.lightDirection = normalize(-lightDirection);

    float3 worldPosition = mul(IN.objectPosition, world).xyz;
    OUT.viewDirection = normalize(cameraPosition - worldPosition);

    return OUT;
}

// Pixel Shader
float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    float3 normal = normalize(IN.normal);
    float3 lightDirection = normalize(IN.lightDirection);
    float3 viewDirection = normalize(IN.viewDirection);
    float n_dot_l = dot(lightDirection, normal);

    float4 color = ColorTexture.Sample(ColorSampler, IN.textureCoordinate);
    float3 ambient = ambientColor.rgb * ambientColor.a * color.rgb;

    float3 diffuse = (float3)0;
    float3 specular = (float3)0;

    if (n_dot_l > 0)
    {
        diffuse = lightColor.rgb * lightColor.a * saturate(n_dot_l) * color.rgb;

        float3 halfVector = normalize(lightDirection + viewDirection);

        // specular = (N dot H)^S with gloss map in color texture's alpha channel
        specular = specularColor.rgb * specularColor.a * min(pow(saturate(dot(normal, halfVector)), specularPower), color.w);
    }

    OUT.rgb = ambient + diffuse + specular;
    OUT.a = 1.0f;

    return OUT;
}

// Techniques
technique10 main10
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, pixel_shader()));

        SetRasterizerState(DisableCulling);
    }
}
