#pragma once
#include <library/components/SceneComponent.h>
#include <library/components/InputReceivableComponent.h>
#include <library/effect/materials/FogEffectMaterial.h>
#include <library/DirectXForwardDeclarations.h>
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
	class FogEffectMaterialComponent
		: public library::SceneComponent
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(FogEffectMaterialComponent, library::SceneComponent, library::InputReceivableComponent)

	public:
		explicit FogEffectMaterialComponent(const library::Application& app);
		~FogEffectMaterialComponent();

		void Initialize() override;
		void Update(const library::Time& time) override;
		using library::DrawableComponent::Draw;

		const library::FogEffectMaterial* GetEffectMaterial() const override { return m_material.get(); }

	private:
		library::FogEffectMaterial* GetEffectMaterial() override { return m_material.get(); }

		void UpdateAmbientLight(const library::Time& time);
		void UpdateDirectionalLight(const library::Time& time);
		void SetActiveTechnique();

		void SetEffectData() override;

		std::shared_ptr<library::Effect> m_effect;
		std::unique_ptr<library::FogEffectMaterial> m_material;

		library::Color m_ambientColor;

		float m_fogStart;
		float m_fogRange;
		float m_fogEnabled;

		std::unique_ptr<library::DirectionalLightComponent> m_directionalLight;
		std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
		std::unique_ptr<library::TextComponent> m_text;
	};
} // namespace demo
