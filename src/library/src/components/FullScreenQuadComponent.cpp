#include "StdAfx.h"
#include "library/Components/FullScreenQuadComponent.h"

#include "library/Effect/Effect.h"
#include "library/Effect/EffectTechnique.h"
#include "library/Effect/EffectPass.h"

#include "library/Materials/Material.h"

#include "library/VertexTypes.h"
#include "library/Application.h"
#include "library/Exception.h"

#include <array>
#include <cassert>

namespace library
{
FullScreenQuadComponent::FullScreenQuadComponent() = default;
FullScreenQuadComponent::~FullScreenQuadComponent() = default;

//-------------------------------------------------------------------------

void FullScreenQuadComponent::SetMaterial(Material& material)
{
	m_material = &material;
}

void FullScreenQuadComponent::SetMaterial(
	Material& material,
	const std::string& techniqueName,
	const std::string& passName /* = "p0"*/
)
{
	SetMaterial(material);
	SetActiveTechnique(techniqueName, passName);
}

//-------------------------------------------------------------------------

void FullScreenQuadComponent::SetActiveTechnique(
	const std::string& techniqueName,
	const std::string& passName /* = "p0"*/
)
{
	assert(!!m_material);

	const auto& technique = m_material->GetEffect().GetTechnique(techniqueName);

	m_effectPass = &technique.GetPass(passName);
	m_inputLayout = m_material->GetInputLayout(*m_effectPass);
}

void FullScreenQuadComponent::SetMaterialUpdateFunction(const MaterialUpdateFunction& func)
{
	m_materialUpdateFunction = func;
}

void FullScreenQuadComponent::Initialize()
{
	PrimitiveComponent::Initialize();

	m_primitivesData.clear();

	auto& pd = m_primitivesData.emplace_back(PrimitiveData());

	// vertex buffer
	{
		using Vertex = VertexPositionTexture;

		std::array<Vertex, 4> vertices = {
			Vertex(DirectX::XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),
			Vertex(DirectX::XMFLOAT4(-1.0f, 1.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),
			Vertex(DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)),
			Vertex(DirectX::XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),
		};
		pd.vertexBuffer.elementsCount = static_cast<unsigned>(vertices.size());

		D3D11_BUFFER_DESC vertexBufferDesc{};
		vertexBufferDesc.ByteWidth = sizeof(Vertex) * pd.vertexBuffer.elementsCount;
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
		vertexSubResourceData.pSysMem = vertices.data();

		auto hr = GetApp()
					  .GetDevice()
					  ->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &pd.vertexBuffer.buffer);
		if (FAILED(hr))
		{
			throw Exception("ID3D11Device::CreateBuffer", hr);
		}
	}

	// index buffer
	{
		pd.indexBuffer = std::make_optional(IndexBufferData());

		std::array<unsigned, 6> indices = {0, 1, 2, 0, 2, 3};
		pd.indexBuffer->elementsCount = static_cast<unsigned>(indices.size());

		D3D11_BUFFER_DESC indexBufferDesc{};
		indexBufferDesc.ByteWidth = sizeof(unsigned) * pd.indexBuffer->elementsCount;
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubResourceData{};
		indexSubResourceData.pSysMem = indices.data();

		auto hr = GetApp()
					  .GetDevice()
					  ->CreateBuffer(&indexBufferDesc, &indexSubResourceData, &pd.indexBuffer->buffer);
		if (FAILED(hr))
		{
			throw Exception("ID3D11Device::CreateBuffer", hr);
		}
	}
}

void FullScreenQuadComponent::Update(const Time& time)
{}

void FullScreenQuadComponent::Draw_SetData(const PrimitiveData& primitiveData)
{
	if (!!m_materialUpdateFunction)
		m_materialUpdateFunction();

	PrimitiveComponent::Draw_SetData(primitiveData);
}
} // namespace library
