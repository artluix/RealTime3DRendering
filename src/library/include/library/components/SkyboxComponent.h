#pragma once
#include "library/materials/SkyboxMaterial.h"

#include "library/components/SceneComponent.h"
#include "library/components/MaterialComponent.h"

namespace library
{
	class Path;

	class SkyboxComponent
		: public SceneComponent
		, public ConcreteMaterialComponent<SkyboxMaterial>
	{
		RTTI_CLASS(SkyboxComponent, SceneComponent, MaterialComponent)

	public:
		explicit SkyboxComponent(const Application& app, const Path& cubeMapPath, const float scale);
		~SkyboxComponent() = default;

		void Initialize() override;
		void Update(const Time& time) override;
		using DrawableComponent::Draw;

	protected:
		void SetEffectData() override;
	};
} // namespace library
