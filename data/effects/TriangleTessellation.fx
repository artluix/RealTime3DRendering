/************* Resources *************/

cbuffer CBufferPerFrame
{
    float k_tessellationEdgeFactors[3];
    float k_tessellationInsideFactor;
}

cbuffer CBufferPerObject
{
    float4x4 k_wvp;
}

/************* Data Structures *************/

struct VS_INPUT
{
    float4 objectPosition : POSITION;
};

struct VS_OUTPUT
{
    float4 objectPosition : POSITION;
};

struct HS_CONSTANT_OUTPUT
{
    float edgeFactors[3] : SV_TessFactor;
    float insideFactor : SV_InsideTessFactor;
};

struct HS_OUTPUT
{
    float4 objectPosition : POSITION;
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

    return OUT;
}

/************* Hull Shaders *************/

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("constant_hull_shader")]
HS_OUTPUT hull_shader(InputPatch<VS_OUTPUT, 3> patch, uint controlPointID : SV_OutputControlPointID)
{
    HS_OUTPUT OUT = (HS_OUTPUT)0;

    OUT.objectPosition = patch[controlPointID].objectPosition;

    return OUT;
}

HS_CONSTANT_OUTPUT constant_hull_shader(InputPatch<VS_OUTPUT, 3> patch)
{
    HS_CONSTANT_OUTPUT OUT = (HS_CONSTANT_OUTPUT)0;

    [unroll]
    for (int i = 0; i < 3; i++)
    {
        OUT.edgeFactors[i] = k_tessellationEdgeFactors[i];
    }

    OUT.insideFactor = k_tessellationInsideFactor;

    return OUT;
}

/************* Domain Shader *************/

[domain("tri")]
DS_OUTPUT domain_shader(HS_CONSTANT_OUTPUT IN, float3 uvw : SV_DomainLocation, const OutputPatch<HS_OUTPUT, 3> patch)
{
    DS_OUTPUT OUT = (DS_OUTPUT)0;

    float3 objectPosition =
        uvw.x * patch[0].objectPosition.xyz + 
        uvw.y * patch[1].objectPosition.xyz +
        uvw.z * patch[2].objectPosition.xyz;

    OUT.position = mul(float4(objectPosition, 1.f), k_wvp);

    return OUT;
}

/************* Pixel Shader *************/

static const float4 k_whiteColor = { 0.961f, 0.871f, 0.702f, 1.f };

float4 pixel_shader(DS_OUTPUT IN) : SV_Target
{
    return k_whiteColor;
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
