#pragma once
#include <library/materials/NormalMappingMaterial.h>

#include <library/components/SceneComponent.h>
#include <library/components/ConcreteDrawableInputMaterialComponent.hpp>
#include <library/components/InputReceivableComponent.h>

#include <library/Color.h>
#include <library/DirectXForwardDeclarations.h>

#include <memory>

namespace library
{
	class DirectionalLightComponent;
	class ProxyModelComponent;
	class TextComponent;
} // namespace library

class NormalMappingDemo
	: public library::SceneComponent
	, public library::ConcreteDrawableInputMaterialComponent<library::NormalMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(NormalMappingDemo, library::SceneComponent, library::DrawableInputMaterialComponent, library::InputReceivableComponent)

public:
	explicit NormalMappingDemo();
	~NormalMappingDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;
	using library::DrawableComponent::Draw;

private:
	void UpdateAmbientLight(const library::Time& time);
	void UpdateDirectionalLight(const library::Time& time);
	void UpdateSpecularLight(const library::Time& time);

	void Draw_SetData() override;

	float m_specularPower;
	library::Color m_specularColor;
	library::Color m_ambientColor;

	ComPtr<ID3D11ShaderResourceView> m_normalMapTexture;

	std::unique_ptr<library::DirectionalLightComponent> m_directionalLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::TextComponent> m_text;
};