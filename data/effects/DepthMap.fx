/************* Resources *************/

cbuffer CBufferPerObject
{
    float4x4 worldLightViewProjection;
};

// RasterizerState DepthBias
// {
//     DepthBias = 84000;
// };

/************* Data Structures *************/

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 depth : TEXCOORD;
};

/************* Vertex Shader *************/

float4 vertex_shader(float4 objectPosition : POSITION) : SV_Position
{
    return mul(objectPosition, worldLightViewProjection);
}

VS_OUTPUT render_target_vertex_shader(float4 objectPosition : POSITION)
{
    VS_OUTPUT OUT= (VS_OUTPUT)0;

    OUT.position = mul(objectPosition, worldLightViewProjection);
    OUT.depth = OUT.position.zw;

    return OUT;
}

/************* Pixel Shader *************/

float4 render_target_pixel_shader(VS_OUTPUT IN) : SV_Target
{
    IN.depth.x /= IN.depth.y;

    return float4(IN.depth.x, 0.f, 0.f, 1.f);
}

/************* Techniques *************/

technique11 create_depth_map
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(NULL);
    }
}

technique11 depth_map_render_target
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, render_target_vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, render_target_pixel_shader()));
    }
}

technique11 depth_map_bias
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, vertex_shader()));
        SetGeometryShader(NULL);
        SetPixelShader(NULL);

        // SetRasterizerState(DepthBias);
    }
}


