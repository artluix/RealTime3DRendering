#pragma once
#include <library/materials/EnvironmentMappingMaterial.h>

#include <library/components/SceneComponent.h>
#include <library/components/MaterialComponentGlue.hpp>
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
		, public library::MaterialComponentGlue<library::EnvironmentMappingMaterial>
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(EnvironmentMappingComponent, library::SceneComponent, library::InputReceivableComponent)

	public:
		explicit EnvironmentMappingComponent(const library::Application& app);
		~EnvironmentMappingComponent();

		void Initialize() override;
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
