#include "include/Common.fxh"

// Resources
cbuffer CBufferPerFrame
{
    float4 ambientColor : AMBIENT <
        string UIName = "Ambient Light";
        string UIWidget = "Color";
    > = { 1.0f, 1.0f, 1.0f, 0.0f };

    float4 envColor : COLOR <
        string UIName = "EnvironmentColor";
        string UIWidget = "Color";
    > = { 1.0f, 1.0f, 1.0f, 1.0f };

    float3 cameraPosition : CAMERAPOSITION <string UIWidget="None";>;
}

cbuffer CBufferPerObject
{
    float4x4 wvp : WORLDVIEWPROJECTION <string UIWIdget="None";>;
    float4x4 world : WORLDVIEWPROJECTION <string UIWIdget="None";>;

    float reflectionAmount <
        string UIName = "Reflection Amount";
        string UIWIdget = "slider";
        float UIMin = 0.0;
        float UIMax = 1.0;
        float UIStep = 0.05;
    > = {0.5f};
}

Texture2D ColorTexture <
    string ResourceName="default_color.dds";
    string UIName="Color Texture";
    string ResourceType="2D";
>;

TextureCube EnvironmentMap <
    string UIName = "Environment Map";
    string ResourceType = "3D";
>;

SamplerState TrilinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
};

RasterizerState DisableCulling
{
    CullMode = None;
};

// Data Structures 
struct VS_INPUT
{
    float4 objectPosition : POSITION;
    float3 normal : NORMAL;
    float2 textureCoordinate : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position: SV_Position;
    float2 textureCoordinate : TEXCOORD;
    float3 reflectionVector : TEXCOORD1;
};

// Vertex Shader 
VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = mul(IN.objectPosition, wvp);
    OUT.textureCoordinate = get_corrected_texture_coordinate(IN.textureCoordinate);

    float3 worldPosition = mul(IN.objectPosition, world);
    float3 incident = normalize(worldPosition - cameraPosition);
    float3 normal = normalize(mul(float4(IN.normal, 0), world).xyz);

    // Reflection Vector for cube map: R = I - 2 * N * (I, N)
    OUT.reflectionVector = reflect(incident, normal);

    return OUT;
}

// Pixel Shader
float4 pixel_shader(VS_OUTPUT IN) : SV_Target
{
    float4 OUT = (float4)0;

    float4 color = ColorTexture.Sample(TrilinearSampler, IN.textureCoordinate);
    float3 ambient = get_color_contribution(ambientColor, color.rgb);
    float3 environment = EnvironmentMap.Sample(TrilinearSampler, IN.reflectionVector).rgb;
    float3 reflection = get_color_contribution(envColor, environment);

    OUT.rgb = lerp(ambient, reflection, reflectionAmount);

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
