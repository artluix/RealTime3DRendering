#pragma once
#include <library/materials/PointLightMaterial.h>

#include <library/components/SceneComponent.h>
#include <library/components/MaterialComponentGlue.hpp>
#include <library/components/InputReceivableComponent.h>

#include <library/Color.h>

#include <memory>

namespace library
{
	class PointLightComponent;
	class ProxyModelComponent;
	class TextComponent;
} // namespace library

namespace demo
{
	class PointLightComponent
		: public library::SceneComponent
		, public library::MaterialComponentGlue<library::PointLightMaterial>
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(PointLightComponent, library::SceneComponent, library::MaterialComponent, library::InputReceivableComponent)

	public:
		explicit PointLightComponent(const library::Application& app);
		~PointLightComponent();

		void Initialize() override;
		void Update(const library::Time& time) override;
		using library::DrawableComponent::Draw;

	private:
		void UpdateAmbientLight(const library::Time& time);
		void UpdatePointLight(const library::Time& time);
		void UpdateSpecularLight(const library::Time& time);

		void SetEffectData() override;

		float m_specularPower;
		library::Color m_specularColor;
		library::Color m_ambientColor;

		std::unique_ptr<library::PointLightComponent> m_pointLight;
		std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
		std::unique_ptr<library::TextComponent> m_text;
	};
} // namespace demo
