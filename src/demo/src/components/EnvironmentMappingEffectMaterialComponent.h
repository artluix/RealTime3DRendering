#pragma once
#include <library/components/SceneComponent.h>
#include <library/effect/materials/EnvironmentMappingEffectMaterial.h>
#include <library/components/InputReceivableComponent.h>
#include <library/DirectXForwardDeclarations.h>
#include <library/Color.h>

#include <memory>

namespace library
{
	class Effect;
	class TextComponent;
} // namespace library

namespace demo
{
	class EnvironmentMappingEffectMaterialComponent
		: public library::SceneComponent
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(EnvironmentMappingEffectMaterialComponent, library::SceneComponent, library::InputReceivableComponent)

	public:
		explicit EnvironmentMappingEffectMaterialComponent(const library::Application& app);
		~EnvironmentMappingEffectMaterialComponent();

		void Initialize() override;
		void Update(const library::Time& time) override;
		using library::DrawableComponent::Draw;

		const library::EnvironmentMappingEffectMaterial* GetEffectMaterial() const override { return m_material.get(); }

	private:
		library::EnvironmentMappingEffectMaterial* GetEffectMaterial() override { return m_material.get(); }

		void SetEffectData() override;

		std::shared_ptr<library::Effect> m_effect;
		std::unique_ptr<library::EnvironmentMappingEffectMaterial> m_material;

		library::Color m_ambientColor;
		float m_reflectionAmount;

		ComPtr<ID3D11ShaderResourceView> m_environmentMapShaderResourceView;

		std::unique_ptr<library::TextComponent> m_text;
	};
} // namespace demo
