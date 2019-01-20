#pragma once
#include <library/components/SceneComponent.h>
#include <library/materials/TextureMappingMaterial.h>
#include <library/components/InputReceivableComponent.h>

#include <memory>

namespace library
{
	class Effect;
} // namespace library

namespace demo
{
	class TextureMappingComponent
		: public library::SceneComponent
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(TextureMappingComponent, library::SceneComponent, library::InputReceivableComponent)

	public:
		explicit TextureMappingComponent(const library::Application& app);

		void Initialize() override;
		void Update(const library::Time& time) override;
		using DrawableComponent::Draw;

		const library::TextureMappingMaterial* GetMaterial() const override { return m_material.get(); }

	protected:
		library::TextureMappingMaterial* GetMaterial() override { return m_material.get(); }

		void SetEffectData() override;

	private:
		std::shared_ptr<library::Effect> m_effect;
		std::unique_ptr<library::TextureMappingMaterial> m_material;
	};
} // namespace demo
