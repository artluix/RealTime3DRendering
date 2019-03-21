#include "StdAfx.h"
#include "library/Components/GridComponent.h"

#include "library/Components/CameraComponent.h"

#include "library/Application.h"
#include "library/Utils.h"
#include "library/Exception.h"
#include "library/VertexTypes.h"

#include "library/Effect/Effect.h"
#include "library/Effect/EffectTechnique.h"
#include "library/Effect/EffectVariable.h"
#include "library/Effect/EffectPass.h"

namespace library
{
	namespace
	{
		constexpr unsigned k_defaultSize = 16;
		constexpr unsigned k_defaultScale = 16;
		const auto k_defaultColor = Color::White;
	}

	GridComponent::GridComponent()
		: m_size(k_defaultSize)
		, m_scale(k_defaultScale)
		, m_color(k_defaultColor)
	{
		m_input.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	}

	GridComponent::GridComponent(
		const unsigned size,
		const unsigned scale,
		const Color& color
	)
		: m_size(size)
		, m_scale(scale)
		, m_color(color)
	{
	}

	GridComponent::~GridComponent() = default;

	//-------------------------------------------------------------------------

	void GridComponent::SetSize(const unsigned size)
	{
		if (m_size != size)
		{
			m_size = size;
			Build();
		}
	}

	void GridComponent::SetScale(const unsigned scale)
	{
		if (m_scale != scale)
		{
			m_scale = scale;
			Build();
		}
	}

	void GridComponent::SetColor(const Color& color)
	{
		if (m_color != color)
		{
			m_color = color;
			Build();
		}
	}

	//-------------------------------------------------------------------------

	void GridComponent::Initialize(const Application& app)
	{
		InitializeMaterial(app, "Basic");
		SceneComponent::Initialize(app);

		Build();
	}

	//-------------------------------------------------------------------------

	void GridComponent::Draw_SetData()
	{
		auto wvp = GetWorldMatrix();
		if (!!m_camera)
			wvp *= m_camera->GetViewProjectionMatrix();

		GetMaterial()->GetWorldViewProjection() << wvp;

		SceneComponent::Draw_SetData();
	}

	//-------------------------------------------------------------------------

	void GridComponent::Build()
	{
		m_input.vertexBuffer.buffer.Reset();

		const float adjustedScale = m_scale * 0.1f;
		const float maxPosition = m_size * adjustedScale / 2;

		m_input.vertexBuffer.elementsCount = 4 * (m_size + 1);

		const unsigned size = sizeof(VertexPositionColor) * m_input.vertexBuffer.elementsCount;

		std::vector<VertexPositionColor> vertices;
		vertices.reserve(m_input.vertexBuffer.elementsCount);

		const DirectX::XMFLOAT4 color(m_color);

		for (unsigned i = 0; i < m_size + 1; i++)
		{
			const float position = maxPosition - (i * adjustedScale);

			// vertical line
			vertices.emplace_back(DirectX::XMFLOAT4(position, maxPosition, 0.0f, 1.0f), color);
			vertices.emplace_back(DirectX::XMFLOAT4(position, -maxPosition, 0.0f, 1.0f), color);

			// horizontal line
			vertices.emplace_back(DirectX::XMFLOAT4(maxPosition, position, 0.0f, 1.0f), color);
			vertices.emplace_back(DirectX::XMFLOAT4(-maxPosition, position, 0.0f, 1.0f), color);
		}

		D3D11_BUFFER_DESC vertexBufferDesc{};
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.ByteWidth = size;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
		vertexSubResourceData.pSysMem = vertices.data();

		auto hr = GetApp()->GetDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexSubResourceData,
			m_input.vertexBuffer.buffer.GetAddressOf()
		);
		if (FAILED(hr))
		{
			throw Exception("ID3D11Device::CreateBuffer() failed.", hr);
		}
	}
} // namespace library
