#include "TextureModelDemo.h"

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

using namespace library;
using library::byte;

namespace
{
	constexpr float k_rotationAngle = math::Pi_Div_2;
	constexpr float k_movementRate = 0.01f;
	constexpr float k_scaleRate = 0.01f;
}

//-------------------------------------------------------------------------

using VertexType = VertexPositionTexture;

//-------------------------------------------------------------------------

TextureModelDemo::TextureModelDemo()
	: m_wheel(0)
{
	SetTextureName("EarthComposite");
}

void TextureModelDemo::Initialize(const Application& app)
{
	SceneComponent::Initialize(app);

	// shader
	{
		const auto path = app.GetTexturesPath() + Path("TextureMapping.fxc");

		std::vector<byte> effectData;
		utils::LoadBinaryFile(path, effectData);
		if (effectData.empty())
		{
			throw Exception("Load compiled effect failed.");
		}

		auto hr = D3DX11CreateEffectFromMemory(
			//shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
			effectData.data(), effectData.size(),
			0,
			app.GetDevice(),
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
			{
				"POSITION",
				0,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				0,
				0,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			},
			{
				"TEXCOORD",
				0,
				DXGI_FORMAT_R32G32_FLOAT,
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			},
		};

		auto hr = app.GetDevice()->CreateInputLayout(
			inputElementDescriptions.data(), inputElementDescriptions.size(),
			passDesc.pIAInputSignature, passDesc.IAInputSignatureSize,
			m_input.layout.GetAddressOf()
		);
		if (FAILED(hr))
		{
			throw Exception("ID3D11Device::CreateInputLayout() failed.", hr);
		}
	}

	// Load the model
	Model model(app, "Sphere", true);

	// Create the vertex and index buffers
	const auto& mesh = model.GetMesh(0);
	CreateVertexBuffer(app.GetDevice(), mesh);

	if (mesh.HasIndices())
	{
		m_input.indices = std::make_unique<BufferData>();
		m_input.indices->buffer = mesh.CreateIndexBuffer();
		m_input.indices->count = mesh.GetIndicesCount();
	}
}

void TextureModelDemo::Update(const Time& time)
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

	DrawableComponent::Update(time);
}

unsigned TextureModelDemo::GetVertexSize() const
{
	return sizeof(VertexType);
}

void TextureModelDemo::Draw_SetData()
{
	auto deviceContext = m_app->GetDeviceContext();

	auto wvp = GetWorldMatrix();
	if (auto camera = GetCamera())
		wvp *= camera->GetViewProjectionMatrix();
	m_wvpVariable->SetMatrix(static_cast<const float*>(wvp));

	m_colorTextureVariable->SetResource(GetTexture());

	m_pass->Apply(0, deviceContext);
}

void TextureModelDemo::CreateVertexBuffer(const ComPtr<ID3D11Device>& device, const Mesh& mesh)
{
	std::vector<VertexType> vertices;

	if (mesh.HasVertices())
	{
		const auto& meshVertices = mesh.GetVertices();
		const auto& textureCoordinates = mesh.GetTextureCoordinates(0);
		m_input.vertices.count = meshVertices.size();

		vertices.reserve(m_input.vertices.count);

		for (unsigned i = 0; i < m_input.vertices.count; i++)
		{
			const auto& position = meshVertices[i];
			const auto& uv = textureCoordinates[i];
			vertices.emplace_back(
				DirectX::XMFLOAT4(position.x, position.y, position.z, 1.0f),
				DirectX::XMFLOAT2(uv.x, uv.y)
			);
		}

		D3D11_BUFFER_DESC vertexBufferDesc{};
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_input.vertices.count;
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
		vertexSubResourceData.pSysMem = vertices.data();

		auto hr = device->CreateBuffer(
			&vertexBufferDesc,
			&vertexSubResourceData,
			m_input.vertices.buffer.GetAddressOf()
		);
		if (FAILED(hr))
		{
			throw Exception("ID3D11Device::CreateBuffer() failed.");
		}
	}
}
