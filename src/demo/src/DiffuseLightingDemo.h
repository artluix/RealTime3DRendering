#pragma once
#include <library/Materials/DiffuseLightingMaterial.h>

#include <library/Components/ConcreteMaterialSceneComponent.hpp>
#include <library/Components/InputReceivableComponent.h>
#include <library/Color.h>

#include <memory>

namespace library
{
	class DirectionalLightComponent;
	class ProxyModelComponent;
	class TextComponent;
} // namespace library

class DiffuseLightingDemo
	: public library::ConcreteMaterialSceneComponent<library::DiffuseLightingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(DiffuseLightingDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit DiffuseLightingDemo();
	~DiffuseLightingDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;

private:
	void UpdateAmbientLight(const library::Time& time);
	void UpdateDirectionalLight(const library::Time& time);

	void Draw_SetData() override;

	library::Color m_ambientColor;

	std::unique_ptr<library::DirectionalLightComponent> m_directionalLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::TextComponent> m_text;
};
