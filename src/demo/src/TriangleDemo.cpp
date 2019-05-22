#include "TriangleDemo.h"

#include <library/Components/CameraComponent.h>

#include <library/VertexTypes.h>
#include <library/Application.h>
#include <library/Utils.h>
#include <library/Exception.h>
#include <library/Math/Math.h>

#include <d3dx11effect.h>
#include <d3dcompiler.h>

using namespace library;

void TriangleDemo::Initialize()
{
	InitializeEffect("Basic", true);

	// Look up the technique, pass, and WVP variable from the effect
	m_technique = m_effect->GetTechniqueByName("main11");
	if (!m_technique)
		throw Exception("ID3DX11Effect::GetTechniqueByName() could not find the specified technique.");

	// get pass
	m_pass = m_technique->GetPassByName("p0");
	if (!m_pass)
		throw Exception("ID3DX11Effect::GetPassByName() could not find the specified pass.");

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

	// vertex buffer
	{
		using DirectX::XMFLOAT4;

		const float l = sqrt(3.f) / 2;

		const std::array<VertexPositionColor, 3> vertices = {
			// left red
			VertexPositionColor(XMFLOAT4(-l, -0.5f, 0.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)),
			// up green
			VertexPositionColor(XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)),
			// right blue
			VertexPositionColor(XMFLOAT4(l, -0.5f, 0.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)),
		};

		m_primitivesData.clear();
		auto& pd = m_primitivesData.emplace_back(PrimitiveData());

		pd.stride = sizeof(VertexPositionColor);

		pd.vertexBuffer.elementsCount = vertices.size();

		D3D11_BUFFER_DESC vertexBufferDesc{};
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.ByteWidth = sizeof(VertexPositionColor) * vertices.size();
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
		vertexSubResourceData.pSysMem = vertices.data();

		auto hr = GetApp().GetDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexSubResourceData,
			&pd.vertexBuffer.buffer);
		if (FAILED(hr))
		{
			throw Exception("ID3D11Device::CreateBuffer() failed.", hr);
		}
	}
}

void TriangleDemo::Update(const Time& time)
{
	const float rotationY = math::Pi_Div_2 * time.elapsed.GetSeconds();
	const auto q = math::Quaternion::RotationPitchYawRoll(0.f, rotationY, 0.f);
	Rotate(q);

	SimplePrimitiveComponent::Update(time);
}

void TriangleDemo::Draw_SetData(const PrimitiveData& meshData)
{
	auto wvp = GetWorldMatrix();
	if (auto camera = GetCamera())
		wvp *= camera->GetViewProjectionMatrix();

	m_wvpVariable->SetMatrix(static_cast<const float*>(wvp));

	m_pass->Apply(0, GetApp().GetDeviceContext());
}
