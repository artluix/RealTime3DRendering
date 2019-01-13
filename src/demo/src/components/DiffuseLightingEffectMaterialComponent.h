#pragma once
#include <library/components/SceneComponent.h>
#include <library/effect/materials/DiffuseLightingEffectMaterial.h>
#include <library/components/InputReceivableComponent.h>
#include <library/Color.h>

#include <memory>

namespace library
{
	class Effect;
	class DirectionalLightComponent;
	class ProxyModelComponent;
	class TextComponent;
} // namespace library

namespace demo
{
	class DiffuseLightingEffectMaterialComponent
		: public library::SceneComponent
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(DiffuseLightingEffectMaterialComponent, library::SceneComponent, library::InputReceivableComponent)

	public:
		explicit DiffuseLightingEffectMaterialComponent(const library::Application& app);
		~DiffuseLightingEffectMaterialComponent();

		void Initialize() override;
		void Update(const library::Time& time) override;
		using library::DrawableComponent::Draw;

		const library::DiffuseLightingEffectMaterial* GetEffectMaterial() const override { return m_material.get(); }

	private:
		library::DiffuseLightingEffectMaterial* GetEffectMaterial() override { return m_material.get(); }

		void UpdateAmbientLight(const library::Time& time);
		void UpdateDirectionalLight(const library::Time& time);

		void SetEffectData() override;

		std::shared_ptr<library::Effect> m_effect;
		std::unique_ptr<library::DiffuseLightingEffectMaterial> m_material;

		library::Color m_ambientColor;
		
		std::unique_ptr<library::DirectionalLightComponent> m_directionalLight;
		std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
		std::unique_ptr<library::TextComponent> m_text;
	};
} // namespace demo
