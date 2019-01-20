#pragma once
#include <library/components/SceneComponent.h>
#include <library/components/InputReceivableComponent.h>
#include <library/materials/TransparencyMappingMaterial.h>
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
	class TransparencyMappingComponent
		: public library::SceneComponent
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(TransparencyMappingComponent, library::SceneComponent, library::InputReceivableComponent)

	public:
		explicit TransparencyMappingComponent(const library::Application& app);
		~TransparencyMappingComponent();

		void Initialize() override;
		void Update(const library::Time& time) override;
		using library::DrawableComponent::Draw;

		const library::TransparencyMappingMaterial* GetMaterial() const override { return m_material.get(); }

	private:
		library::TransparencyMappingMaterial* GetMaterial() override { return m_material.get(); }

		void UpdateAmbientLight(const library::Time& time);
		void UpdatePointLight(const library::Time& time);
		void UpdateSpecularLight(const library::Time& time);

		void SetEffectData() override;
		void Render() override;

		std::shared_ptr<library::Effect> m_effect;
		std::unique_ptr<library::TransparencyMappingMaterial> m_material;

		ComPtr<ID3D11ShaderResourceView> m_transparencyMapShaderResourceView;

		library::Color m_ambientColor;
		library::Color m_specularColor;
		float m_specularPower;

		std::unique_ptr<library::PointLightComponent> m_pointLight;
		std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
		std::unique_ptr<library::TextComponent> m_text;
	};
} // namespace demo
