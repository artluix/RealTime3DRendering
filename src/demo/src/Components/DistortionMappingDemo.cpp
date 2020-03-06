#include "DistortionMappingDemo.h"

#include "DemoUtils.h"

#include <library/Components/FullScreenQuadComponent.h>
#include <library/Components/TextComponent.h>
#include <library/Components/KeyboardComponent.h>
#include <library/Components/CameraComponent.h>

#include <library/Application.h>
#include <library/Utils.h>
#include <library/Time.h>

#include <library/RenderTargets/FullScreenRenderTarget.h>

#include <library/Model/Model.h>
#include <library/Model/Mesh.h>

#include <library/Effect/Effect.h>
#include <library/Effect/EffectTechnique.h>
#include <library/Effect/EffectPass.h>
#include <library/Effect/EffectVariable.h>

#include <library/Math/Math.h>

#include <sstream>
#include <iomanip>

using namespace library;

namespace
{
constexpr auto k_backgroundColor = colors::Black;
constexpr auto k_emptyColor = math::Color();
} // namespace

DistortionMappingDemo::DistortionMappingDemo()
	: m_displacementScale(1.f)
	, m_mode(Mode::Fullscreen)
{
}

DistortionMappingDemo::~DistortionMappingDemo() = default;

//-------------------------------------------------------------------------

void DistortionMappingDemo::InitializeInternal()
{
	CreateMaterialWithEffect("DistortionMapping");
	InitializeQuad("distortion");

	// Load model
	{
		Model model(GetApp(), "Sphere", true);
		m_primitiveData = m_material->CreatePrimitiveData(GetApp().GetDevice(), model.GetMesh(0));
	}

	m_textures.resize(Texture::Count);
	m_textures[Texture::DistortionMap] = GetApp().CreateTexture2DSRV("DistortionGlass.png");
	//m_textures[Texture::DistortionMap] = GetApp().LoadTexture("TextDistortionMap.png");

	m_text = std::make_unique<TextComponent>();
	m_text->SetTextUpdateFunction(
		[this]() -> std::wstring
		{
			std::wostringstream woss;
			woss
				<< L"Displacement Scale (+Comma/-Period): " << m_displacementScale << '\n'
				<< L"Display Mode (Space): " << library::utils::ToWideString(ModeToString(m_mode));
			return woss.str();
		}
	);
	m_text->SetPosition(math::Vector2(0.f, 200.f));
	m_text->Initialize(GetApp());

	m_cutoutPass = &m_material->GetEffect().GetTechnique("distortion_cutout").GetPass("p0");
	m_cutoutRenderTarget = std::make_unique<FullScreenRenderTarget>(GetApp());
}

void DistortionMappingDemo::Update(const Time& time)
{
	if (!!m_keyboard)
	{
		if (m_keyboard->WasKeyPressed(Key::Space))
			m_mode = NextMode(m_mode);
	}

	m_text->Update(time);

	UpdateDisplacementScale(time);
}

void DistortionMappingDemo::Draw(const Time& time)
{
	auto deviceContext = GetApp().GetDeviceContext();

	switch (m_mode)
	{
		case Mode::Masking:
		{
			m_cutoutRenderTarget->Begin();

			deviceContext->ClearRenderTargetView(
				m_cutoutRenderTarget->GetRenderTargetView(),
				static_cast<const float*>(k_emptyColor)
			);
			deviceContext->ClearDepthStencilView(
				m_cutoutRenderTarget->GetDepthStencilView(),
				D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
				1.f, 0
			);

			DrawMeshForDistortionCutout();

			m_cutoutRenderTarget->End();
		}

		case Mode::Fullscreen:
		{
			m_fullScreenQuad->SetActiveTechnique("distortion");
			m_fullScreenQuad->SetMaterialUpdateFunction(
				std::bind(&DistortionMappingDemo::UpdateDistortion, this)
			);
			break;
		};

		case Mode::MaskOnly:
		{
			m_fullScreenQuad->SetActiveTechnique("no_distortion");
			m_fullScreenQuad->SetMaterialUpdateFunction(
				std::bind(&DistortionMappingDemo::UpdateDistortionMask, this)
			);
			break;
		}

		default:
			return;
	}

	m_fullScreenQuad->Draw(time);

	GetApp().UnbindPixelShaderResources(0, 2);
}

//-------------------------------------------------------------------------

void DistortionMappingDemo::DrawMeshForDistortionCutout()
{
	auto deviceContext = GetApp().GetDeviceContext();

	const auto& vertexBuffer = m_primitiveData.vertexBuffer;
	const auto& indexBuffer = m_primitiveData.indexBuffer;

	// Set IA
	{
		auto inputLayout = m_material->GetInputLayout(*m_cutoutPass);
		deviceContext->IASetInputLayout(inputLayout);

		deviceContext->IASetVertexBuffers(
			0,
			1,
			&vertexBuffer.buffer,
			&vertexBuffer.stride,
			&vertexBuffer.offset
		);

		if (indexBuffer)
		{
			deviceContext->IASetIndexBuffer(indexBuffer->buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		}
	}

	// SetData
	{
		auto wvp = math::Matrix4::Identity;

		if (!!m_camera)
		{
			wvp *= m_camera->GetViewProjectionMatrix();
		}

		m_material->GetWVP() << wvp;
		m_material->GetDistortionMapTexture() << m_textures[Texture::DistortionMap].Get();

		m_cutoutPass->Apply(0, deviceContext);
	}

	// Render
	if (indexBuffer)
		deviceContext->DrawIndexed(indexBuffer->elementsCount, 0, 0);
	else
		deviceContext->Draw(vertexBuffer.elementsCount, 0);
}

//-------------------------------------------------------------------------

void DistortionMappingDemo::UpdateDisplacementScale(const Time& time)
{
	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();
		::utils::UpdateValue(m_displacementScale, elapsedTime, math::Interval(.0f, 3.f), *m_keyboard, KeyPair(Key::Comma, Key::Period));
	}
}

void DistortionMappingDemo::UpdateDistortion()
{
	if (m_mode == Mode::Fullscreen)
		m_material->GetDistortionMapTexture() << m_textures[Texture::DistortionMap].Get();
	else if (m_mode == Mode::Masking)
		m_material->GetDistortionMapTexture() << m_cutoutRenderTarget->GetOutputTexture();

	m_material->GetSceneTexture() << GetSceneTexture();
	m_material->GetDisplacementScale() << m_displacementScale;
}

void DistortionMappingDemo::UpdateDistortionMask()
{
	m_material->GetSceneTexture() << m_cutoutRenderTarget->GetOutputTexture();
}

//-------------------------------------------------------------------------

std::string DistortionMappingDemo::ModeToString(const Mode mode)
{
	switch (mode)
	{
		case Mode::Fullscreen:	return "Fullscreen";
		case Mode::Masking:		return "Masking";
		case Mode::MaskOnly:	return "MaskOnly";
		default:				return "";
	}
}

DistortionMappingDemo::Mode DistortionMappingDemo::NextMode(const Mode mode)
{
	switch (mode)
	{
		case Mode::Fullscreen:	return Mode::Masking;
		case Mode::Masking:		return Mode::MaskOnly;
		case Mode::MaskOnly:	return Mode::Fullscreen;
		default:				return Mode::Fullscreen;
	}
}
