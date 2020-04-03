#include "TextureModelDemo.h"

#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>
#include <library/Components/MouseComponent.h>

#include "VertexTypes.h"
#include <library/Application.h>
#include <library/Utils.h>

#include <library/Math/Math.h>
#include <library/Model/Model.h>
#include <library/Model/Mesh.h>

#include <d3dx11effect.h>

using namespace library;

namespace
{
constexpr float k_rotationAngle = math::Pi_Div_2;
constexpr float k_movementRate = 0.01f;
constexpr float k_scaleRate = 0.01f;
} // namespace

//-------------------------------------------------------------------------

TextureModelDemo::TextureModelDemo() : m_wheel(0)
{}

//-------------------------------------------------------------------------

void TextureModelDemo::InitializeInternal()
{
	InitializeEffect("TextureMapping");

	// Look up the technique, pass, and WVP variable from the effect
	m_technique = m_effect->GetTechniqueByName("main11");
	assert("ID3DX11Effect::GetTechniqueByName() could not find the specified technique." && m_technique);

	// get pass
	m_pass = m_technique->GetPassByName("p0");
	assert("ID3DX11Effect::GetPassByName() could not find the specified pass." && m_pass);

	// get wvp variable
	if (auto variable = m_effect->GetVariableByName("wvp"))
	{
		// cast variable to specific type
		m_wvpVariable = variable->AsMatrix();
		assert("Invalid effect variable cast." && m_wvpVariable->IsValid());
	}
	else
	{
		assert("ID3DX11Effect::GetVariableByName() could not find the specified variable." && false);
	}

	// get color texture variable
	if (auto variable = m_effect->GetVariableByName("ColorTexture"))
	{
		// cast variable to specific type
		m_colorTextureVariable = variable->AsShaderResource();
		assert("Invalid effect variable cast." && m_colorTextureVariable->IsValid());
	}
	else
	{
		assert("ID3DX11Effect::GetVariableByName() could not find the specified variable." && false);
	}

	using Vertex = VertexPositionTexture;

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

	// Load the model
	Model model(GetApp(), "Sphere", true);
	const auto& mesh = model.GetMesh(0);

	m_primitivesData.clear();
	m_primitivesData.emplace_back(mesh.CreatePrimitiveData<Vertex>());

	m_textures.resize(Texture::Count);
	m_textures[Texture::Default] = GetApp().CreateTexture2DSRV("EarthComposite.jpg");
}

void TextureModelDemo::Update(const Time& time)
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

	SimplePrimitiveComponent::Update(time);
}

void TextureModelDemo::Draw_SetData(const PrimitiveData& primitiveData)
{
	auto wvp = GetWorldMatrix();
	if (auto camera = GetCamera())
		wvp *= camera->GetViewProjectionMatrix();

	m_wvpVariable->SetMatrix(static_cast<const float*>(wvp));

	m_colorTextureVariable->SetResource(m_textures[Texture::Default].Get());

	SimplePrimitiveComponent::Draw_SetData(primitiveData);
}
