#include "components/TextureModelComponent.h"

#include <library/components/CameraComponent.h>
#include <library/components/KeyboardComponent.h>
#include <library/components/MouseComponent.h>

#include <library/VertexTypes.h>
#include <library/Application.h>
#include <library/Utils.h>
#include <library/Exception.h>
#include <library/Color.h>

#include <library/Model.h>
#include <library/Mesh.h>

#include <d3dx11effect.h>

namespace demo
{
	using namespace library;

	namespace
	{
		constexpr float k_rotationAngle = math::Pi_Div_2;
		constexpr float k_movementRate = 0.01f;

		constexpr float k_scaleRate = 0.01f;

		const auto k_effectPath = utils::GetExecutableDirectory().Join(
#if defined(DEBUG) || defined(DEBUG)
			Path("../data/effects/TextureMapping_d.fxc")
#else
			Path("../data/effects/TextureMapping.fxc")
#endif
		);
		const auto k_modelPath = utils::GetExecutableDirectory().Join(Path("../data/models/Sphere.obj"));
		const auto k_texturePath = utils::GetExecutableDirectory().Join(Path("../data/textures/EarthComposite.dds"));
	}

	//-------------------------------------------------------------------------

	using VertexType = VertexPositionTexture;

	//-------------------------------------------------------------------------

	TextureModelComponent::TextureModelComponent(const Application& app)
		: SceneComponent()
		, DrawableComponent(app)
		, InputReceivableComponent()
		, m_indicesCount(0)
		, m_wheel(0)
	{
	}

	void TextureModelComponent::Initialize()
	{
		m_app.LoadTexture(k_texturePath, m_textureShaderResourceView);

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
				m_app.GetDevice(),
				m_effect.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("D3DX11CreateEffectFromMemory() failed.", hr);
			}
		}

		// Look up the technique, pass, and WVP variable from the effect
		m_technique = m_effect->GetTechniqueByName("main10");
		if (!m_technique)
		{
			throw Exception("ID3DX11Effect::GetTechniqueByName() could not find the specified technique.");
		}

		// get pass
		m_pass = m_technique->GetPassByName("p0");
		if (!m_pass)
		{
			throw Exception("ID3DX11Effect::GetPassByName() could not find the specified pass.");
		}

		// get wvp variable
		if (auto variable = m_effect->GetVariableByName("wvp"))
		{
			// cast variable to specific type
			m_wvpVariable = variable->AsMatrix();
			if (!m_wvpVariable->IsValid())
			{
				throw Exception("Invalid effect variable cast.");
			}
		}
		else
		{
			throw Exception("ID3DX11Effect::GetVariableByName() could not find the specified variable.");
		}

		// get color texture variable
		if (auto variable = m_effect->GetVariableByName("ColorTexture"))
		{
			// cast variable to specific type
			m_colorTextureVariable = variable->AsShaderResource();
			if (!m_colorTextureVariable->IsValid())
			{
				throw Exception("Invalid effect variable cast.");
			}
		}
		else
		{
			throw Exception("ID3DX11Effect::GetVariableByName() could not find the specified variable.");
		}

		// Create the input layout
		{
			D3DX11_PASS_DESC passDesc;
			m_pass->GetDesc(&passDesc);

			std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescriptions =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			auto hr = m_app.GetDevice()->CreateInputLayout(
				inputElementDescriptions.data(), inputElementDescriptions.size(),
				passDesc.pIAInputSignature, passDesc.IAInputSignatureSize,
				m_inputLayout.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device::CreateInputLayout() failed.", hr);
			}
		}

		// Load the model
		Model model(m_app, k_modelPath, true);

		// Create the vertex and index buffers
		const auto& mesh = model.GetMesh(0);
		CreateVertexBuffer(m_app.GetDevice(), mesh);
		m_indexBuffer = mesh.CreateIndexBuffer();
		m_indicesCount = mesh.GetIndicesCount();

		DrawableComponent::Initialize();
	}

	void TextureModelComponent::Update(const Time& time)
	{
		if (!!m_keyboard)
		{
			// rotation
			if (m_keyboard->IsKeyDown(Key::R))
			{
				const auto rotationDelta = k_rotationAngle * time.elapsed.GetSeconds();
				Rotate(rotationDelta);
			}

			// movement
			{
				math::Vector2 movementAmount;
				if (m_keyboard->IsKeyDown(Key::Up))
				{
					movementAmount.y += 1.0f;
				}

				if (m_keyboard->IsKeyDown(Key::Down))
				{
					movementAmount.y -= 1.0f;
				}

				if (m_keyboard->IsKeyDown(Key::Left))
				{
					movementAmount.x -= 1.0f;
				}

				if (m_keyboard->IsKeyDown(Key::Right))
				{
					movementAmount.x += 1.0f;
				}

				if (movementAmount)
				{
					Translate(math::Vector3(movementAmount * k_movementRate));
				}
			}

			// scaling
			if (!!m_mouse)
			{
				const auto wheel = m_mouse->GetWheel();
				if (m_wheel != wheel)
				{
					if (m_keyboard->IsKeyDown(Key::Alt_Left))
					{
						const float delta = float(wheel - m_wheel) * k_scaleRate;
						Scale(math::Vector3(delta));
					}

					m_wheel = wheel;
				}
			}
		}
	}

	void TextureModelComponent::Draw(const Time& time)
	{
		auto deviceContext = m_app.GetDeviceContext();

		auto wvp = GetWorldMatrix();
		if (auto camera = GetCamera())
			wvp *= camera->GetViewProjectionMatrix();
		m_wvpVariable->SetMatrix(static_cast<const float*>(wvp));

		m_colorTextureVariable->SetResource(m_textureShaderResourceView.Get());

		m_pass->Apply(0, deviceContext);

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deviceContext->IASetInputLayout(m_inputLayout.Get());

		deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		unsigned stride = sizeof(VertexType);
		unsigned offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

		deviceContext->DrawIndexed(m_indicesCount, 0, 0);
	}

	void TextureModelComponent::CreateVertexBuffer(const ComPtr<ID3D11Device>& device, const Mesh& mesh)
	{
		std::vector<VertexType> vertices;

		if (mesh.HasVertices())
		{
			const auto& meshVertices = mesh.GetVertices();
			const auto& textureCoordinates = mesh.GetTextureCoordinates(0);
			const auto verticesCount = meshVertices.size();

			vertices.reserve(verticesCount);

			for (unsigned i = 0; i < verticesCount; i++)
			{
				const auto& position = meshVertices[i];
				const auto& uv = textureCoordinates[i];
				vertices.emplace_back(
					DirectX::XMFLOAT4(position.x, position.y, position.z, 1.0f),
					DirectX::XMFLOAT2(uv.x, uv.y)
				);
			}

			D3D11_BUFFER_DESC vertexBufferDesc{};
			vertexBufferDesc.ByteWidth = sizeof(VertexType) * verticesCount;
			vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
			vertexSubResourceData.pSysMem = vertices.data();

			auto hr = device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, m_vertexBuffer.GetAddressOf());
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device::CreateBuffer() failed.");
			}
		}
	}
} // namespace demo
