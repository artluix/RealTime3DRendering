#pragma once
#include <library/components/SceneComponent.h>
#include <library/materials/SpotlightMaterial.h>
#include <library/components/InputReceivableComponent.h>
#include <library/Color.h>

#include <memory>

namespace library
{
	class Effect;
	class SpotlightComponent;
	class ProxyModelComponent;
	class TextComponent;
} // namespace library

namespace demo
{
	class SpotlightComponent
		: public library::SceneComponent
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(SpotlightComponent, library::SceneComponent, library::InputReceivableComponent)

	public:
		explicit SpotlightComponent(const library::Application& app);
		~SpotlightComponent();

		void Initialize() override;
		void Update(const library::Time& time) override;
		using library::DrawableComponent::Draw;

		const library::SpotlightMaterial* GetMaterial() const override { return m_material.get(); }

	private:
		library::SpotlightMaterial* GetMaterial() override { return m_material.get(); }

		void UpdateAmbientLight(const library::Time& time);
		void UpdateSpotlight(const library::Time& time);
		void UpdateSpecularLight(const library::Time& time);

		void SetEffectData() override;

		std::shared_ptr<library::Effect> m_effect;
		std::unique_ptr<library::SpotlightMaterial> m_material;

		float m_specularPower;
		library::Color m_specularColor;
		library::Color m_ambientColor;

		std::unique_ptr<library::SpotlightComponent> m_spotlight;
		std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
		std::unique_ptr<library::TextComponent> m_text;
	};
} // namespace demo
