#pragma once
#include "library/components/SceneComponent.h"
#include "library/materials/SkyboxMaterial.h"

#include <memory>

namespace library
{
	class Effect;
	class Path;

	class SkyboxComponent : public SceneComponent
	{
		RTTI_CLASS(SkyboxComponent, SceneComponent)

	public:
		explicit SkyboxComponent(const Application& app, const Path& cubeMapPath, const float scale);
		~SkyboxComponent() = default;

		void Initialize() override;
		void Update(const Time& time) override;
		using DrawableComponent::Draw;

		const SkyboxMaterial* GetMaterial() const override { return m_material.get(); }

	protected:
		SkyboxMaterial* GetMaterial() override { return m_material.get(); }
		void SetEffectData() override;

	private:
		std::shared_ptr<Effect> m_effect;
		std::unique_ptr<SkyboxMaterial> m_material;
	};
} // namespace library
