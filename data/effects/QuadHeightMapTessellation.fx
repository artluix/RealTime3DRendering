/************* Resources *************/

cbuffer CBufferPerFrame
{
    float tessellationEdgeFactors[4];
    float tessellationInsideFactor[2];
}

cbuffer CBufferPerObject
{
    float4x4 wvp;
    float4x4 textureMatrix;

    float displacementScale;
}

Texture2D HeightMap;

SamplerState TrilinearSampler
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
};

struct VS_OUTPUT
{
    float4 objectPosition : POSITION;
    float2 textureCoordinate : TEXCOORD;
};

struct HS_CONSTANT_OUTPUT
{
    float edgeFactors[4] : SV_TessFactor;
    float insideFactors[2] : SV_InsideTessFactor;
};

struct HS_OUTPUT
{
    float4 objectPosition : POSITION;
    float2 textureCoordinate : TEXCOORD;
};

struct DS_OUTPUT
{
    float4 position : SV_Position;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.objectPosition = IN.objectPosition;
    OUT.textureCoordinate = mul(float4(IN.textureCoordinate.xy, 0.f, 1.f), textureMatrix).xy;

    return OUT;
}

/************* Hull Shaders *************/

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("constant_hull_shader")]
HS_OUTPUT hull_shader(InputPatch<VS_OUTPUT, 4> patch, uint controlPointID : SV_OutputControlPointID)
{
    HS_OUTPUT OUT = (HS_OUTPUT)0;

    OUT.objectPosition = patch[controlPointID].objectPosition;
    OUT.textureCoordinate = patch[controlPointID].textureCoordinate;

    return OUT;
}

HS_CONSTANT_OUTPUT constant_hull_shader(InputPatch<VS_OUTPUT, 4> patch)
{
    HS_CONSTANT_OUTPUT OUT = (HS_CONSTANT_OUTPUT)0;

    [unroll]
    for (int i = 0; i < 4; i++)
    {
        OUT.edgeFactors[i] = tessellationEdgeFactors[i];
    }

    OUT.insideFactors[0] = tessellationInsideFactor[0];
    OUT.insideFactors[1] = tessellationInsideFactor[1];

    return OUT;
}

/************* Domain Shader *************/

[domain("quad")]
DS_OUTPUT domain_shader(HS_CONSTANT_OUTPUT IN, float2 uv : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 4> patch)
{
    DS_OUTPUT OUT = (DS_OUTPUT)0;

    float4 v0 = lerp(patch[0].objectPosition, patch[1].objectPosition, uv.x);
    float4 v1 = lerp(patch[2].objectPosition, patch[3].objectPosition, uv.x);
    float4 objectPosition = lerp(v0, v1, uv.y);

    float2 texCoord0 = lerp(patch[0].textureCoordinate, patch[1].textureCoordinate, uv.x);
    float2 texCoord1 = lerp(patch[2].textureCoordinate, patch[3].textureCoordinate, uv.x);
    float2 textureCoordinate = lerp(texCoord0, texCoord1, uv.y);

    objectPosition.y = (2.f * HeightMap.SampleLevel(TrilinearSampler, textureCoordinate, 0.f).x - 1.f) * displacementScale;

    OUT.position = mul(float4(objectPosition.xyz, 1.f), wvp);

    return OUT;
}

/************* Pixel Shader *************/

static const float4 k_wheatColor = { 0.961f, 0.871f, 0.702f, 1.f };

float4 pixel_shader(DS_OUTPUT IN) : SV_Target
{
    return k_wheatColor;
}

/************* Techniques *************/

technique11 main11
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetHullShader(CompileShader(hs_5_0, hull_shader()));
        SetDomainShader(CompileShader(ds_5_0, domain_shader()));
        SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
    }
}
