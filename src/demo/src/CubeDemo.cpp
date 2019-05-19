#include "CubeDemo.h"

#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>

#include <library/VertexTypes.h>
#include <library/Application.h>
#include <library/Utils.h>
#include <library/Exception.h>

#include <d3dx11effect.h>
#include <d3dcompiler.h>

using namespace library;

namespace
{
	constexpr float k_rotationAngle = math::Pi_Div_2;
	constexpr float k_movementRate = 0.01f;
}

void CubeDemo::Initialize()
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
			&shaderBlob,
			&errorBlob
		);
		if (FAILED(hr))
		{
			std::string error =
				std::string("D3DX11CompileEffectFromFile() failed: ") +
				std::string(
					static_cast<const char*>(errorBlob->GetBufferPointer()), errorBlob->GetBufferSize()
				);
			throw Exception(error.c_str(), hr);
		}

		hr = D3DX11CreateEffectFromMemory(
			shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
			0,
			app.GetDevice(),
			&m_effect
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

		auto hr = app.GetDevice()->CreateInputLayout(
			inputElementDescriptions.data(), inputElementDescriptions.size(),
			passDesc.pIAInputSignature, passDesc.IAInputSignatureSize,
			&m_inputLayout
		);
		if (FAILED(hr))
		{
			throw Exception("ID3D11Device::CreateInputLayout() failed.", hr);
		}
	}

	m_meshesData = { PrimitiveData() };
	auto& md = m_meshesData.front();

	// index buffer
	{
		constexpr std::array<short, 2 * 3 * 6> k_indices =
		{
			0, 1, 2,
			0, 2, 3,

			4, 5, 6,
			4, 6, 7,

			0, 4, 5,
			0, 5, 1,

			3, 7, 6,
			3, 6, 2,

			0, 4, 7,
			0, 7, 3,

			1, 5, 6,
			1, 6, 2
		};


		md.indexBuffer = std::make_optional(BufferData());
		md.indexBuffer->elementsCount = k_indices.size();

		D3D11_BUFFER_DESC indexBufferDesc{};
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.ByteWidth = sizeof(short) * k_indices.size();
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
		vertexSubResourceData.pSysMem = k_indices.data();

		auto hr = app.GetDevice()->CreateBuffer(
			&indexBufferDesc,
			&vertexSubResourceData,
			&md.indexBuffer->buffer
		);
		if (FAILED(hr))
		{
			throw Exception("ID3D11Device::CreateBuffer() failed.", hr);
		}
	}

	// vertex buffer
	{
		using DirectX::XMFLOAT4;

		const std::array<VertexPositionColor, 8> vertices =
		{
			// bottom
			VertexPositionColor(XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f)),
			VertexPositionColor(XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f)),
			VertexPositionColor(XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.5f, 1.0f)),
			VertexPositionColor(XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f)),

			// top
			VertexPositionColor(XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f)),
			VertexPositionColor(XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0.5f, 1.0f, 1.0f, 1.0f)),
			VertexPositionColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)),
			VertexPositionColor(XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.5f, 1.0f, 1.0f)),
		};

		md.vertexBuffer.elementsCount = vertices.size();

		D3D11_BUFFER_DESC vertexBufferDesc{};
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.ByteWidth = sizeof(VertexPositionColor) * vertices.size();
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
		vertexSubResourceData.pSysMem = vertices.data();

		auto hr = app.GetDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexSubResourceData,
			&md.vertexBuffer.buffer
		);
		if (FAILED(hr))
		{
			throw Exception("ID3D11Device::CreateBuffer() failed.", hr);
		}
	}
}

void CubeDemo::Update(const Time& time)
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

void CubeDemo::Draw_SetData(const PrimitiveData& meshData)
{
	auto wvp = GetWorldMatrix();
	if (!!m_camera)
		wvp *= m_camera->GetViewProjectionMatrix();
	m_wvpVariable->SetMatrix(reinterpret_cast<const float*>(&wvp));

	m_pass->Apply(0, GetApp()->GetDeviceContext());
}

unsigned CubeDemo::GetVertexSize() const
{
	return sizeof(VertexPositionColor);
}
