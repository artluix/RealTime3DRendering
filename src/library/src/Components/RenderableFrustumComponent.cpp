#include "StdAfx.h"
#include "library/Components/RenderableFrustumComponent.h"

#include "library/Components/CameraComponent.h"

#include "library/Application.h"
#include "library/Math/Frustum.h"

#include "library/Effect/EffectVariable.h"

namespace library
{

namespace
{
constexpr auto k_defaultColor = colors::Green;

constexpr std::size_t k_verticesCount = 8;

constexpr std::size_t k_primitivesCount = 12;
constexpr std::size_t k_indicesPerPrimitive = 2;
constexpr std::size_t k_indicesCount = k_primitivesCount * k_indicesPerPrimitive;
} // namespace

//-------------------------------------------------------------------------

RenderableFrustumComponent::RenderableFrustumComponent()
	: RenderableFrustumComponent(k_defaultColor)
{}

RenderableFrustumComponent::RenderableFrustumComponent(const math::Color& color)
	: m_color(color)
{
	auto& pd = m_primitivesData.emplace_back(PrimitiveData());
	pd.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
}

//-------------------------------------------------------------------------

void RenderableFrustumComponent::InitializeInternal()
{
	CreateMaterialWithEffect("Basic");

	InitializeIndexBuffer();
}

void RenderableFrustumComponent::Draw_SetData(const PrimitiveData& primitiveData)
{
	auto wvp = GetWorldMatrix();
	if (auto camera = GetCamera())
		wvp *= camera->GetViewProjectionMatrix();

	m_material->GetWorldViewProjection() << wvp;

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}

void RenderableFrustumComponent::InitializeGeometry(const math::Frustum& frustum)
{
	InitializeVertexBuffer(frustum);
}

//-------------------------------------------------------------------------

void RenderableFrustumComponent::InitializeVertexBuffer(const math::Frustum& frustum)
{
	using Vertex = Material::Vertex;

	auto& pd = m_primitivesData.front();

	DynArray<Vertex> vertices;
	vertices.reserve(k_verticesCount);

	const auto& corners = frustum.GetCorners();
	for (const auto& corner : corners)
	{
		vertices.emplace_back(math::Vector4(corner, 1.f), m_color);
	}

	pd.vertexBuffer = VertexBufferData(GetApp().GetDevice(), MakeArrayBuffer(vertices));
}

void RenderableFrustumComponent::InitializeIndexBuffer()
{
	constexpr Array<int, k_indicesCount> indices = MakeArray(
		// Near plane lines
		0, 1,
		1, 2,
		2, 3,
		3, 0,

		// Sides
		0, 4,
		1, 5,
		2, 6,
		3, 7,

		// Far plane lines
		4, 5,
		5, 6,
		6, 7,
		7, 4
	);

	m_primitivesData[0].indexBuffer = IndexBufferData(GetApp().GetDevice(), MakeArrayBuffer(indices));
}
} // namespace library
