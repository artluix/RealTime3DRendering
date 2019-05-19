#include "StdAfx.h"
#include "library/Components/RenderableFrustumComponent.h"

#include "library/Components/CameraComponent.h"

#include "library/Application.h"
#include "library/Exception.h"

#include "library/Frustum.h"

#include "library/Effect/EffectVariable.h"

#include <array>

namespace library
{

namespace
{
const auto k_defaultColor = Color::Green;

constexpr std::size_t k_verticesCount = 8;

constexpr std::size_t k_primitivesCount = 12;
constexpr std::size_t k_indicesPerPrimitive = 2;
constexpr std::size_t k_indicesCount = k_primitivesCount * k_indicesPerPrimitive;
} // namespace

//-------------------------------------------------------------------------

RenderableFrustumComponent::RenderableFrustumComponent() : RenderableFrustumComponent(k_defaultColor)
{}

RenderableFrustumComponent::RenderableFrustumComponent(const Color& color) : m_color(color)
{}

//-------------------------------------------------------------------------

void RenderableFrustumComponent::Initialize()
{
	InitializeMaterial("Basic");
	PrimitiveComponent::Initialize();

	InitializeIndexBuffer();
}

void RenderableFrustumComponent::Draw_SetData(const PrimitiveData& primitiveData)
{
	auto wvp = GetWorldMatrix();
	if (auto camera = GetCamera())
	{
		wvp *= camera->GetViewProjectionMatrix();
	}

	GetMaterial()->GetWorldViewProjection() << math::XMMatrix(wvp);

	PrimitiveComponent::Draw_SetData(primitiveData);
}

void RenderableFrustumComponent::InitializeGeometry(const Frustum& frustum)
{
	InitializeVertexBuffer(frustum);
}

//-------------------------------------------------------------------------

void RenderableFrustumComponent::InitializeVertexBuffer(const Frustum& frustum)
{
	using Vertex = Material::Vertex;
	using DirectX::XMFLOAT4;

	auto& pd = m_primitivesData.front();

	pd.vertexBuffer.buffer.Reset();

	std::vector<Vertex> vertices;
	vertices.reserve(k_verticesCount);

	for (const auto& corner : frustum.GetCorners())
	{
		vertices.emplace_back(XMFLOAT4(corner.x, corner.y, corner.z, 1.f), XMFLOAT4(m_color));
	}

	D3D11_BUFFER_DESC vertexBufferDesc{};
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * k_verticesCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
	vertexSubResourceData.pSysMem = vertices.data();

	auto hr =
		GetApp().GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &pd.vertexBuffer.buffer);
	if (FAILED(hr))
	{
		throw Exception("ID3D11Device::CreateBuffer() failed.", hr);
	}
}

void RenderableFrustumComponent::InitializeIndexBuffer()
{
	m_primitivesData.clear();
	auto& pd = m_primitivesData.emplace_back(PrimitiveData());

	pd.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	// clang-format off
	constexpr std::array<unsigned, k_indicesCount> k_indices = {// Near plane lines
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
	// clang-format on

	pd.indexBuffer = std::make_optional<IndexBufferData>();
	pd.indexBuffer->elementsCount = k_indicesCount;

	D3D11_BUFFER_DESC indexBufferDesc{};
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = sizeof(unsigned) * k_indicesCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
	vertexSubResourceData.pSysMem = k_indices.data();

	auto hr =
		GetApp().GetDevice()->CreateBuffer(&indexBufferDesc, &vertexSubResourceData, &pd.indexBuffer->buffer);
	if (FAILED(hr))
	{
		throw Exception("ID3D11Device::CreateBuffer() failed.", hr);
	}
}
} // namespace library
