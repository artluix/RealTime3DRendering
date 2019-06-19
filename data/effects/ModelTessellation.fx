/************* Resources *************/

cbuffer CBufferPerFrame
{
    float tessellationEdgeFactors[3];
    float tessellationInsideFactor;

    float maxTessFactor = 64.f;
    float minTessDistance = 2.f;
    float maxTessDistance = 20.f;

    float3 cameraPosition : CAMERAPOSITION;
}

cbuffer CBufferPerObject
{
    float4x4 wvp : WORLDVIEWPROJECTION;
    float4x4 world : WORLD;
}

Texture2D ColorTexture;

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
    float edgeFactors[3] : SV_TessFactor;
    float insideFactor : SV_InsideTessFactor;
};

struct HS_OUTPUT
{
    float4 objectPosition : POSITION;
    float2 textureCoordinate : TEXCOORD;
};

struct DS_OUTPUT
{
    float4 position : SV_Position;
    float2 textureCoordinate : TEXCOORD;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.objectPosition = IN.objectPosition;
    OUT.textureCoordinate = IN.textureCoordinate;

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
    OUT.textureCoordinate = patch[controlPointID].textureCoordinate;

    return OUT;
}

HS_CONSTANT_OUTPUT constant_hull_shader(InputPatch<VS_OUTPUT, 3> patch)
{
    HS_CONSTANT_OUTPUT OUT = (HS_CONSTANT_OUTPUT)0;

    [unroll]
    for (int i = 0; i < 3; i++)
    {
        OUT.edgeFactors[i] = tessellationEdgeFactors[i];
    }

    OUT.insideFactor = tessellationInsideFactor;

    return OUT;
}

/****************************************/

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("distance_constant_hull_shader")]
HS_OUTPUT distance_hull_shader(InputPatch<VS_OUTPUT, 3> patch, uint controlPointID : SV_OutputControlPointID)
{
    HS_OUTPUT OUT = (HS_OUTPUT)0;

    OUT.objectPosition = patch[controlPointID].objectPosition;
    OUT.textureCoordinate = patch[controlPointID].textureCoordinate;

    return OUT;
}

HS_CONSTANT_OUTPUT distance_constant_hull_shader(InputPatch<VS_OUTPUT, 3> patch)
{
    HS_CONSTANT_OUTPUT OUT = (HS_CONSTANT_OUTPUT)0;

    // Calculate the center of the patch
    float3 objectCenter = (patch[0].objectPosition.xyz + patch[1].objectPosition.xyz + patch[2].objectPosition.xyz) / 3.f;
    float3 worldCenter = mul(float4(objectCenter, 1.f), world).xyz;

    // Calculate uniform tessellation factor based on distance from the camera
    float tessFactor = max(
        min(
            maxTessFactor,
            (maxTessDistance - distance(worldCenter, cameraPosition)) / (maxTessDistance - minTessDistance) * maxTessFactor
        ),
        1.f
    );

    [unroll]
    for (int i = 0; i < 3; i++)
    {
        OUT.edgeFactors[i] = tessFactor;
    }

    OUT.insideFactor = tessFactor;

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

    objectPosition = normalize(objectPosition); // for icosahedron demo

    float2 textureCoordinate =
        uvw.x * patch[0].textureCoordinate + 
        uvw.y * patch[1].textureCoordinate +
        uvw.z * patch[2].textureCoordinate;

    OUT.position = mul(float4(objectPosition, 1.f), wvp);
    OUT.textureCoordinate = textureCoordinate;

    return OUT;
}

/************* Pixel Shader *************/

static const float4 k_wheatColor = { 0.961f, 0.871f, 0.702f, 1.f };

float4 pixel_shader(DS_OUTPUT IN) : SV_Target
{
    return ColorTexture.Sample(TrilinearSampler, IN.textureCoordinate);
}

float4 solid_color_pixel_shader(DS_OUTPUT IN) : SV_Target
{
    return k_wheatColor;
}

/************* Techniques *************/

technique11 textured_manual_factors
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetHullShader(CompileShader(hs_5_0, hull_shader()));
        SetDomainShader(CompileShader(ds_5_0, domain_shader()));
        SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
    }
}

technique11 solid_color_manual_factors
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetHullShader(CompileShader(hs_5_0, hull_shader()));
        SetDomainShader(CompileShader(ds_5_0, domain_shader()));
        SetPixelShader(CompileShader(ps_5_0, solid_color_pixel_shader()));
    }
}

technique11 solid_color_distance_factors
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetHullShader(CompileShader(hs_5_0, distance_hull_shader()));
        SetDomainShader(CompileShader(ds_5_0, domain_shader()));
        SetPixelShader(CompileShader(ps_5_0, solid_color_pixel_shader()));
    }
}
