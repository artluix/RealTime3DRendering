#include "ProjectiveTextureMappingDemo.h"

#include "DemoUtils.h"

#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>

#include <library/Components/TextComponent.h>
#include <library/Components/ProjectorComponent.h>
#include <library/Components/ProxyModelComponent.h>
#include <library/Components/RenderableFrustumComponent.h>
#include <library/Components/PointLightComponent.h>

#include <library/Effect/EffectVariable.h>

#include <library/Application.h>

#include <sstream>
#include <iomanip>

using namespace library;

namespace
{
constexpr float k_byteMax = static_cast<float>(0xFF);
constexpr float k_lightModulationRate = 10.f;
constexpr float k_lightMovementRate = 10.f;
constexpr auto k_lightRotationRate = math::Vector2(math::Pi_2);
}

//-------------------------------------------------------------------------

ProjectiveTextureMappingDemo::ProjectiveTextureMappingDemo()
	: m_projectorFrustum(math::Matrix4::Identity)
	, m_projectedTextureScalingMatrix(math::Matrix4::Identity)

	, m_specularPower(25.f)
	, m_specularColor(1.f, 1.f, 1.f, 1.f)
	, m_ambientColor(1.f, 1.f, 1.f, 0.f)
{
}

ProjectiveTextureMappingDemo::~ProjectiveTextureMappingDemo() = default;

//-------------------------------------------------------------------------

void ProjectiveTextureMappingDemo::InitializeInternal()
{
	const auto camera = GetCamera();
	assert(!!camera);

	CreateMaterialWithEffect("ProjectiveTextureMapping");

	// build plane vertices manually
	{
		using Vertex = Material::Vertex;
		using namespace library::math;

		constexpr std::array<Vertex, 6> vertices =
		{
			Vertex(Vector4(-0.5f, -0.5f, 0.0f, 1.0f), Vector2(0.0f, 1.0f), Direction::Backward),
			Vertex(Vector4(-0.5f, 0.5f, 0.0f, 1.0f), Vector2(0.0f, 0.0f), Direction::Backward),
			Vertex(Vector4(0.5f, 0.5f, 0.0f, 1.0f), Vector2(1.0f, 0.0f), Direction::Backward),

			Vertex(Vector4(-0.5f, -0.5f, 0.0f, 1.0f), Vector2(0.0f, 1.0f), Direction::Backward),
			Vertex(Vector4(0.5f, 0.5f, 0.0f, 1.0f), Vector2(1.0f, 0.0f), Direction::Backward),
			Vertex(Vector4(0.5f, -0.5f, 0.0f, 1.0f), Vector2(1.0f, 1.0f), Direction::Backward),
		};

		m_primitivesData.clear();
		auto& pd = m_primitivesData.emplace_back(PrimitiveData{});

		pd.vertexBuffer = VertexBufferData(GetApp().GetDevice(), vertices);
	}


	m_textures.resize(Texture::Count);
	m_textures[Texture::Default] = GetApp().LoadTexture("Checkerboard");
	m_textures[Texture::Projected] = GetApp().LoadTexture("ProjectedTexture");

	m_pointLight = std::make_unique<PointLightComponent>();
	m_pointLight->SetRadius(500.f);
	m_pointLight->SetPosition(math::Vector3(0.f, 0.f, 10.f));

	m_proxyModel = std::make_unique<ProxyModelComponent>("PointLightProxy", 0.5f);
	m_proxyModel->SetCamera(*camera);
	m_proxyModel->SetPosition(m_pointLight->GetPosition());
	m_proxyModel->Initialize(GetApp());

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction(
		[this]() -> std::wstring
		{
			std::wostringstream woss;
			woss <<
				L"Ambient Intensity (+PageUp/-PageDown): " << m_ambientColor.a << "\n" <<
				L"Point Light Intensity (+Home/-End): " << m_pointLight->GetColor().a << "\n" <<
				L"Specular Power (+Insert/-Delete): " << m_specularPower << "\n" <<
				L"Move Projector/Light (Numpad: 8/2, 4/6, 3/9)\n" <<
				L"Rotate Projector (Arrow Keys)\n";// <<
				//L"Reverse Projection? (Space)" << (m_useNoReverseTechnique ? "Off" : "On");
			return woss.str();
		}
	);
	m_text->Initialize(GetApp());

	// specific
	SetScaling(math::Vector3(10.f));

	m_projector = std::make_unique<ProjectorComponent>();
	m_projector->SetPosition(m_pointLight->GetPosition());
	m_projector->Initialize(GetApp());

	m_projectorFrustum.SetProjectionMatrix(m_projector->GetProjectionMatrix());

	m_renderableProjectorFrustum = std::make_unique<RenderableFrustumComponent>();
	m_renderableProjectorFrustum->SetCamera(*camera);
	m_renderableProjectorFrustum->SetPosition(m_pointLight->GetPosition());
	m_renderableProjectorFrustum->SetInitialTransform(math::Matrix4::RotationY(math::Pi));
	m_renderableProjectorFrustum->Initialize(GetApp());
	m_renderableProjectorFrustum->InitializeGeometry(m_projectorFrustum);

	// texture scaling matrix
	{
		ComPtr<ID3D11Resource> projectedTextureResource;
		m_textures[Texture::Projected]->GetResource(&projectedTextureResource);

		ComPtr<ID3D11Texture2D> projectedTexture;
		auto hr = projectedTextureResource.As(projectedTexture);
		assert("ID3D11Resource::QueryInterface (ID3D11Texture2D) failed." && SUCCEEDED(hr));

		D3D11_TEXTURE2D_DESC projectedTextureDesc;
		projectedTexture->GetDesc(&projectedTextureDesc);

		InitializeProjectedTextureScalingMatrix(projectedTextureDesc.Width, projectedTextureDesc.Height);
	}
}

