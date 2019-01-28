#pragma once
#include <library/materials/EnvironmentMappingMaterial.h>

#include <library/components/SceneComponent.h>
#include <library/components/ConcreteMaterialComponent.hpp>
#include <library/components/InputReceivableComponent.h>

#include <library/DirectXForwardDeclarations.h>
#include <library/Color.h>

#include <memory>

namespace library
{
	class TextComponent;
} // namespace library

namespace demo
{
	class EnvironmentMappingComponent
		: public library::SceneComponent
		, public library::ConcreteMaterialComponent<library::EnvironmentMappingMaterial>
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(EnvironmentMappingComponent, library::SceneComponent, library::InputReceivableComponent)

	public:
		explicit EnvironmentMappingComponent();
		~EnvironmentMappingComponent();

		void Initialize(const library::Application& app) override;
		void Update(const library::Time& time) override;
		using library::DrawableComponent::Draw;

	private:
		void SetEffectData() override;

		library::Color m_ambientColor;
		float m_reflectionAmount;

		ComPtr<ID3D11ShaderResourceView> m_environmentMapShaderResourceView;

		std::unique_ptr<library::TextComponent> m_text;
	};
} // namespace demo
