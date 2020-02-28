#include "Components/AnimationDemo.h"

#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>

#include <library/Components/TextComponent.h>
#include <library/Components/ProxyModelComponent.h>
#include <library/Components/PointLightComponent.h>
#include <library/Components/AnimationPlayerComponent.h>

#include <library/Model/AnimationClip.h>

#include <library/Application.h>

#include <library/Effect/Effect.h>
#include <library/Effect/EffectTechnique.h>
#include <library/Effect/EffectPass.h>
#include <library/Effect/EffectVariable.h>

#include <library/Model/Model.h>
#include <library/Model/Mesh.h>
#include <library/Model/ModelMaterial.h>

#include <library/RasterizerStates.h>

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

AnimationDemo::AnimationDemo()
	: m_specularPower(25.f)
	, m_specularColor(1.f, 1.f, 1.f, 1.f)
	, m_ambientColor(1.f, 1.f, 1.f, 0.f)

	, m_manualAdvanceMode(false)
{
}

AnimationDemo::~AnimationDemo() = default;

//-------------------------------------------------------------------------

void AnimationDemo::InitializeInternal()
{
	const auto camera = GetCamera();
	assert(!!camera);

	CreateMaterialWithEffect("SkinnedModel");

	m_model = std::make_unique<Model>(GetApp(), "RunningSoldier.dae", true);
	m_primitivesData = m_material->CreatePrimitivesData(GetApp().GetDevice(), *m_model);

	m_textures.resize(Texture::Count);
	// extract diffuse texture name from ModelMaterial
	{
		const auto& mesh = m_model->GetMesh(0);
		const auto& modelMaterial = mesh.GetMaterial();
		if (modelMaterial.HasTextureNames(TextureType::Diffuse))
		{
			const auto& diffuseTexturePath = Path(
				modelMaterial.GetTextureNames(TextureType::Diffuse).front()
			);
			const auto textureName = diffuseTexturePath.GetBaseName().SplitExt().first.GetString();
			m_textures[Texture::Default] = GetApp().LoadTexture(textureName);
		}
	}

	SetScaling(math::Vector3(0.05f));

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
			woss << std::setprecision(5) <<
				L"Ambient Intensity (+PageUp/-PageDown): " << m_ambientColor.a << '\n' <<
				L"Point Light Intensity (+Home/-End): " << m_pointLight->GetColor().a << '\n' <<
				L"Specular Power (+Insert/-Delete): " << m_specularPower << '\n' <<
				L"Move Point Light (Numpad: 8/2, 4/6, 3/9)\n" <<
				L"Frame Advance Mode (Enter): " << (m_manualAdvanceMode ? "Manual" : "Auto") << '\n' <<
				L"Animation Time: " << m_animationPlayer->GetTime().count() << '\n' <<
				L"Frame Interpolation (I): " << (m_animationPlayer->IsInterpolationEnabled() ? "On" : "Off") << '\n' <<
				L"Go To Bind Pose (B)";

			if (m_manualAdvanceMode)
				woss << "\nCurrentKeyFrame (Space): " << m_animationPlayer->GetCurrentKeyframe();
			else
				woss << "\nPause \\ Resume (Pause)";

			return woss.str();
		}
	);
	m_text->Initialize(GetApp());

	m_animationPlayer = std::make_unique<AnimationPlayerComponent>(*m_model, false);
	m_animationPlayer->Initialize(GetApp());
	m_animationPlayer->Play(m_model->GetAnimation(0));
}

void AnimationDemo::Update(const Time& time)
{
	UpdateOptions();

	UpdateAmbientLight(time);
	UpdatePointLight(time);
	UpdateSpecularLight(time);

	m_text->Update(time);
	m_proxyModel->Update(time);

	if (!m_manualAdvanceMode)
		m_animationPlayer->Update(time);

	PrimitiveComponent::Update(time);
}

//-------------------------------------------------------------------------

void AnimationDemo::Draw_SetData(const PrimitiveData& primitiveData)
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

	m_material->GetSpecularPower() << m_specularPower;
	m_material->GetSpecularColor() << m_specularColor.ToVector4();
	m_material->GetAmbientColor() << m_ambientColor.ToVector4();

	m_material->GetLightsData() << m_lightsData;

	m_material->GetColorTexture() << m_textures[Texture::Default].Get();

	m_material->GetBoneTransforms() << m_animationPlayer->GetBoneTransforms();

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}

//-------------------------------------------------------------------------

