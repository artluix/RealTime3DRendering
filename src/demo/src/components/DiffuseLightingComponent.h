#pragma once
#include <library/components/SceneComponent.h>
#include <library/materials/DiffuseLightingMaterial.h>
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
	class DiffuseLightingComponent
		: public library::SceneComponent
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(DiffuseLightingComponent, library::SceneComponent, library::InputReceivableComponent)

	public:
		explicit DiffuseLightingComponent(const library::Application& app);
		~DiffuseLightingComponent();

		void Initialize() override;
		void Update(const library::Time& time) override;
		using library::DrawableComponent::Draw;

		const library::DiffuseLightingMaterial* GetMaterial() const override { return m_material.get(); }

	private:
		library::DiffuseLightingMaterial* GetMaterial() override { return m_material.get(); }

		void UpdateAmbientLight(const library::Time& time);
		void UpdateDirectionalLight(const library::Time& time);

		void SetEffectData() override;

		std::shared_ptr<library::Effect> m_effect;
		std::unique_ptr<library::DiffuseLightingMaterial> m_material;

		library::Color m_ambientColor;
		
		std::unique_ptr<library::DirectionalLightComponent> m_directionalLight;
		std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
		std::unique_ptr<library::TextComponent> m_text;
	};
} // namespace demo
