#include "CubeDemo.h"

#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>

#include <library/VertexTypes.h>
#include <library/Application.h>
#include <library/Utils.h>
#include <library/Math/Math.h>

#include <d3dx11effect.h>
#include <d3dcompiler.h>

using namespace library;

namespace
{
constexpr float k_rotationAngle = math::Pi_Div_2;
constexpr float k_movementRate = 0.01f;
} // namespace

void CubeDemo::InitializeInternal()
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

		auto hr = GetApp().GetDevice()->CreateInputLayout(
			inputElementDescriptions.data(),
			inputElementDescriptions.size(),
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize,
			&m_inputLayout
		);
		assert("ID3D11Device::CreateInputLayout() failed." && SUCCEEDED(hr));
	}

	m_primitivesData.clear();
	auto& pd = m_primitivesData.emplace_back(PrimitiveData());

	// index buffer
	{
		constexpr std::array<unsigned, 2 * 3 * 6> indices =
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

		pd.indexBuffer = IndexBufferData(GetApp().GetDevice(), indices);
	}

	// vertex buffer
	{
		using math::Vector4;
		using math::Color;

		constexpr std::array<VertexPositionColor, 8> vertices = {
			// bottom
			VertexPositionColor(Vector4(-1.0f, -1.0f, -1.0f, 1.0f), Color(0.5f, 0.5f, 0.5f, 1.0f)),
			VertexPositionColor(Vector4(-1.0f, 1.0f, -1.0f, 1.0f), Color(0.5f, 1.0f, 0.5f, 1.0f)),
			VertexPositionColor(Vector4(1.0f, 1.0f, -1.0f, 1.0f), Color(1.0f, 1.0f, 0.5f, 1.0f)),
			VertexPositionColor(Vector4(1.0f, -1.0f, -1.0f, 1.0f), Color(1.0f, 0.5f, 0.5f, 1.0f)),

			// top
			VertexPositionColor(Vector4(-1.0f, -1.0f, 1.0f, 1.0f), Color(0.5f, 0.5f, 1.0f, 1.0f)),
			VertexPositionColor(Vector4(-1.0f, 1.0f, 1.0f, 1.0f), Color(0.5f, 1.0f, 1.0f, 1.0f)),
			VertexPositionColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 1.0f, 1.0f)),
			VertexPositionColor(Vector4(1.0f, -1.0f, 1.0f, 1.0f), Color(1.0f, 0.5f, 1.0f, 1.0f)),
		};

		pd.vertexBuffer = VertexBufferData(GetApp().GetDevice(), vertices);
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
			Rotate(math::Quaternion::RotationPitchYawRoll(math::Vector3(rotationDelta)));
		}

		// movement
		{
			math::Vector2i movementAmount;
			if (m_keyboard->IsKeyDown(Key::Up))
				movementAmount.y++;

			if (m_keyboard->IsKeyDown(Key::Down))
				movementAmount.y--;

			if (m_keyboard->IsKeyDown(Key::Left))
				movementAmount.x--;

			if (m_keyboard->IsKeyDown(Key::Right))
				movementAmount.x++;

			if (movementAmount)
				Translate(math::Vector3(movementAmount * k_movementRate));
		}
	}

	PrimitiveComponent::Update(time);
}

void CubeDemo::Draw_SetData(const PrimitiveData& primitiveData)
{
	auto wvp = GetWorldMatrix();
	if (auto camera = GetCamera())
		wvp *= camera->GetViewProjectionMatrix();

	m_wvpVariable->SetMatrix(static_cast<const float*>(wvp));

	SimplePrimitiveComponent::Draw_SetData(primitiveData);
}