void AnimationDemo::UpdateOptions()
{
	if (!!m_keyboard)
	{
		if (m_keyboard->WasKeyPressed(Key::Pause))
		{
			if (m_animationPlayer->IsPlaying())
				m_animationPlayer->Pause();
			else
				m_animationPlayer->Resume();
		}

		if (m_keyboard->WasKeyPressed(Key::B))
		{
			// Reset animation to the bind pose
			m_animationPlayer->Play(m_model->GetAnimation(0));
		}

		if (m_keyboard->WasKeyPressed(Key::I))
		{
			// Switch interpolation (on/off)
			m_animationPlayer->SetInterpolationEnabled(!m_animationPlayer->IsInterpolationEnabled());
		}

		if (m_keyboard->WasKeyPressed(Key::Enter))
		{
			// Turn on/off manual advance mode
			m_manualAdvanceMode = !m_manualAdvanceMode;
			m_animationPlayer->SetCurrentKeyframe(0);
		}

		if (m_manualAdvanceMode && m_keyboard->WasKeyPressed(Key::Space))
		{
			// Advance current keyframe
			const auto keyframesCount = m_animationPlayer->GetAnimation()->GetKeyframesCount();
			const auto keyframe = (m_animationPlayer->GetCurrentKeyframe() + 1) % keyframesCount;
			m_animationPlayer->SetCurrentKeyframe(keyframe);
		}
	}
}

//-------------------------------------------------------------------------

void AnimationDemo::UpdateAmbientLight(const Time& time)
{
	static float ambientLightIntensity = m_ambientColor.a;

	if (!!m_keyboard)
	{
		if (m_keyboard->IsKeyDown(Key::PageUp) && ambientLightIntensity < k_byteMax)
		{
			ambientLightIntensity += k_lightModulationRate * time.elapsed.GetSeconds();
			m_ambientColor.a = math::Min(ambientLightIntensity, k_byteMax);
		}

		if (m_keyboard->IsKeyDown(Key::PageDown) && ambientLightIntensity > 0)
		{
			ambientLightIntensity -= k_lightModulationRate * time.elapsed.GetSeconds();
			m_ambientColor.a = math::Max(ambientLightIntensity, 0.f);
		}
	}
}

void AnimationDemo::UpdatePointLight(const Time& time)
{
	static float pointLightIntensity = m_pointLight->GetColor().a;

	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		// update directional light intensity
		if (m_keyboard->IsKeyDown(Key::Home) && pointLightIntensity < k_byteMax)
		{
			pointLightIntensity += k_lightModulationRate * elapsedTime;

			auto pointLightColor = m_pointLight->GetColor();
			pointLightColor.a = math::Min(pointLightIntensity, k_byteMax);
			m_pointLight->SetColor(pointLightColor);
		}

		if (m_keyboard->IsKeyDown(Key::End) && pointLightIntensity > 0)
		{
			pointLightIntensity -= k_lightModulationRate * elapsedTime;

			auto pointLightColor = m_pointLight->GetColor();
			pointLightColor.a = math::Max(pointLightIntensity, 0.f);
			m_pointLight->SetColor(pointLightColor);
		}

		// move
		{
			math::Vector3i movementAmount;

			if (m_keyboard->IsKeyDown(Key::Num_4))
				movementAmount.x--;
			if (m_keyboard->IsKeyDown(Key::Num_6))
				movementAmount.x++;

			if (m_keyboard->IsKeyDown(Key::Num_9))
				movementAmount.y++;
			if (m_keyboard->IsKeyDown(Key::Num_3))
				movementAmount.y--;

			if (m_keyboard->IsKeyDown(Key::Num_8))
				movementAmount.z--;
			if (m_keyboard->IsKeyDown(Key::Num_2))
				movementAmount.z++;

			if (movementAmount)
			{
				const auto movement = movementAmount * k_lightMovementRate * elapsedTime;
				const auto position = m_pointLight->GetPosition() + movement;

				m_pointLight->SetPosition(position);
				m_proxyModel->SetPosition(position);
			}
		}
	}
}

void AnimationDemo::UpdateSpecularLight(const Time& time)
{
	static float specularLightIntensity = m_specularPower;

	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		if (m_keyboard->IsKeyDown(Key::Insert) && specularLightIntensity < k_byteMax)
		{
			specularLightIntensity += k_lightModulationRate * elapsedTime;
			specularLightIntensity = math::Min(specularLightIntensity, k_byteMax);

			m_specularPower = specularLightIntensity;;
		}

		if (m_keyboard->IsKeyDown(Key::Delete) && specularLightIntensity > 0)
		{
			specularLightIntensity -= k_lightModulationRate * elapsedTime;
			specularLightIntensity = math::Max(specularLightIntensity, 0.f);

			m_specularPower = specularLightIntensity;
		}
	}
}
