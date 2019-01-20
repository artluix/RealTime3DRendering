#pragma once
#include <library/components/SceneComponent.h>
#include <library/materials/DisplacementMappingMaterial.h>
#include <library/components/InputReceivableComponent.h>
#include <library/DirectXForwardDeclarations.h>
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
	class DisplacementMappingComponent
		: public library::SceneComponent
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(DisplacementMappingComponent, library::SceneComponent, library::InputReceivableComponent)

	public:
		explicit DisplacementMappingComponent(const library::Application& app);
		~DisplacementMappingComponent();

		void Initialize() override;
		void Update(const library::Time& time) override;
		using library::DrawableComponent::Draw;

		const library::DisplacementMappingMaterial* GetMaterial() const override { return m_material.get(); }

	private:
		library::DisplacementMappingMaterial* GetMaterial() override { return m_material.get(); }

		void UpdateAmbientLight(const library::Time& time);
		void UpdatePointLight(const library::Time& time);

		void UpdateDisplacement(const library::Time& time);

		void SetEffectData() override;

		std::shared_ptr<library::Effect> m_effect;
		std::unique_ptr<library::DisplacementMappingMaterial> m_material;

		ComPtr<ID3D11ShaderResourceView> m_displacementMapShaderResourceView;

		library::Color m_ambientColor;
		library::Color m_specularColor;
		float m_specularPower;

		float m_displacementScale;

		std::unique_ptr<library::PointLightComponent> m_pointLight;
		std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
		std::unique_ptr<library::TextComponent> m_text;
	};
} // namespace demo
