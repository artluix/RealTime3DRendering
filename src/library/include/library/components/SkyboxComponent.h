#pragma once
#include "library/components/SceneComponent.h"
#include "library/effect/materials/SkyboxEffectMaterial.h"
#include "library/Path.h"
#include "library/DirectXForwardDeclarations.h"

#include <memory>

namespace library
{
	class Effect;

	class SkyboxComponent : public SceneComponent
	{
		RTTI_CLASS(SkyboxComponent, SceneComponent)

	public:
		explicit SkyboxComponent(const Application& app, const Path& cubeMapPath, const float scale);
		~SkyboxComponent() = default;

		void Initialize() override;
		void Update(const Time& time) override;
		using DrawableComponent::Draw;

		const SkyboxEffectMaterial* GetEffectMaterial() const override { return m_material.get(); }

	protected:
		SkyboxEffectMaterial* GetEffectMaterial() override { return m_material.get(); }
		void SetEffectData() override;

	private:
		std::shared_ptr<Effect> m_effect;
		std::unique_ptr<SkyboxEffectMaterial> m_material;

		Path m_cubeMapPath;
		ComPtr<ID3D11ShaderResourceView> m_cubeMapShaderResourceView;
	};
} // namespace library
