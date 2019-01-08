#include "components/DiffuseLightingMaterialComponent.h"

#include <library/components/CameraComponent.h>
#include <library/components/KeyboardComponent.h>

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
		const auto k_modelPath = utils::GetExecutableDirectory().Join(fs::Path("../data/models/Sphere.obj"));
	}

	DiffuseLightingMaterialComponent::DiffuseLightingMaterialComponent(const Application& app)
		: SceneComponent()
		, InputReceivableComponent()
		, ConcreteMaterialComponent(app, k_modelPath)
	{
	}

	void DiffuseLightingMaterialComponent::Initialize()
	{

	}

	void DiffuseLightingMaterialComponent::Update(const Time& time)
	{

	}

	void DiffuseLightingMaterialComponent::Draw(const Time& time)
	{

	}

	void DiffuseLightingMaterialComponent::UpdateAmbientLight(const Time& time)
	{

	}

	void DiffuseLightingMaterialComponent::UpdateDirectionalLight(const Time& time)
	{

	}

} // namespace demo
