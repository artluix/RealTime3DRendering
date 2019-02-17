#pragma once
#include <library/materials/EnvironmentMappingMaterial.h>

#include <library/components/SceneComponent.h>
#include <library/components/ConcreteDrawableInputMaterialComponent.hpp>
#include <library/components/InputReceivableComponent.h>

#include <library/DirectXForwardDeclarations.h>
#include <library/Color.h>

#include <memory>

namespace library
{
	class TextComponent;
} // namespace library

class EnvironmentMappingDemo
	: public library::SceneComponent
	, public library::ConcreteDrawableInputMaterialComponent<library::EnvironmentMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(EnvironmentMappingDemo, library::SceneComponent, library::DrawableInputMaterialComponent, library::InputReceivableComponent)

public:
	explicit EnvironmentMappingDemo();
	~EnvironmentMappingDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;
	using library::DrawableComponent::Draw;

private:
	void Draw_SetData() override;

	library::Color m_ambientColor;
	float m_reflectionAmount;

	ComPtr<ID3D11ShaderResourceView> m_environmentMapTexture;

	std::unique_ptr<library::TextComponent> m_text;
};
