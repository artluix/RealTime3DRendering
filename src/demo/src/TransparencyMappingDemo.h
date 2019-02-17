#pragma once
#include <library/materials/TransparencyMappingMaterial.h>

#include <library/components/SceneComponent.h>
#include <library/components/ConcreteDrawableInputMaterialComponent.hpp>
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

class TransparencyMappingDemo
	: public library::SceneComponent
	, public library::ConcreteDrawableInputMaterialComponent<library::TransparencyMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(TransparencyMappingDemo, library::SceneComponent, library::DrawableInputMaterialComponent, library::InputReceivableComponent)

public:
	explicit TransparencyMappingDemo();
	~TransparencyMappingDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;
	using library::DrawableComponent::Draw;

private:
	void UpdateAmbientLight(const library::Time& time);
	void UpdatePointLight(const library::Time& time);
	void UpdateSpecularLight(const library::Time& time);

	void Draw_SetData() override;
	void Draw_Render() override;

	ComPtr<ID3D11ShaderResourceView> m_transparencyMapTexture;

	library::Color m_ambientColor;
	library::Color m_specularColor;
	float m_specularPower;

	std::unique_ptr<library::PointLightComponent> m_pointLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::TextComponent> m_text;
};
