#pragma once
#include <library/components/SceneComponent.h>
#include <library/materials/EnvironmentMappingMaterial.h>
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
	class EnvironmentMappingComponent
		: public library::SceneComponent
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(EnvironmentMappingComponent, library::SceneComponent, library::InputReceivableComponent)

	public:
		explicit EnvironmentMappingComponent(const library::Application& app);
		~EnvironmentMappingComponent();

		void Initialize() override;
		void Update(const library::Time& time) override;
		using library::DrawableComponent::Draw;

		const library::EnvironmentMappingMaterial* GetMaterial() const override { return m_material.get(); }

	private:
		library::EnvironmentMappingMaterial* GetMaterial() override { return m_material.get(); }

		void SetEffectData() override;

		std::shared_ptr<library::Effect> m_effect;
		std::unique_ptr<library::EnvironmentMappingMaterial> m_material;

		library::Color m_ambientColor;
		float m_reflectionAmount;

		ComPtr<ID3D11ShaderResourceView> m_environmentMapShaderResourceView;

		std::unique_ptr<library::TextComponent> m_text;
	};
} // namespace demo
