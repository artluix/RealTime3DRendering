#include "GeometryShaderDemo.h"

#include <library/Components/KeyboardComponent.h>
#include <library/Components/CameraComponent.h>
#include <library/Components/TextComponent.h>

#include <library/Application.h>

#include <library/Effect/Effect.h>
#include <library/Effect/EffectVariable.h>
#include <library/Effect/EffectTechnique.h>
#include <library/Effect/EffectPass.h>

#include <sstream>
#include <iomanip>
#include <random>

using namespace library;

GeometryShaderDemo::GeometryShaderDemo()
	: m_showRandomPoints(true)
{
	SetTextureName("BookCover");
}

//-------------------------------------------------------------------------

void GeometryShaderDemo::Initialize()
{
	m_primitivesData = { PrimitiveData() };
	m_primitivesData.front().primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	InitializeMaterial("PointSprite");

	InitializeRandomPoints();

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction(
		[this]() -> std::wstring
		{
			std::wostringstream woss;
			woss << L"Mode: " << (m_showRandomPoints ? "Random Points" : "Fixed Points");
			return woss.str();
		}
	);
	m_text->Initialize();
}

void GeometryShaderDemo::Update(const Time& time)
{
	if (!!m_keyboard)
	{
		if (m_keyboard->WasKeyPressed(Key::Space))
		{
			std::string techniqueName;

			if (m_showRandomPoints)
			{
				m_showRandomPoints = false;
				InitializeFixedPoints();
				techniqueName = "main11_nosize";
			}
			else
			{
				m_showRandomPoints = true;
				InitializeRandomPoints();
				techniqueName = "main11_strip";
			}

			const auto& technique = m_effect->GetTechnique(techniqueName);
			m_material->SetCurrentTechnique(technique);
			m_currentPass = &technique.GetPass(0);
			m_currentInputLayout = m_material->GetInputLayout(*m_currentPass);
		}
	}

	m_text->Update(time);

	SceneComponent::Update(time);
}

//-------------------------------------------------------------------------

void GeometryShaderDemo::Draw_SetData(const PrimitiveData& primitiveData)
{
	if (!!m_camera)
	{
		m_material->GetCameraPosition() << m_camera->GetPosition();
		m_material->GetCameraUp() << m_camera->GetUp();
		m_material->GetViewProjection() << m_camera->GetViewProjectionMatrix();
	}

	m_material->GetColorTexture() << primitiveData.texture.Get();

	SceneComponent::Draw_SetData(primitiveData);
}

void GeometryShaderDemo::Draw_Render(const library::PrimitiveData& primitiveData)
{
	SceneComponent::Draw_Render(primitiveData);

	GetApp().GetDeviceContext()->GSSetShader(nullptr, nullptr, 0);
}

//-------------------------------------------------------------------------

void GeometryShaderDemo::InitializeRandomPoints()
{
	constexpr unsigned k_maxPointsCount = 100;
	constexpr float k_maxDistance = 10.f;

	constexpr float k_minSize = 2.f;
	constexpr float k_maxSize = 2.f;

	std::random_device randomDevice;
	std::default_random_engine randomGenerator(randomDevice());
	std::uniform_real_distribution<float> distanceDistribution(-k_maxDistance, k_maxDistance);
	std::uniform_real_distribution<float> sizeDistribution(k_minSize, k_maxSize);

	std::vector<VertexPositionSize> vertices;
	vertices.reserve(k_maxPointsCount);

	for (unsigned i = 0; i < k_maxPointsCount; i++)
	{
		const float x = distanceDistribution(randomGenerator);
		const float y = distanceDistribution(randomGenerator);
		const float z = distanceDistribution(randomGenerator);

		const float size = sizeDistribution(randomGenerator);

		vertices.emplace_back(DirectX::XMFLOAT4(x, y, z, 1.f), DirectX::XMFLOAT2(size, size));
	}

	auto& md = m_primitivesData.front();

	md.vertexBuffer.elementsCount = vertices.size();
	md.vertexBuffer.buffer = library::Material::CreateVertexBuffer(
		GetApp().GetDevice(),
		vertices
	);
}

void GeometryShaderDemo::InitializeFixedPoints()
{
	constexpr unsigned k_maxPointsCount = 10;
	constexpr float k_horizontalOffset = 3.f;

	std::vector<VertexPositionSize> vertices;
	vertices.reserve(k_maxPointsCount);

	for (unsigned i = 0; i < k_maxPointsCount; i++)
	{
		vertices.emplace_back(
			DirectX::XMFLOAT4(i * k_horizontalOffset, 0.f, 0.f, 1.f),
			DirectX::XMFLOAT2(math::Vector2::Zero)
		);
	}

	auto& md = m_primitivesData.front();

	md.vertexBuffer.elementsCount = vertices.size();
	md.vertexBuffer.buffer = library::Material::CreateVertexBuffer(
		GetApp().GetDevice(),
		vertices
	);
}