void ProjectiveTextureMappingDemo::Draw_SetData(const PrimitiveData& primitiveData)
{
	const auto& world = GetWorldMatrix();

	auto wvp = world;
	if (auto camera = GetCamera())
	{
		wvp *= camera->GetViewProjectionMatrix();

		m_material->GetCameraPosition() << camera->GetPosition();
	}

	const auto projectiveTextureMatrix =
		world * m_projector->GetViewProjectionMatrix() * m_projectedTextureScalingMatrix;

	m_material->GetWVP() << wvp;
	m_material->GetWorld() << world;

	m_material->GetSpecularPower() << m_specularPower;
	m_material->GetSpecularColor() << m_specularColor.ToVector4();
	m_material->GetAmbientColor() << m_ambientColor.ToVector4();

	const auto isLightVisible = m_pointLight->IsVisibleFrom(GetPosition());
	m_material->GetPointLightsCount() << unsigned(isLightVisible);
	if (isLightVisible)
		m_material->GetPointLights() << PointLightData(*m_pointLight);

	m_material->GetColorTexture() << m_textures[Texture::Default].Get();
	m_material->GetProjectedTexture() << m_textures[Texture::Projected].Get();

	m_material->GetProjectiveTextureMatrix() << projectiveTextureMatrix;

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}

//-------------------------------------------------------------------------

void ProjectiveTextureMappingDemo::Update(const Time& time)
{
	if (!!m_keyboard)
	{
		const auto& keyboard = *m_keyboard;

		UpdateAmbientLight(time, keyboard);
		UpdatePointLightAndProjector(time, keyboard);
		UpdateSpecularLight(time, keyboard);
	}

	m_text->Update(time);
	m_proxyModel->Update(time);
	m_projector->Update(time);
	m_renderableProjectorFrustum->Update(time);

	PrimitiveComponent::Update(time);
}

void ProjectiveTextureMappingDemo::UpdateAmbientLight(const Time& time, const KeyboardComponent& keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_lightModulationRate;
	::utils::UpdateValue(m_ambientColor.a, stepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::PageUp, Key::PageDown));
}

void ProjectiveTextureMappingDemo::UpdatePointLightAndProjector(const Time& time, const KeyboardComponent& keyboard)
{
	const auto elapsedTime = time.elapsed.GetSeconds();

	// update light color intensity
	{
		const float modulationStepValue = elapsedTime * k_lightModulationRate;

		auto lightColor = m_pointLight->GetColor();
		if (::utils::UpdateValue(lightColor.a, modulationStepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::Home, Key::End)))
		{
			m_pointLight->SetColor(lightColor);
		}
	}

	// move
	{
		math::Vector3i movementAmount;

		if (keyboard.IsKeyDown(Key::Num_4))
			movementAmount.x--;
		if (keyboard.IsKeyDown(Key::Num_6))
			movementAmount.x++;

		if (keyboard.IsKeyDown(Key::Num_9))
			movementAmount.y++;
		if (keyboard.IsKeyDown(Key::Num_3))
			movementAmount.y--;

		if (keyboard.IsKeyDown(Key::Num_8))
			movementAmount.z--;
		if (keyboard.IsKeyDown(Key::Num_2))
			movementAmount.z++;

		if (movementAmount)
		{
			const auto movement = movementAmount * k_lightMovementRate * elapsedTime;
			const auto position = m_pointLight->GetPosition() + movement;

			m_pointLight->SetPosition(position);
			m_proxyModel->SetPosition(position);
			m_projector->SetPosition(position);
			m_renderableProjectorFrustum->SetPosition(position);
		}
	}

	// rotate projector
	{
		math::Vector2 rotationAmount;
		if (keyboard.IsKeyDown(Key::Left))
			rotationAmount.x += k_lightRotationRate.x * elapsedTime;

		if (keyboard.IsKeyDown(Key::Right))
			rotationAmount.x -= k_lightRotationRate.x * elapsedTime;

		if (keyboard.IsKeyDown(Key::Up))
			rotationAmount.y += k_lightRotationRate.y * elapsedTime;

		if (keyboard.IsKeyDown(Key::Down))
			rotationAmount.y -= k_lightRotationRate.y * elapsedTime;

		if (rotationAmount)
		{
			const auto rotationQuat = math::Quaternion::RotationPitchYawRoll(
				math::Vector3(rotationAmount.y, rotationAmount.x, 0.f)
			);

			m_proxyModel->Rotate(rotationQuat);
			m_renderableProjectorFrustum->Rotate(rotationQuat);
			m_projector->Rotate(rotationQuat);
		}
	}
}

void ProjectiveTextureMappingDemo::UpdateSpecularLight(const Time& time, const KeyboardComponent& keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_lightModulationRate;
	::utils::UpdateValue(m_specularPower, stepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::Insert, Key::Delete));
}

//-------------------------------------------------------------------------

void ProjectiveTextureMappingDemo::InitializeProjectedTextureScalingMatrix(
	const unsigned textureWidth,
	const unsigned textureHeight
)
{
	const float scalingBiasX = 0.5f + (0.5f / textureWidth);
	const float scalingBiasY = 0.5f + (0.5f / textureHeight);

	m_projectedTextureScalingMatrix._11 = 0.5f;
	m_projectedTextureScalingMatrix._22 = -0.5f;
	m_projectedTextureScalingMatrix._33 = 1.0f;
	m_projectedTextureScalingMatrix._41 = scalingBiasX;
	m_projectedTextureScalingMatrix._42 = scalingBiasY;
	m_projectedTextureScalingMatrix._44 = 1.f;
}
