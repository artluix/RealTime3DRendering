#include "TriangleDemo.h"

#include <library/Components/CameraComponent.h>

#include "VertexTypes.h"
#include <library/Application.h>
#include <library/Utils.h>

#include <library/Math/Math.h>

#include <d3dx11effect.h>
#include <d3dcompiler.h>

using namespace library;

void TriangleDemo::InitializeInternal()
{
	InitializeEffect("Basic", true);

	// Look up the technique, pass, and WVP variable from the effect
	m_technique = m_effect->GetTechniqueByName("main11");
	assert("ID3DX11Effect::GetTechniqueByName() could not find the specified technique." && m_technique);

	// get pass
	m_pass = m_technique->GetPassByName("p0");
	assert("ID3DX11Effect::GetPassByName() could not find the specified pass." && m_pass);

	// get abstract variable
	if (auto variable = m_effect->GetVariableByName("WorldViewProjection"))
	{
		// cast variable to specific type
		m_wvpVariable = variable->AsMatrix();
		assert("Invalid effect variable cast." && m_wvpVariable->IsValid());
	}
	else
	{
		assert("ID3DX11Effect::GetVariableByName() could not find the specified variable." && false);
	}

	using Vertex = VertexPositionColor;

	// Create the input layout
	{
		D3DX11_PASS_DESC passDesc;
		m_pass->GetDesc(&passDesc);

		auto hr = GetApp().GetDevice()->CreateInputLayout(
			Vertex::ElementDescriptions.data(),
			unsigned(Vertex::ElementDescriptions.size()),
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize,
			&m_inputLayout
		);
		assert("ID3D11Device::CreateInputLayout() failed." && SUCCEEDED(hr));
	}

	// vertex buffer
	{
		constexpr float l = 1.73205080757f / 2; // sqrt(3) / 2

		constexpr auto vertices = MakeArray(
			// left red
			Vertex(math::Vector4(-l, -0.5f, 0.0f, 1.0f), math::Color(1.0f, 0.0f, 0.0f, 1.0f)),
			// up green
			Vertex(math::Vector4(0.0f, 1.0f, 0.0f, 1.0f), math::Color(0.0f, 1.0f, 0.0f, 1.0f)),
			// right blue
			Vertex(math::Vector4(l, -0.5f, 0.0f, 1.0f), math::Color(0.0f, 0.0f, 1.0f, 1.0f))
		);

		m_primitivesData.clear();
		auto& pd = m_primitivesData.emplace_back(PrimitiveData());

		pd.vertexBuffer = VertexBufferData(GetApp().GetDevice(), MakeArrayBuffer(vertices));
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
