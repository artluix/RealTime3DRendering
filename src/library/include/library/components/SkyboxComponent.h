#pragma once
#include "library/Materials/SkyboxMaterial.h"
#include "library/Components/ConcreteMaterialSceneComponent.hpp"

namespace library
{
	class SkyboxComponent : public ConcreteMaterialSceneComponent<SkyboxMaterial>
	{
		RTTI_CLASS(SkyboxComponent, SceneComponent)

	public:
		explicit SkyboxComponent(const std::string& cubeMapName, const float scale);
		~SkyboxComponent() = default;

		void Initialize(const Application& app) override;
		void Update(const Time& time) override;

	protected:
		void Draw_SetData(const MeshData& meshData) override;
	};
} // namespace library
