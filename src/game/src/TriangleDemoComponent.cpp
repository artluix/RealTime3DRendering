#include "TriangleDemoComponent.h"

#include "library/components/CameraComponent.h"

#include "library/VertexTypes.h"
#include "library/Application.h"
#include "library/Utils.h"
#include "library/Exception.h"

#include <d3dx11effect.h>
#include <d3dcompiler.h>

namespace
{
	const auto k_effectPath = library::utils::GetExecutableDirectory().Join(
		library::filesystem::Path(L"data/effects/BasicEffect.fx")
	);
}

TriangleDemoComponent::TriangleDemoComponent(const library::Application& app, const library::components::CameraComponent& camera)
	: Class(app, camera)
	, m_effect()
	, m_technique()
	, m_pass()
	, m_wvpVariable()
	, m_inputLayout()
	, m_vertexBuffer()
{
}

void TriangleDemoComponent::Initialize()
{
	const library::Application& app = m_app;

	// shader
	{
		ComPtr<ID3DBlob> errorBlob;
		ComPtr<ID3DBlob> shaderBlob;

		auto hr = D3DCompileFromFile(
			k_effectPath.GetAsWideString().c_str(),
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
			throw library::Exception(error.c_str(), hr);
		}

		hr = D3DX11CreateEffectFromMemory(
			shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
			0,
			app.GetD3DDevice(),
			m_effect.GetAddressOf()
		);
		if (FAILED(hr))
		{
			throw library::Exception("D3DX11CreateEffectFromMemory() failed.", hr);
		}
	}

	// Look up the technique, pass, and WVP variable from the effect
	m_technique = m_effect->GetTechniqueByName("main11");
	if (!m_technique)
	{
		throw library::Exception("ID3DX11Effect::GetTechniqueByName() could not find the specified technique.");
	}

	// get pass
	m_pass = m_technique->GetPassByName("p0");
	if (!m_pass)
	{
		throw library::Exception("ID3DX11Effect::GetPassByName() could not find the specified pass.");
	}

	// get abstract variable
	if (auto variable = m_effect->GetVariableByName("WorldViewProjection"))
	{
		// cast variable to specific type
		m_wvpVariable = variable->AsMatrix();
		if (!m_wvpVariable->IsValid())
		{
			throw library::Exception("Invalid effect variable cast.");
		}
	}
	else
	{
		throw library::Exception("ID3DX11Effect::GetVariableByName() could not find the specified variable.");
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
			throw library::Exception("ID3D11Device::CreateInputLayout() failed.", hr);
		}
	}

	// vertex buffer
	{
		const float l = sqrt(3.f) / 2;

		std::array<library::VertexPositionColor, 3> vertices =
		{
			library::VertexPositionColor(DirectX::XMFLOAT4(-l, -0.5f, 0.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)), // left red
			library::VertexPositionColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), // up green
			library::VertexPositionColor(DirectX::XMFLOAT4(l, -0.5f, 0.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)), // right blue
		};

		D3D11_BUFFER_DESC vertexBufferDesc{};
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.ByteWidth = sizeof(library::VertexPositionColor) * vertices.size();
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
		vertexSubResourceData.pSysMem = vertices.data();

		auto hr = m_app.GetD3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexSubResourceData,
			m_vertexBuffer.GetAddressOf()
		);
		if (FAILED(hr))
		{
			throw library::Exception("ID3D11Device::CreateBuffer() failed.", hr);
		}
	}
}

void TriangleDemoComponent::Update(const library::Time& time)
{
	auto rotation = m_rotation;
	rotation.z += library::math::constants::Pi_Div_2 * time.elapsed.GetSeconds<float>();
	SetRotation(rotation);
}

void TriangleDemoComponent::Draw(const library::Time& time)
{
	auto d3dDeviceContext = m_app.GetD3DDeviceContext();

	const auto wvp = m_worldMatrix * GetCamera().GetViewProjectionMatrix();
	m_wvpVariable->SetMatrix(reinterpret_cast<const float*>(&wvp));

	m_pass->Apply(0, d3dDeviceContext);

	d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3dDeviceContext->IASetInputLayout(m_inputLayout.Get());

	unsigned stride = sizeof(library::VertexPositionColor);
	unsigned offset = 0;
	d3dDeviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

	d3dDeviceContext->Draw(3, 0);
}
