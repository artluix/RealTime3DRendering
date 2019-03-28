#pragma once
#include <library/Materials/FogMaterial.h>

#include <library/Components/ConcreteMaterialSceneComponent.hpp>
#include <library/Components/InputReceivableComponent.h>

#include <library/DirectXForwardDeclarations.h>
#include <library/Color.h>

#include <memory>

namespace library
{
	class DirectionalLightComponent;
	class ProxyModelComponent;
	class TextComponent;
} // namespace library

class FogDemo
	: public library::ConcreteMaterialSceneComponent<library::FogMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(FogDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit FogDemo();
	~FogDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData(const library::MeshData& meshData) override;

private:
	void UpdateAmbientLight(const library::Time& time);
	void UpdateDirectionalLight(const library::Time& time);
	void SetActiveTechnique();

	library::Color m_ambientColor;

	float m_fogStart;
	float m_fogRange;
	float m_fogEnabled;

	std::unique_ptr<library::DirectionalLightComponent> m_directionalLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::TextComponent> m_text;
};
