#pragma once
#include <library/materials/PointLightMaterial.h>

#include <library/components/ConcreteMaterialSceneComponent.hpp>
#include <library/components/InputReceivableComponent.h>

#include <library/Color.h>

#include <memory>

namespace library
{
	class PointLightComponent;
	class ProxyModelComponent;
	class TextComponent;
} // namespace library

class PointLightDemo
	: public library::ConcreteMaterialSceneComponent<library::PointLightMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(PointLightDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit PointLightDemo();
	~PointLightDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;

private:
	void UpdateAmbientLight(const library::Time& time);
	void UpdatePointLight(const library::Time& time);
	void UpdateSpecularLight(const library::Time& time);

	void Draw_SetData() override;

	float m_specularPower;
	library::Color m_specularColor;
	library::Color m_ambientColor;

	std::unique_ptr<library::PointLightComponent> m_pointLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::TextComponent> m_text;
};
