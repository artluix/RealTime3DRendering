#pragma once
#include "library/materials/SkyboxMaterial.h"

#include "library/components/SceneComponent.h"
#include "library/components/ConcreteMaterialComponent.hpp"

#include <string>

namespace library
{
	class SkyboxComponent
		: public SceneComponent
		, public ConcreteMaterialComponent<SkyboxMaterial>
	{
		RTTI_CLASS(SkyboxComponent, SceneComponent, MaterialComponent)

	public:
		explicit SkyboxComponent(const std::string& cubeMapName, const float scale);
		~SkyboxComponent() = default;

		void Initialize(const Application& app) override;
		void Update(const Time& time) override;
		using DrawableComponent::Draw;

	protected:
		void SetEffectData() override;
	};
} // namespace library
