#include "StdAfx.h"
#include "library/components/GridComponent.h"

#include "library/components/CameraComponent.h"
#include "library/Application.h"
#include "library/Utils.h"
#include "library/Exception.h"
#include "library/VertexTypes.h"

#include <d3dx11effect.h>
#include <vector>

namespace library
{
	namespace components
	{
		namespace
		{
			constexpr unsigned k_defaultSize = 16;
			constexpr unsigned k_defaultScale = 16;
			constexpr auto k_defaultColor = colors::White;
			const auto k_effectPath = utils::GetExecutableDirectory().Join(
#if defined(DEBUG) || defined(DEBUG)
				filesystem::Path("../data/effects/BasicEffect_d.fxc")
#else
				filesystem::Path("../data/effects/BasicEffect.fxc")
#endif
			);
		}

		GridComponent::GridComponent(const Application& app, const CameraComponent& camera)
			: Class(app, camera)
			, m_size(k_defaultSize)
			, m_scale(k_defaultScale)
			, m_color(k_defaultColor)
		{
		}

		GridComponent::GridComponent(
			const Application& app,
			const CameraComponent& camera,
			const unsigned size,
			const unsigned scale,
			const Color& color
		)
			: Class(app, camera)
			, m_size(size)
			, m_scale(scale)
			, m_color(color)
		{
		}

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

		void GridComponent::Initialize()
		{
			const Application& app = m_app;

			// shader
			{
				std::vector<library::byte> effectData;
				utils::LoadBinaryFile(k_effectPath, effectData);
				if (effectData.empty())
				{
					throw Exception("Load compiled effect failed.");
				}

				auto hr = D3DX11CreateEffectFromMemory(
					//shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
					effectData.data(), effectData.size(),
					0,
					app.GetD3DDevice(),
					m_effect.GetAddressOf()
				);
				if (FAILED(hr))
				{
					throw Exception("D3DX11CreateEffectFromMemory() failed.", hr);
				}
			}

			// Look up the technique, pass, and WVP variable from the effect
			m_technique = m_effect->GetTechniqueByName("main11");
			if (!m_technique)
			{
				throw Exception("ID3DX11Effect::GetTechniqueByName() could not find the specified technique.");
			}

			m_pass = m_technique->GetPassByName("p0");
			if (!m_pass)
			{
				throw Exception("ID3DX11Effect::GetPassByName() could not find the specified pass.");
			}

			auto variable = m_effect->GetVariableByName("WorldViewProjection");
			if (!variable)
			{
				throw Exception("ID3DX11Effect::GetVariableByName() could not find the specified variable.");
			}

			m_wvpVariable = variable->AsMatrix();
			if (!m_wvpVariable->IsValid())
			{
				throw Exception("Invalid effect variable cast.");
			}

			// Create the input layout
			{
				D3DX11_PASS_DESC passDesc;
				m_pass->GetDesc(&passDesc);

				std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescriptions =
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				};

				auto hr = app.GetD3DDevice()->CreateInputLayout(
					inputElementDescriptions.data(), inputElementDescriptions.size(),
					passDesc.pIAInputSignature, passDesc.IAInputSignatureSize,
					m_inputLayout.GetAddressOf()
				);
				if (FAILED(hr))
				{
					throw Exception("ID3D11Device::CreateInputLayout() failed.", hr);
				}
			}

			Build();
		}

		void GridComponent::Draw(const Time& time)
		{
			auto d3dDeviceContext = m_app.GetD3DDeviceContext();

			const auto wvp = m_worldMatrix * GetCamera().GetViewProjectionMatrix();
			m_wvpVariable->SetMatrix(reinterpret_cast<const float*>(&wvp));

			m_pass->Apply(0, d3dDeviceContext);

			d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			d3dDeviceContext->IASetInputLayout(m_inputLayout.Get());

			unsigned stride = sizeof(VertexPositionColor);
			unsigned offset = 0;
			d3dDeviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

			d3dDeviceContext->Draw((m_size + 1) * 4, 0);
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

			auto hr = m_app.GetD3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, m_vertexBuffer.GetAddressOf());
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device::CreateBuffer() failed.", hr);
			}
		}
	} // namespace components
} // namespace library
