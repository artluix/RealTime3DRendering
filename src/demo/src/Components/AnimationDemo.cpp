#include "Components/AnimationDemo.h"

#include "DemoUtils.h"

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

#include <library/Render/RasterizerStates.h>

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
	CreatePrimitivesData(*m_model);

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
			const auto textureName = diffuseTexturePath.GetBaseName().GetString();
			//m_textures[Texture::Default] = GetApp().CreateTexture2DSRV(textureName);
			m_textures[Texture::Default] = GetApp().CreateTexture2DSRV("Soldier.png");
		}
	}

	SetScaling(math::Vector3(0.05f));

	m_pointLight = std::make_unique<PointLightComponent>();
	m_pointLight->SetupProxyModel(*GetCamera());
	m_pointLight->SetRadius(500.f);
	m_pointLight->SetPosition(math::Vector3(0.f, 0.f, 10.f));

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
	if (!!m_keyboard)
	{
		const auto& keyboard = *m_keyboard;

		UpdateOptions(keyboard);
		UpdateAmbientLight(time, keyboard);
		UpdatePointLight(time, keyboard);
		UpdateSpecularLight(time, keyboard);
	}

	m_text->Update(time);
	m_pointLight->Update(time);

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

	const auto isLightVisible = m_pointLight->IsVisibleFrom(GetPosition());
	m_material->GetPointLightsCount() << unsigned(isLightVisible);
	if (isLightVisible)
		m_material->GetPointLights() << PointLightData(*m_pointLight);

	m_material->GetColorTexture() << m_textures[Texture::Default].Get();

	m_material->GetBoneTransforms() << m_animationPlayer->GetBoneTransforms();

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}

//-------------------------------------------------------------------------

void AnimationDemo::UpdateOptions(const KeyboardComponent& keyboard)
{
	if (keyboard.WasKeyPressed(Key::Pause))
	{
		if (m_animationPlayer->IsPlaying())
			m_animationPlayer->Pause();
		else
			m_animationPlayer->Resume();
	}

	if (keyboard.WasKeyPressed(Key::B))
	{
		// Reset animation to the bind pose
		m_animationPlayer->Play(m_model->GetAnimation(0));
	}

	if (keyboard.WasKeyPressed(Key::I))
	{
		// Switch interpolation (on/off)
		m_animationPlayer->SetInterpolationEnabled(!m_animationPlayer->IsInterpolationEnabled());
	}

	if (keyboard.WasKeyPressed(Key::Enter))
	{
		// Turn on/off manual advance mode
		m_manualAdvanceMode = !m_manualAdvanceMode;
		m_animationPlayer->SetCurrentKeyframe(0);
	}

	if (m_manualAdvanceMode && keyboard.WasKeyPressed(Key::Space))
	{
		// Advance current keyframe
		const auto keyframesCount = m_animationPlayer->GetAnimation()->GetKeyframesCount();
		const auto keyframe = (m_animationPlayer->GetCurrentKeyframe() + 1) % keyframesCount;
		m_animationPlayer->SetCurrentKeyframe(keyframe);
	}
}

//-------------------------------------------------------------------------

void AnimationDemo::UpdateAmbientLight(const Time& time, const KeyboardComponent& keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_lightModulationRate;
	::utils::UpdateValue(m_ambientColor.a, stepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::PageUp, Key::PageDown));
}

void AnimationDemo::UpdatePointLight(const Time& time, const KeyboardComponent& keyboard)
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
		}
	}
}

void AnimationDemo::UpdateSpecularLight(const Time& time, const KeyboardComponent& keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_lightModulationRate;
	::utils::UpdateValue(m_specularPower, stepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::Insert, Key::Delete));
}
