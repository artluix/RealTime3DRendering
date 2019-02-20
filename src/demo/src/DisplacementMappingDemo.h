#pragma once
#include <library/materials/DisplacementMappingMaterial.h>

#include <library/components/ConcreteMaterialSceneComponent.hpp>
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

class DisplacementMappingDemo
	: public library::ConcreteMaterialSceneComponent<library::DisplacementMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(DisplacementMappingDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit DisplacementMappingDemo();
	~DisplacementMappingDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;

private:
	void UpdateAmbientLight(const library::Time& time);
	void UpdatePointLight(const library::Time& time);

	void UpdateDisplacement(const library::Time& time);

	void Draw_SetData() override;

	ComPtr<ID3D11ShaderResourceView> m_displacementMapTexture;

	library::Color m_ambientColor;
	library::Color m_specularColor;
	float m_specularPower;

	float m_displacementScale;

	std::unique_ptr<library::PointLightComponent> m_pointLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::TextComponent> m_text;
};
