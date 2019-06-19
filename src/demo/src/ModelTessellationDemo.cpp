#include "ModelTessellationDemo.h"

#include <library/Components/TextComponent.h>
#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>

#include <library/Application.h>
#include <library/RasterizerStates.h>

#include <library/Effect/EffectVariable.h>

#include <library/Math/Math.h>

#include <library/Model/Model.h>

#include <sstream>
#include <iomanip>

using namespace library;

namespace
{
constexpr float k_maxTessFactor = 64.f;
constexpr float k_minTessFactor = 1.f;

constexpr float k_minTessDistance = 1.f;
} // namespace

ModelTessellationDemo::ModelTessellationDemo()
	: m_maxTessFactor(64.f)
	, m_minTessDistance(2.f)
	, m_maxTessDistance(20.f)
	, m_tessInsideFactor(2.f)
{
}

void ModelTessellationDemo::InitializeInternal()
{
	CreateMaterialWithEffect("ModelTessellation");

	// load model
	{
		Model model(GetApp(), "Sphere", true);
		m_primitivesData = m_material->CreatePrimitivesData(GetApp().GetDevice(), model);

		m_primitivesData.front().primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
	}

	m_textures.resize(Texture::Count);
	m_textures[Texture::Default] = GetApp().LoadTexture("EarthComposite");

	m_tessEdgeFactors.fill(2.f);

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction(
		[this]() -> std::wstring
		{
			std::wostringstream woss;
			woss << "Wireframe visible (Enter): " << (m_isWireframeVisible ? "True" : "False")
				 << "\nTessellation Mode (Space): " << (m_isDistanceModeEnabled ? "Distance" : "Manual");
		
			if (m_isDistanceModeEnabled)
			{
				woss << "\nMax Tessellation Factor (+I/-J): " << m_maxTessFactor
					 << "\nMin Tessellation Distance (+O/-K): " << m_minTessDistance
					 << "\nMax Tessellation Distance (+P/-L): " << m_maxTessDistance;
			}
			else
			{
				woss << std::setprecision(2) << "\nTessellation Edge Factors (+Up/-Down, +PgUp/-PgDn): " <<
					"[" << m_tessEdgeFactors[0] << ", " <<
					m_tessEdgeFactors[1] << ", " <<
					m_tessEdgeFactors[2] << "]" <<
					"\nTessellation Inside Factor: " << m_tessInsideFactor;
			}

			return woss.str();
		}
	);
	m_text->Initialize(GetApp());

	UpdateTechnique();
}

//-------------------------------------------------------------------------

void ModelTessellationDemo::Update(const Time& time)
{
	if (!!m_keyboard)
	{
		if (m_keyboard->WasKeyPressed(Key::Space))
		{
			m_isDistanceModeEnabled = !m_isDistanceModeEnabled;
			UpdateTechnique();
		}

		if (m_keyboard->WasKeyPressed(Key::Return))
		{
			m_isWireframeVisible = !m_isWireframeVisible;
			UpdateTechnique();
		}

		if (m_isDistanceModeEnabled)
		{
			if (m_keyboard->IsKeyDown(Key::I))
				m_maxTessFactor = math::Min(m_maxTessFactor + 1.f, k_maxTessFactor);
			if (m_keyboard->IsKeyDown(Key::J))
				m_maxTessFactor = math::Max(m_maxTessFactor - 1.f, k_minTessFactor);

			if (m_keyboard->IsKeyDown(Key::O))
				m_minTessDistance = math::Min(m_minTessDistance + 1.f, m_maxTessDistance);
			if (m_keyboard->IsKeyDown(Key::K))
				m_minTessDistance = math::Max(m_minTessDistance - 1.f, k_minTessDistance);

			if (m_keyboard->IsKeyDown(Key::P))
				m_maxTessDistance += 1.f;
			if (m_keyboard->IsKeyDown(Key::L))
				m_maxTessDistance = math::Max(m_maxTessDistance - 1.f, m_minTessDistance);
		}
		else
		{
			if (m_keyboard->WasKeyPressed(Key::Up))
			{
				const auto tessFactor = math::Min(m_tessEdgeFactors[0] + 1.f, m_maxTessFactor);
				m_tessEdgeFactors.fill(tessFactor);
				m_tessInsideFactor = tessFactor;
			}

			if (m_keyboard->WasKeyPressed(Key::Down))
			{
				const auto tessFactor = math::Max(m_tessEdgeFactors[0] - 1.f, k_minTessFactor);
				m_tessEdgeFactors.fill(tessFactor);
				m_tessInsideFactor = tessFactor;
			}

			if (m_keyboard->WasKeyPressed(Key::PageUp))
			{
				const auto tessFactor = math::Min(m_tessEdgeFactors[0] + 5.f, m_maxTessFactor);
				m_tessEdgeFactors.fill(tessFactor);
				m_tessInsideFactor = tessFactor;
			}

			if (m_keyboard->WasKeyPressed(Key::PageDown))
			{
				const auto tessFactor = math::Max(m_tessEdgeFactors[0] - 5.f, k_minTessFactor);
				m_tessEdgeFactors.fill(tessFactor);
				m_tessInsideFactor = tessFactor;
			}
		}
	}

	m_text->Update(time);

	PrimitiveComponent::Update(time);
}

void ModelTessellationDemo::UpdateTechnique()
{
	if (m_isWireframeVisible)
	{
		if (m_isDistanceModeEnabled)
			m_material->SetCurrentTechnique("solid_color_distance_factors");
		else
			m_material->SetCurrentTechnique("solid_color_manual_factors");
	}
	else
	{
		m_material->SetCurrentTechnique("textured_manual_factors");
	}
}

//-------------------------------------------------------------------------

void ModelTessellationDemo::Draw_SetData(const PrimitiveData& primitiveData)
{
	const auto& world = GetWorldMatrix();
	auto wvp = world;

	if (auto camera = GetCamera())
	{
		wvp *= camera->GetViewProjectionMatrix();

		m_material->GetCameraPosition() << camera->GetPosition();
	}

	m_material->GetWVP() << wvp;
	m_material->GetWorld() << world;
	m_material->GetTessellationEdgeFactors() << m_tessEdgeFactors;
	m_material->GetTessellationInsideFactors() << m_tessInsideFactor;
	m_material->GetMaxTessFactor() << m_maxTessFactor;
	m_material->GetMinTessDistance() << m_minTessDistance;
	m_material->GetMaxTessDistance() << m_maxTessDistance;
	m_material->GetColorTexture() << m_textures[Texture::Default].Get();

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}

void ModelTessellationDemo::Draw_Render(const PrimitiveData& primitiveData)
{
	auto deviceContext = GetApp().GetDeviceContext();

	if (m_isWireframeVisible)
		deviceContext->RSSetState(RasterizerStates::Wireframe);

	PrimitiveComponent::Draw_Render(primitiveData);
	GetApp().GetDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	GetApp().GetDeviceContext()->DSSetShader(nullptr, nullptr, 0);

	if (m_isWireframeVisible)
		deviceContext->RSSetState(nullptr);
}

