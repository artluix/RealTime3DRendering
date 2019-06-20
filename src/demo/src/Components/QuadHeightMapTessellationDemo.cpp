#include "QuadHeightMapTessellationDemo.h"

#include <library/Components/TextComponent.h>
#include <library/Components/UIComponent.h>
#include <library/Components/KeyboardComponent.h>
#include <library/Components/CameraComponent.h>

#include <library/Application.h>
#include <library/RasterizerStates.h>

#include <library/Effect/EffectVariable.h>

#include <library/Math/Math.h>

#include <sstream>
#include <iomanip>

using namespace library;

namespace
{
constexpr float k_byteMax = static_cast<float>(0xFF);
constexpr float k_maxTessellationFactor = 64.f;

constexpr float ClampTessellation(const float t)
{
	return math::Clamp(t, 1.f, k_maxTessellationFactor);
}

const RECT k_heightMapDestRect = { 0, 768, 256, 1024 };
constexpr auto k_textureModulationRate = math::Vector2(-0.1f, 0.05f);
} // namespace

QuadHeightMapTessellationDemo::QuadHeightMapTessellationDemo()
	: m_displacementScale(1.f)
	, m_textureMatrix(math::Matrix4::Identity)
	, m_isAnimationEnabled(true)
{
}

void QuadHeightMapTessellationDemo::InitializeInternal()
{
	CreateMaterialWithEffect("QuadHeightMapTessellation");

	SetInitialTransform(math::Matrix4::RotationX(math::Pi_Div_4));

	// build vertices manually
	{
		using Vertex = VertexPositionTexture;

		constexpr std::array<Vertex, 4> vertices =
		{
			Vertex(math::Vector4(-10.f, 1.f, -10.f, 1.f), math::Vector2(0.f, 0.f)), // upper left
			Vertex(math::Vector4(10.f, 1.f, -10.f, 1.f), math::Vector2(1.f, 0.f)),  // upper right
			Vertex(math::Vector4(-10.f, 1.f, 10.f, 1.f), math::Vector2(0.f, 1.f)),  // bottom left
			Vertex(math::Vector4(10.f, 1.f, 10.f, 1.f), math::Vector2(1.f, 1.f)),   // bottom right
		};

		m_primitivesData.clear();
		auto& pd = m_primitivesData.emplace_back(PrimitiveData{});

		pd.stride = sizeof(Vertex);

		pd.vertexBuffer = VertexBufferData(GetApp().GetDevice(), vertices);
		pd.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
	}

	m_textures.resize(Texture::Count);
	m_textures[Texture::HeightMap] = GetApp().LoadTexture("HeightMap");

	m_tessEdgeFactors.fill(10);
	m_tessInsideFactors.fill(10);

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction(
		[this]() -> std::wstring
		{
			std::wostringstream woss;
			woss << std::setprecision(2) <<
				"Tessellation Edge Factors (+Up/-Down, +PgUp/-PgDn): " <<
				"[" << m_tessEdgeFactors[0] << ", " <<
				m_tessEdgeFactors[1] << ", " <<
				m_tessEdgeFactors[2] << ", " <<
				m_tessEdgeFactors[3] << "]" <<
				"\nTessellation Inside Factors: " <<
				"[" << m_tessInsideFactors[0] << ", " << m_tessInsideFactors[1] << "]" <<
				"\nDisplacement Scale (+Right/-Left): " << m_displacementScale <<
				"\nAnimation Enabled (Space): " << (m_isAnimationEnabled ? "True" : "False");
			return woss.str();
		}
	);
	m_text->Initialize(GetApp());

	m_uiHeightMap = std::make_unique<UIComponent>();
	m_uiHeightMap->SetDestinationRect(k_heightMapDestRect);
	m_uiHeightMap->SetTexture(m_textures[Texture::HeightMap].Get());
	m_uiHeightMap->Initialize(GetApp());
}

//-------------------------------------------------------------------------

void QuadHeightMapTessellationDemo::Update(const Time& time)
{
	UpdateDisplacementScale(time);

	if (m_isAnimationEnabled)
	{
		m_texturePosition += k_textureModulationRate * time.elapsed.GetSeconds();
		m_textureMatrix.SetTranslation(math::Vector3(m_texturePosition));
	}

	if (!!m_keyboard)
	{
		if (m_keyboard->WasKeyPressed(Key::Space))
			m_isAnimationEnabled = !m_isAnimationEnabled;

		if (m_keyboard->WasKeyPressed(Key::Up))
		{
			const auto tessellationFactor = ClampTessellation(m_tessEdgeFactors[0] + 1.f);
			m_tessEdgeFactors.fill(tessellationFactor);
			m_tessInsideFactors.fill(tessellationFactor);
		}

		if (m_keyboard->WasKeyPressed(Key::Down))
		{
			const auto tessellationFactor = ClampTessellation(m_tessEdgeFactors[0] - 1.f);
			m_tessEdgeFactors.fill(tessellationFactor);
			m_tessInsideFactors.fill(tessellationFactor);
		}

		if (m_keyboard->WasKeyPressed(Key::PageUp))
		{
			const auto tessellationFactor = ClampTessellation(m_tessEdgeFactors[0] + 5.f);
			m_tessEdgeFactors.fill(tessellationFactor);
			m_tessInsideFactors.fill(tessellationFactor);
		}

		if (m_keyboard->WasKeyPressed(Key::PageDown))
		{
			const auto tessellationFactor = ClampTessellation(m_tessEdgeFactors[0] - 5.f);
			m_tessEdgeFactors.fill(tessellationFactor);
			m_tessInsideFactors.fill(tessellationFactor);
		}
	}

	m_text->Update(time);
	m_uiHeightMap->Update(time);

	PrimitiveComponent::Update(time);
}

void QuadHeightMapTessellationDemo::UpdateDisplacementScale(const Time& time)
{
	if (!!m_keyboard)
	{
		if (m_keyboard->IsKeyDown(Key::Right) && m_displacementScale < k_byteMax)
		{
			m_displacementScale += time.elapsed.GetSeconds();
			m_displacementScale = math::Min(m_displacementScale, k_byteMax);
		}

		if (m_keyboard->IsKeyDown(Key::Left))
		{
			m_displacementScale -= time.elapsed.GetSeconds();
			m_displacementScale = math::Max(m_displacementScale, -k_byteMax);
		}
	}
}

//-------------------------------------------------------------------------

void QuadHeightMapTessellationDemo::Draw_SetData(const library::PrimitiveData& primitiveData)
{
	auto wvp = GetWorldMatrix();
	if (auto camera = GetCamera())
		wvp *= camera->GetViewProjectionMatrix();

	m_material->GetWVP() << wvp;
	m_material->GetTextureMatrix() << m_textureMatrix;
	m_material->GetTessellationEdgeFactors() << m_tessEdgeFactors;
	m_material->GetTessellationInsideFactors() << m_tessInsideFactors;
	m_material->GetHeightMapTexture() << m_textures[Texture::HeightMap].Get();
	m_material->GetDisplacementScale() << m_displacementScale;

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}

void QuadHeightMapTessellationDemo::Draw_Render(const library::PrimitiveData& primitiveData)
{
	auto deviceContext = GetApp().GetDeviceContext();
	deviceContext->RSSetState(RasterizerStates::Wireframe);

	PrimitiveComponent::Draw_Render(primitiveData);
	GetApp().GetDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	GetApp().GetDeviceContext()->DSSetShader(nullptr, nullptr, 0);

	deviceContext->RSSetState(nullptr);
}
