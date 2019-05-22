#include "DistortionMappingDemo.h"

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
	const auto k_backgroundColor = Color::Black;
	constexpr auto k_emptyColor = Color();
}

DistortionMappingDemo::DistortionMappingDemo()
	: m_displacementScale(1.f)
	, m_mode(Mode::Fullscreen)
{
}

DistortionMappingDemo::~DistortionMappingDemo() = default;

//-------------------------------------------------------------------------

void DistortionMappingDemo::Initialize()
{
	PostProcessingComponent::Initialize();

	InitializeMaterial("DistortionMapping");
	InitializeQuad(app, "distortion");

	m_distortionMapTexture = app.LoadTexture("DistortionGlass");
	//m_distortionMapTexture = app.LoadTexture("TextDistortionMap");

	m_text = std::make_unique<TextComponent>();
	m_text->SetTextUpdateFunction(
		[this]() -> std::wstring
		{
			std::wostringstream woss;
			woss
				<< L"Displacement Scale (+Comma/-Period): " << m_displacementScale << '\n'
				<< L"Display Mode (Space): " << utils::ToWideString(ModeToString(m_mode));
			return woss.str();
		}
	);
	m_text->SetPosition(math::Vector2(0.f, 200.f));
	m_text->Initialize();

	// Load model
	Model model(app, "Sphere", true);
	const auto& mesh = model.GetMesh(0);
	m_vertexBuffer.buffer = m_material->CreateVertexBuffer(app.GetDevice(), mesh);
	m_vertexBuffer.elementsCount = mesh.GetVerticesCount();

	if (mesh.HasIndices())
	{
		m_indices = std::make_optional(
			BufferData{ mesh.CreateIndexBuffer(), mesh.GetIndicesCount() }
		);
	}

	m_cutoutPass = &m_effect->GetTechnique("distortion_cutout").GetPass("p0");
	m_cutoutRenderTarget = std::make_unique<FullScreenRenderTarget>(app);
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

	// Set IA
	{
		auto inputLayout = m_material->GetInputLayout(*m_cutoutPass);
		deviceContext->IASetInputLayout(inputLayout);

		const auto stride = m_material->GetVertexSize();
		unsigned offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer.buffer, &stride, &offset);

		if (m_indices)
		{
			deviceContext->IASetIndexBuffer(m_indices->buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
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
		m_material->GetDistortionMapTexture() << m_distortionMapTexture.Get();

		m_cutoutPass->Apply(0, deviceContext);
	}

	// Render
	if (m_indices)
		deviceContext->DrawIndexed(m_indices->elementsCount, 0, 0);
	else
		deviceContext->Draw(m_vertexBuffer.elementsCount, 0);
}

//-------------------------------------------------------------------------

void DistortionMappingDemo::UpdateDisplacementScale(const Time& time)
{
	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		if (m_keyboard->IsKeyDown(Key::Comma) && m_displacementScale < 3.0f)
		{
			m_displacementScale += elapsedTime;
			m_displacementScale = math::Min(m_displacementScale, 3.0f);
		}

		if (m_keyboard->IsKeyDown(Key::Period) && m_displacementScale > 0.0f)
		{
			m_displacementScale -= elapsedTime;
			m_displacementScale = math::Max(m_displacementScale, 0.0f);
		}
	}
}

void DistortionMappingDemo::UpdateDistortion()
{
	if (m_mode == Mode::Fullscreen)
		m_material->GetDistortionMapTexture() << m_distortionMapTexture.Get();
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
