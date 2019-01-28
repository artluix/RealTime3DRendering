#pragma once
#include <library/materials/DiffuseLightingMaterial.h>

#include <library/components/SceneComponent.h>
#include <library/components/ConcreteMaterialComponent.hpp>
#include <library/components/InputReceivableComponent.h>
#include <library/Color.h>

#include <memory>

namespace library
{
	class DirectionalLightComponent;
	class ProxyModelComponent;
	class TextComponent;
} // namespace library

namespace demo
{
	class DiffuseLightingComponent
		: public library::SceneComponent
		, public library::ConcreteMaterialComponent<library::DiffuseLightingMaterial>
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(DiffuseLightingComponent, library::SceneComponent, library::MaterialComponent, library::InputReceivableComponent)

	public:
		explicit DiffuseLightingComponent();
		~DiffuseLightingComponent();

		void Initialize(const library::Application& app) override;
		void Update(const library::Time& time) override;
		using library::DrawableComponent::Draw;

	private:
		void UpdateAmbientLight(const library::Time& time);
		void UpdateDirectionalLight(const library::Time& time);

		void SetEffectData() override;

		library::Color m_ambientColor;
		
		std::unique_ptr<library::DirectionalLightComponent> m_directionalLight;
		std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
		std::unique_ptr<library::TextComponent> m_text;
	};
} // namespace demo
