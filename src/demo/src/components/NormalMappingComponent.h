#pragma once
#include <library/components/SceneComponent.h>
#include <library/materials/NormalMappingMaterial.h>
#include <library/components/InputReceivableComponent.h>
#include <library/Color.h>
#include <library/DirectXForwardDeclarations.h>

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
	class NormalMappingComponent
		: public library::SceneComponent
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(NormalMappingComponent, library::SceneComponent, library::InputReceivableComponent)

	public:
		explicit NormalMappingComponent(const library::Application& app);
		~NormalMappingComponent();

		void Initialize() override;
		void Update(const library::Time& time) override;
		using library::DrawableComponent::Draw;

		const library::NormalMappingMaterial* GetMaterial() const override { return m_material.get(); }

	private:
		library::NormalMappingMaterial* GetMaterial() override { return m_material.get(); }

		void UpdateAmbientLight(const library::Time& time);
		void UpdateDirectionalLight(const library::Time& time);
		void UpdateSpecularLight(const library::Time& time);

		void SetEffectData() override;

		std::shared_ptr<library::Effect> m_effect;
		std::unique_ptr<library::NormalMappingMaterial> m_material;

		float m_specularPower;
		library::Color m_specularColor;
		library::Color m_ambientColor;

		ComPtr<ID3D11ShaderResourceView> m_normalMapShaderResourceView;

		std::unique_ptr<library::DirectionalLightComponent> m_directionalLight;
		std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
		std::unique_ptr<library::TextComponent> m_text;
	};
} // namespace demo
