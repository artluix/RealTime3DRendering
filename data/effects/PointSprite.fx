/************* Resources *************/

cbuffer CBufferPerFrame
{
    float3 k_cameraPosition : CAMERAPOSITION;
    float3 k_cameraUp;
}

cbuffer CBufferPerObject
{
    float4x4 k_viewProjection;
}

Texture2D k_colorTexture;

SamplerState k_colorSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

/************* Data Structures *************/

struct VS_INPUT
{
    float4 position : POSITION;
    float2 size : SIZE;
};

struct VS_OUTPUT
{
    float4 position : POSITION;
    float2 size : SIZE;
};

struct VS_NOSIZE_OUTPUT
{
    float4 position : POSITION;
};

struct GS_OUTPUT
{
    float4 position : SV_Position;
    float2 textureCoordinate : TEXCOORD;
};

/************* Vertex Shader *************/

VS_OUTPUT vertex_shader(VS_INPUT IN)
{
    VS_OUTPUT OUT = (VS_OUTPUT)0;

    OUT.position = IN.position;
    OUT.size = IN.size;

    return OUT;
}

/************* Geometry Shader *************/

[maxvertexcount(6)]
void geometry_shader(point VS_OUTPUT IN[1], inout TriangleStream<GS_OUTPUT> triStream)
{
    static const float2 k_quadUVs[4] = {
        float2(0.f, 1.f), // v0
        float2(0.f, 0.f), // v1
        float2(1.f, 0.f), // v2
        float2(1.f, 1.f), // v3
    };

    GS_OUTPUT OUT = (GS_OUTPUT)0;

    float2 halfSize = IN[0].size / 2.f;

    float3 direction = k_cameraPosition - IN[0].position.xyz;
    float3 right = cross(normalize(direction), k_cameraUp);

    float3 offsetX = halfSize.x * right;
    float3 offsetY = halfSize.y * k_cameraUp;

    float4 vertices[4] = {
        float4(IN[0].position.xyz + offsetX - offsetY, 1.f), // lower-left
        float4(IN[0].position.xyz + offsetX + offsetY, 1.f), // upper-left
        float4(IN[0].position.xyz - offsetX + offsetY, 1.f), // upper-right
        float4(IN[0].position.xyz - offsetX - offsetY, 1.f), // lower-right
    };

    // tri: 0, 1, 2
    OUT.position = mul(vertices[0], k_viewProjection);
    OUT.textureCoordinate = k_quadUVs[0];
    triStream.Append(OUT);

    OUT.position = mul(vertices[1], k_viewProjection);
    OUT.textureCoordinate = k_quadUVs[1];
    triStream.Append(OUT);

    OUT.position = mul(vertices[2], k_viewProjection);
    OUT.textureCoordinate = k_quadUVs[2];
    triStream.Append(OUT);

    triStream.RestartStrip();

    // tri: 0, 2, 3
    OUT.position = mul(vertices[0], k_viewProjection);
    OUT.textureCoordinate = k_quadUVs[0];
    triStream.Append(OUT);

    OUT.position = mul(vertices[2], k_viewProjection);
    OUT.textureCoordinate = k_quadUVs[2];
    triStream.Append(OUT);

    OUT.position = mul(vertices[3], k_viewProjection);
    OUT.textureCoordinate = k_quadUVs[3];
    triStream.Append(OUT);
}

[maxvertexcount(4)]
void geometry_shader_strip(point VS_OUTPUT IN[1], inout TriangleStream<GS_OUTPUT> triStream)
{
    static const float2 k_quadStripUVs[4] = {
        float2(0.f, 1.f), // v0
        float2(0.f, 0.f), // v1
        float2(1.f, 1.f), // v2
        float2(1.f, 0.f), // v3
    };

    GS_OUTPUT OUT = (GS_OUTPUT)0;

    float2 halfSize = IN[0].size / 2.f;

    float3 direction = k_cameraPosition - IN[0].position.xyz;
    float3 right = cross(normalize(direction), k_cameraUp);

    float3 offsetX = halfSize.x * right;
    float3 offsetY = halfSize.y * k_cameraUp;

    float4 vertices[4] = {
        float4(IN[0].position.xyz + offsetX - offsetY, 1.f), // lower-left
        float4(IN[0].position.xyz + offsetX + offsetY, 1.f), // upper-left
        float4(IN[0].position.xyz - offsetX - offsetY, 1.f), // lower-right
        float4(IN[0].position.xyz - offsetX + offsetY, 1.f), // upper-right
    };

    [unroll]
    for (int i = 0; i < 4; i++)
    {
        OUT.position = mul(vertices[i], k_viewProjection);
        OUT.textureCoordinate = k_quadStripUVs[i];

        triStream.Append(OUT);
    }
}

