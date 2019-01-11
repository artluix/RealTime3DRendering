#include "components/DiffuseLightingEffectMaterialComponent.h"

#include <library/components/CameraComponent.h>
#include <library/components/KeyboardComponent.h>
#include <library/components/DirectionalLightComponent.h>
#include <library/components/ProxyModelComponent.h>

#include <library/Application.h>
#include <library/Utils.h>
#include <library/Path.h>
#include <library/Exception.h>

#include <library/effect/Effect.h>
#include <library/effect/EffectPass.h>
#include <library/effect/EffectTechnique.h>
#include <library/effect/EffectVariable.h>

namespace demo
{
	using namespace library;

	namespace
	{
		const auto k_modelPath = utils::GetExecutableDirectory().Join(Path("../data/models/Sphere.obj"));
	}

	DiffuseLightingEffectMaterialComponent::DiffuseLightingEffectMaterialComponent(const Application& app)
		: SceneComponent(app)
		, InputReceivableComponent()
	{
	}

	DiffuseLightingEffectMaterialComponent::~DiffuseLightingEffectMaterialComponent() = default;

	void DiffuseLightingEffectMaterialComponent::Initialize()
	{

	}

	void DiffuseLightingEffectMaterialComponent::Update(const Time& time)
	{

	}

	void DiffuseLightingEffectMaterialComponent::Draw(const Time& time)
	{

	}

	void DiffuseLightingEffectMaterialComponent::UpdateAmbientLight(const Time& time)
	{

	}

	void DiffuseLightingEffectMaterialComponent::UpdateDirectionalLight(const Time& time)
	{

	}
} // namespace demo
