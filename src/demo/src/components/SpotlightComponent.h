#pragma once
#include <library/materials/SpotlightMaterial.h>

#include <library/components/SceneComponent.h>
#include <library/components/MaterialComponentGlue.hpp>
#include <library/components/InputReceivableComponent.h>
#include <library/Color.h>

#include <memory>

namespace library
{
	class SpotlightComponent;
	class ProxyModelComponent;
	class TextComponent;
} // namespace library

namespace demo
{
	class SpotlightComponent
		: public library::SceneComponent
		, public library::MaterialComponentGlue<library::SpotlightMaterial>
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(SpotlightComponent, library::SceneComponent, library::InputReceivableComponent)

	public:
		explicit SpotlightComponent(const library::Application& app);
		~SpotlightComponent();

		void Initialize() override;
		void Update(const library::Time& time) override;
		using library::DrawableComponent::Draw;

	private:
		void UpdateAmbientLight(const library::Time& time);
		void UpdateSpotlight(const library::Time& time);
		void UpdateSpecularLight(const library::Time& time);

		void SetEffectData() override;

		float m_specularPower;
		library::Color m_specularColor;
		library::Color m_ambientColor;

		std::unique_ptr<library::SpotlightComponent> m_spotlight;
		std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
		std::unique_ptr<library::TextComponent> m_text;
	};
} // namespace demo
