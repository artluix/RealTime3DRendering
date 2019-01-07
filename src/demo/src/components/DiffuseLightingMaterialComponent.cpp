#include "components/DiffuseLightingMaterialComponent.h"

#include <library/components/CameraComponent.h>
#include <library/components/KeyboardComponent.h>

#include <library/Application.h>
#include <library/Utils.h>
#include <library/Path.h>
#include <library/Exception.h>

#include <library/materials/TextureMappingMaterial.h>

#include <library/effect/Effect.h>
#include <library/effect/EffectPass.h>
#include <library/effect/EffectTechnique.h>
#include <library/effect/EffectVariable.h>

#include <library/Model.h>
#include <library/Mesh.h>

namespace demo
{
	using namespace library;

	DiffuseLightingMaterialComponent::DiffuseLightingMaterialComponent(
		const Application& app,
		const CameraComponent& camera,
		const KeyboardComponent& keyboard
	)
		: SceneComponent(app, camera)
		, m_keyboard(keyboard)
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
