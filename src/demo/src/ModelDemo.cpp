#include "ModelDemo.h"

#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>

#include <library/VertexTypes.h>
#include <library/Application.h>
#include <library/Utils.h>
#include <library/Exception.h>
#include <library/math/Math.h>

#include <library/Model/Model.h>
#include <library/Model/Mesh.h>

#include <d3dx11effect.h>
#include <d3dcompiler.h>

using namespace library;

namespace
{
constexpr float k_rotationAngle = math::Pi_Div_2;
constexpr float k_movementRate = 0.01f;
} // namespace

//-------------------------------------------------------------------------

void ModelDemo::Initialize()
{
	InitializeEffect("Basic", true);

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

		// clang-format off
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
				"COLOR",
				0,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			},
		};
		// clang-format on

		auto hr = GetApp().GetDevice()->CreateInputLayout(
			inputElementDescriptions.data(),
			inputElementDescriptions.size(),
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize,
			&m_inputLayout);
		if (FAILED(hr))
		{
			throw Exception("ID3D11Device::CreateInputLayout() failed.", hr);
		}
	}

	// Load the model
	Model model(GetApp(), "Sphere", true);

	// Create vertex and index buffers
	const auto& mesh = model.GetMesh(0);
	CreatePrimitivesData(GetApp().GetDevice(), mesh);
}

void ModelDemo::Update(const Time& time)
{
	if (!!m_keyboard)
	{
		// rotation
		if (m_keyboard->IsKeyDown(Key::R))
		{
			const auto rotationDelta = k_rotationAngle * time.elapsed.GetSeconds();
			Rotate(math::Quaternion::RotationPitchYawRoll(math::Vector3(rotationDelta)));
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

	SimplePrimitiveComponent::Update(time);
}

void ModelDemo::Draw_SetData(const PrimitiveData& primitiveData)
{
	auto wvp = GetWorldMatrix();
	if (auto camera = GetCamera())
		wvp *= camera->GetViewProjectionMatrix();

	m_wvpVariable->SetMatrix(static_cast<const float*>(wvp));

	SimplePrimitiveComponent::Draw_SetData(primitiveData);
}

void ModelDemo::CreatePrimitivesData(ID3D11Device* const device, const Mesh& mesh)
{
	using VertexType = VertexPositionColor;

	if (mesh.HasVertices())
	{
		std::vector<VertexType> vertices;

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
				const auto& color = DirectX::XMFLOAT4(math::Color::Random());
				vertices.emplace_back(DirectX::XMFLOAT4(position.x, position.y, position.z, 1.0f), color);
			}
		}

		D3D11_BUFFER_DESC vertexBufferDesc{};
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * verticesCount;
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
		vertexSubResourceData.pSysMem = vertices.data();

		m_primitivesData.clear();
		auto& pd = m_primitivesData.emplace_back(PrimitiveData());

		pd.stride = sizeof(VertexType);

		if (mesh.HasIndices())
			pd.indexBuffer = mesh.CreateIndexBufferData();

		pd.vertexBuffer.elementsCount = verticesCount;

		auto hr = device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &pd.vertexBuffer.buffer);
		if (FAILED(hr))
			throw Exception("ID3D11Device::CreateBuffer() failed.");
	}
}
