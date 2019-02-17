#pragma once
#include "library/materials/SkyboxMaterial.h"

#include "library/components/SceneComponent.h"
#include "library/components/ConcreteDrawableInputMaterialComponent.hpp"

namespace library
{
	class SkyboxComponent
		: public SceneComponent
		, public ConcreteDrawableInputMaterialComponent<SkyboxMaterial>
	{
		RTTI_CLASS(SkyboxComponent, SceneComponent, DrawableInputMaterialComponent)

	public:
		explicit SkyboxComponent(const std::string& cubeMapName, const float scale);
		~SkyboxComponent() = default;

		void Initialize(const Application& app) override;
		void Update(const Time& time) override;

	protected:
		void Draw_SetData() override;
	};
} // namespace library
