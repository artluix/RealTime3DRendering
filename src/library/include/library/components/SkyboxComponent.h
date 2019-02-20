#pragma once
#include "library/materials/SkyboxMaterial.h"
#include "library/components/ConcreteMaterialSceneComponent.hpp"

namespace library
{
	class SkyboxComponent : public ConcreteMaterialSceneComponent<SkyboxMaterial>
	{
		RTTI_CLASS(SkyboxComponent, MaterialSceneComponent)

	public:
		explicit SkyboxComponent(const std::string& cubeMapName, const float scale);
		~SkyboxComponent() = default;

		void Initialize(const Application& app) override;
		void Update(const Time& time) override;

	protected:
		void Draw_SetData() override;
	};
} // namespace library
