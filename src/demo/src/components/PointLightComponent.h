#pragma once
#include <library/components/SceneComponent.h>
#include <library/materials/PointLightMaterial.h>
#include <library/components/InputReceivableComponent.h>
#include <library/Color.h>

#include <memory>

namespace library
{
	class Effect;
	class PointLightComponent;
	class ProxyModelComponent;
	class TextComponent;
} // namespace library

namespace demo
{
	class PointLightComponent
		: public library::SceneComponent
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(PointLightComponent, library::SceneComponent, library::InputReceivableComponent)

	public:
		explicit PointLightComponent(const library::Application& app);
		~PointLightComponent();

		void Initialize() override;
		void Update(const library::Time& time) override;
		using library::DrawableComponent::Draw;

		const library::PointLightMaterial* GetMaterial() const override { return m_material.get(); }

	private:
		library::PointLightMaterial* GetMaterial() override { return m_material.get(); }

		void UpdateAmbientLight(const library::Time& time);
		void UpdatePointLight(const library::Time& time);
		void UpdateSpecularLight(const library::Time& time);

		void SetEffectData() override;

		std::shared_ptr<library::Effect> m_effect;
		std::unique_ptr<library::PointLightMaterial> m_material;

		float m_specularPower;
		library::Color m_specularColor;
		library::Color m_ambientColor;

		std::unique_ptr<library::PointLightComponent> m_pointLight;
		std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
		std::unique_ptr<library::TextComponent> m_text;
	};
} // namespace demo
