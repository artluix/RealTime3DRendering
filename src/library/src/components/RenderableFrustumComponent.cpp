#include "StdAfx.h"
#include "library/Components/RenderableFrustumComponent.h"

#include "library/Components/CameraComponent.h"

#include "library/Application.h"
#include "library/Math/Frustum.h"

#include "library/Effect/EffectVariable.h"

#include <array>

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
{}

//-------------------------------------------------------------------------

void RenderableFrustumComponent::InitializeInternal()
{
	InitializeMaterial("Basic");

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

	pd.stride = sizeof(Vertex);

	std::vector<Vertex> vertices;
	vertices.reserve(k_verticesCount);

	const auto& corners = frustum.GetCorners();
	for (const auto& corner : corners)
	{
		vertices.emplace_back(math::Vector4(corner, 1.f), m_color);
	}

	pd.vertexBuffer = VertexBufferData(GetApp().GetDevice(), vertices);
}

void RenderableFrustumComponent::InitializeIndexBuffer()
{
	m_primitivesData.clear();
	auto& pd = m_primitivesData.emplace_back(PrimitiveData());

	pd.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	constexpr std::array<unsigned, k_indicesCount> k_indices = {
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
	};

	pd.indexBuffer = IndexBufferData(GetApp().GetDevice(), k_indices);
}
} // namespace library
