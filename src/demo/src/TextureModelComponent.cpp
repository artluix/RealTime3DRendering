#include "TextureModelComponent.h"

#include <library/components/CameraComponent.h>
#include <library/components/KeyboardComponent.h>

#include <library/VertexTypes.h>
#include <library/Application.h>
#include <library/Utils.h>
#include <library/Exception.h>
#include <library/Color.h>

#include <library/Model.h>
#include <library/Mesh.h>

#include <d3dx11effect.h>

#include <DDSTextureLoader.h>

namespace demo
{
	using namespace library;

	namespace
	{
		constexpr float k_rotationAngle = math::constants::Pi_Div_2;
		constexpr float k_movementRate = 0.01f;

		const auto k_effectPath = utils::GetExecutableDirectory().Join(
#if defined(DEBUG) || defined(DEBUG)
			filesystem::Path("data/effects/TextureMapping_d.fxc")
#else
			filesystem::Path("data/effects/TextureMapping.fxc")
#endif
		);
		const auto k_modelPath = utils::GetExecutableDirectory().Join(
			filesystem::Path("data/models/Sphere.obj")
		);
		const auto k_texturePath = utils::GetExecutableDirectory().Join(
			//filesystem::Path("data/textures/EarthComposite.jpg")
			filesystem::Path("data/textures/EarthComposite.dds")
		);
	}

	//-------------------------------------------------------------------------

	using VertexType = library::VertexPositionTexture;

	//-------------------------------------------------------------------------

	TextureModelComponent::TextureModelComponent(
		const Application& app,
		const components::CameraComponent& camera,
		const components::KeyboardComponent& keyboard
	)
		: Class(app, camera)
		, m_keyboard(keyboard)
		, m_indicesCount(0)
	{
	}

	void TextureModelComponent::Initialize()
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

		// Load the model
		library::Model model(app, k_modelPath, true);

		// Create the vertex and index buffers
		if (const auto& mesh = model.GetMesh(0))
		{
			CreateVertexBuffer(app.GetD3DDevice(), *mesh);
			m_indexBuffer = mesh->CreateIndexBuffer();
			m_indicesCount = mesh->GetIndicesCount();
		}

		// Load the texture
		{
			std::vector<library::byte> textureData;
			utils::LoadBinaryFile(k_texturePath, textureData);
			if (textureData.empty())
			{
				throw Exception("Load texture failed.");
			}

			auto hr = DirectX::CreateDDSTextureFromMemory(
				app.GetD3DDevice(),
				reinterpret_cast<const std::uint8_t*>(textureData.data()),
				textureData.size(),
				nullptr,
				m_textureShaderResourceView.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("CreateWICTextureFromFile() failed.", hr);
			}
		}
	}

	void TextureModelComponent::Update(const Time& time)
	{
		const components::KeyboardComponent& keyboard = m_keyboard;

		// rotation
		if (keyboard.IsKeyDown(Key::R))
		{
			const auto rotationDelta = k_rotationAngle * time.elapsed.GetSeconds<float>();
			Rotate(rotationDelta);
		}

		// movement
		{
			math::Vector2 movementAmount;
			if (keyboard.IsKeyDown(Key::Up))
			{
				movementAmount.y += 1.0f;
			}

			if (keyboard.IsKeyDown(Key::Down))
			{
				movementAmount.y -= 1.0f;
			}

			if (keyboard.IsKeyDown(Key::Left))
			{
				movementAmount.x -= 1.0f;
			}

			if (keyboard.IsKeyDown(Key::Right))
			{
				movementAmount.x += 1.0f;
			}

			if (movementAmount)
			{
				Translate(math::Vector3(movementAmount * k_movementRate));
			}
		}
	}

	void TextureModelComponent::Draw(const Time& time)
	{
		auto d3dDeviceContext = m_app.GetD3DDeviceContext();

		const auto wvp = m_worldMatrix * GetCamera().GetViewProjectionMatrix();
		m_wvpVariable->SetMatrix(reinterpret_cast<const float*>(&wvp));

		m_colorTextureVariable->SetResource(m_textureShaderResourceView.Get());

		m_pass->Apply(0, d3dDeviceContext);

		d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		d3dDeviceContext->IASetInputLayout(m_inputLayout.Get());

		d3dDeviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		unsigned stride = sizeof(VertexType);
		unsigned offset = 0;
		d3dDeviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

		d3dDeviceContext->DrawIndexed(m_indicesCount, 0, 0);
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
