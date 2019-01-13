#pragma once
#include <library/components/SceneComponent.h>
#include <library/effect/materials/TextureMappingEffectMaterial.h>
#include <library/components/InputReceivableComponent.h>

#include <memory>

namespace library
{
	class Effect;
} // namespace library

namespace demo
{
	class TextureMappingEffectMaterialComponent
		: public library::SceneComponent
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(TextureMappingEffectMaterialComponent, library::SceneComponent, library::InputReceivableComponent)

	public:
		explicit TextureMappingEffectMaterialComponent(const library::Application& app);

		void Initialize() override;
		void Update(const library::Time& time) override;
		using DrawableComponent::Draw;

		const library::TextureMappingEffectMaterial* GetEffectMaterial() const override { return m_material.get(); }

	protected:
		library::TextureMappingEffectMaterial* GetEffectMaterial() override { return m_material.get(); }

		void SetEffectData() override;

	private:
		std::shared_ptr<library::Effect> m_effect;
		std::unique_ptr<library::TextureMappingEffectMaterial> m_material;
	};
} // namespace demo
