#pragma once
#include <library/materials/TransparencyMappingMaterial.h>

#include <library/components/SceneComponent.h>
#include <library/components/ConcreteMaterialComponent.hpp>
#include <library/components/InputReceivableComponent.h>

#include <library/DirectXForwardDeclarations.h>
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
	class TransparencyMappingComponent
		: public library::SceneComponent
		, public library::ConcreteMaterialComponent<library::TransparencyMappingMaterial>
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(TransparencyMappingComponent, library::SceneComponent, library::MaterialComponent, library::InputReceivableComponent)

	public:
		explicit TransparencyMappingComponent();
		~TransparencyMappingComponent();

		void Initialize(const library::Application& app) override;
		void Update(const library::Time& time) override;
		using library::DrawableComponent::Draw;

	private:
		void UpdateAmbientLight(const library::Time& time);
		void UpdatePointLight(const library::Time& time);
		void UpdateSpecularLight(const library::Time& time);

		void SetEffectData() override;
		void Render() override;

		ComPtr<ID3D11ShaderResourceView> m_transparencyMapShaderResourceView;

		library::Color m_ambientColor;
		library::Color m_specularColor;
		float m_specularPower;

		std::unique_ptr<library::PointLightComponent> m_pointLight;
		std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
		std::unique_ptr<library::TextComponent> m_text;
	};
} // namespace demo
