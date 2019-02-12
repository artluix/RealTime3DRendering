#include "StdAfx.h"
#include "library/components/GridComponent.h"

#include "library/components/CameraComponent.h"

#include "library/Application.h"
#include "library/Utils.h"
#include "library/Exception.h"
#include "library/VertexTypes.h"

#include "library/effect/Effect.h"
#include "library/effect/EffectTechnique.h"
#include "library/effect/EffectVariable.h"
#include "library/effect/EffectPass.h"

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
		MaterialComponent::Initialize(app);

		Build();
	}

	void GridComponent::Update(const Time& time)
	{
		AddToRenderer();
	}

	void GridComponent::Draw(const Time& time)
	{
		auto deviceContext = m_app->GetDeviceContext();

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		deviceContext->IASetInputLayout(m_inputLayout.Get());

		unsigned stride = sizeof(VertexPositionColor);
		unsigned offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

		auto wvp = GetWorldMatrix();
		if (auto camera = GetCamera())
			wvp *= camera->GetViewProjectionMatrix();

		m_material->GetWorldViewProjection() << wvp;

		auto& currentTechnique = m_material->GetCurrentTechnique();
		auto& pass = currentTechnique.GetPass(0);
		pass.Apply(0, deviceContext);

		deviceContext->Draw((m_size + 1) * 4, 0);
	}

	//-------------------------------------------------------------------------

	void GridComponent::Build()
	{
		m_vertexBuffer.Reset();

		const float adjustedScale = m_scale * 0.1f;
		const float maxPosition = m_size * adjustedScale / 2;

		const unsigned length = 4 * (m_size + 1);
		const unsigned size = sizeof(VertexPositionColor) * length;

		std::vector<VertexPositionColor> vertices;
		vertices.reserve(length);

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

		auto hr = m_app->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, m_vertexBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			throw Exception("ID3D11Device::CreateBuffer() failed.", hr);
		}
	}
} // namespace library
