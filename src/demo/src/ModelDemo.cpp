#include "ModelDemo.h"

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
#include <d3dcompiler.h>

using namespace library;
	
namespace
{
	constexpr float k_rotationAngle = math::Pi_Div_2;
	constexpr float k_movementRate = 0.01f;
}

//-------------------------------------------------------------------------

void ModelDemo::Initialize(const Application& app)
{
	DrawableComponent::Initialize(app);

	// shader
	{
		const auto path = app.GetEffectsPath() + Path("Basic.fx");

		ComPtr<ID3DBlob> errorBlob;
		ComPtr<ID3DBlob> shaderBlob;

		auto hr = D3DCompileFromFile(
			path.GetWideCString(),
			nullptr,
			nullptr,
			nullptr,
			"fx_5_0",
#if defined(DEBUG) || defined(DEBUG)
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
#else
			0,
#endif
			0,
			shaderBlob.GetAddressOf(),
			errorBlob.GetAddressOf()
		);
		if (FAILED(hr))
		{
			std::string error = std::string("D3DX11CompileEffectFromFile() failed: ")
				+ std::string(static_cast<const char*>(errorBlob->GetBufferPointer()), errorBlob->GetBufferSize());
			throw Exception(error.c_str(), hr);
		}

		hr = D3DX11CreateEffectFromMemory(
			shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
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
	m_technique = m_effect->GetTechniqueByName("main11");
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

	// get abstract variable
	if (auto variable = m_effect->GetVariableByName("WorldViewProjection"))
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

	// Create the input layout
	{
		D3DX11_PASS_DESC passDesc;
		m_pass->GetDesc(&passDesc);

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescriptions =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

	// Create vertex and index buffers
	const auto& mesh = model.GetMesh(0);
	CreateVertexBuffer(app.GetDevice(), mesh);

	m_input.indices = std::make_unique<BufferData>();
	m_input.indices->buffer = mesh.CreateIndexBuffer();
	m_input.indices->count = mesh.GetIndicesCount();
}

void ModelDemo::Update(const Time& time)
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
	}

	SceneComponent::Update(time);
}

void ModelDemo::Draw_SetData()
{
	auto wvp = GetWorldMatrix();
	if (auto camera = GetCamera())
		wvp *= camera->GetViewProjectionMatrix();
	m_wvpVariable->SetMatrix(reinterpret_cast<const float*>(&wvp));

	m_pass->Apply(0, m_app->GetDeviceContext());
}

unsigned ModelDemo::GetVertexSize() const
{
	return sizeof(VertexPositionColor);
}

void ModelDemo::CreateVertexBuffer(const ComPtr<ID3D11Device>& device, const Mesh& mesh)
{
	if (mesh.HasVertices())
	{
		std::vector<VertexPositionColor> vertices;

		const auto& meshVertices = mesh.GetVertices();
		const auto verticesCount = meshVertices.size();

		vertices.reserve(verticesCount);

		if (mesh.HasVerticesColors())
		{
			const auto& vertexColors = mesh.GetVertexColors(0);

			for (unsigned i = 0; i < verticesCount; i++)
			{
				const auto& position = meshVertices[i];
				const auto& color = vertexColors[i];
				vertices.emplace_back(DirectX::XMFLOAT4(position.x, position.y, position.z, 1.0f), color);
			}
		}
		else
		{
			for (unsigned i = 0; i < verticesCount; i++)
			{
				const auto& position = meshVertices[i];
				const auto& color = DirectX::XMFLOAT4(Color::Random());
				vertices.emplace_back(DirectX::XMFLOAT4(position.x, position.y, position.z, 1.0f), color);
			}
		}

		D3D11_BUFFER_DESC vertexBufferDesc{};
		vertexBufferDesc.ByteWidth = sizeof(VertexPositionColor) * verticesCount;
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
		vertexSubResourceData.pSysMem = vertices.data();

		auto hr = device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, m_input.vertices.buffer.GetAddressOf());
		if (FAILED(hr))
		{
			throw Exception("ID3D11Device::CreateBuffer() failed.");
		}
	}
}