[maxvertexcount(5)]
void geometry_shader_linestrip(point VS_OUTPUT IN[1], inout LineStream<GS_OUTPUT> lineStream)
{
    GS_OUTPUT OUT = (GS_OUTPUT)0;

    float2 halfSize = IN[0].size / 2.f;

    float3 direction = k_cameraPosition - IN[0].position.xyz;
    float3 right = cross(normalize(direction), k_cameraUp);

    float3 offsetX = halfSize.x * right;
    float3 offsetY = halfSize.y * k_cameraUp;

    float4 vertices[5] = {
        float4(IN[0].position.xyz - offsetX, 1.f), // left
        float4(IN[0].position.xyz + offsetY, 1.f), // upper
        float4(IN[0].position.xyz + offsetX, 1.f), // right
        float4(IN[0].position.xyz - offsetY, 1.f), // bottom
        float4(IN[0].position.xyz - offsetX, 1.f), // left
    };

    [unroll]
    for (int i = 0; i < 5; i++)
    {
        OUT.position = mul(vertices[i], k_viewProjection);
        lineStream.Append(OUT);
    }
}

[maxvertexcount(4)]
void geometry_shader_nosize(
    point VS_NOSIZE_OUTPUT IN[1],
    uint primitiveID : SV_PrimitiveID,
    inout TriangleStream<GS_OUTPUT> triStream
)
{
    static const float2 k_quadStripUVs[4] = {
        float2(0.f, 1.f), // v0
        float2(0.f, 0.f), // v1
        float2(1.f, 1.f), // v2
        float2(1.f, 0.f), // v3
    };

    GS_OUTPUT OUT = (GS_OUTPUT)0;

    float size = float(primitiveID) + 1.f;
    float2 halfSize = size / 2.f;

    float3 direction = k_cameraPosition - IN[0].position.xyz;
    float3 right = cross(normalize(direction), k_cameraUp);

    float3 offsetX = halfSize.x * right;
    float3 offsetY = halfSize.y * k_cameraUp;

    float4 vertices[4] = {
        float4(IN[0].position.xyz + offsetX - offsetY, 1.f), // lower-left
        float4(IN[0].position.xyz + offsetX + offsetY, 1.f), // upper-left
        float4(IN[0].position.xyz - offsetX - offsetY, 1.f), // lower-right
        float4(IN[0].position.xyz - offsetX + offsetY, 1.f), // upper-right
    };

    [unroll]
    for (int i = 0; i < 4; i++)
    {
        OUT.position = mul(vertices[i], k_viewProjection);
        OUT.textureCoordinate = k_quadStripUVs[i];

        triStream.Append(OUT);
    }
}

/************* Pixel Shader *************/

float4 pixel_shader(GS_OUTPUT IN) : SV_Target
{
    return k_colorTexture.Sample(k_colorSampler, IN.textureCoordinate);
}

float4 pixel_shader_linestrip(GS_OUTPUT IN) : SV_Target
{
    return float4(1.f, 1.f, 1.f, 1.f);
}

/************* Techniques *************/

technique11 main11
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(CompileShader(gs_5_0, geometry_shader()));
        SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
    }
}

technique11 main11_strip
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(CompileShader(gs_5_0, geometry_shader_strip()));
        SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
    }
}

technique11 main11_linestrip
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(CompileShader(gs_5_0, geometry_shader_linestrip()));
        SetPixelShader(CompileShader(ps_5_0, pixel_shader_linestrip()));
    }
}


technique11 main11_nosize
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(CompileShader(gs_5_0, geometry_shader_nosize()));
        SetPixelShader(CompileShader(ps_5_0, pixel_shader()));
    }
}