#pragma once
#include <library/materials/SpotlightMaterial.h>

#include <library/components/ConcreteMaterialSceneComponent.hpp>
#include <library/components/InputReceivableComponent.h>
#include <library/Color.h>

#include <memory>

namespace library
{
	class SpotlightComponent;
	class ProxyModelComponent;
	class TextComponent;
} // namespace library

class SpotlightDemo
	: public library::ConcreteMaterialSceneComponent<library::SpotlightMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(SpotlightDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit SpotlightDemo();
	~SpotlightDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;

private:
	void UpdateAmbientLight(const library::Time& time);
	void UpdateSpotlight(const library::Time& time);
	void UpdateSpecularLight(const library::Time& time);

	void Draw_SetData() override;

	float m_specularPower;
	library::Color m_specularColor;
	library::Color m_ambientColor;

	std::unique_ptr<library::SpotlightComponent> m_spotlight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::TextComponent> m_text;
};
