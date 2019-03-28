#pragma once
#include <library/Materials/SkinnedModelMaterial.h>
#include <library/Components/ConcreteMaterialSceneComponent.hpp>

#include <library/Components/InputReceivableComponent.h>

#include <library/Color.h>
#include <library/CommonTypes.h>
#include <library/DirectXForwardDeclarations.h>

#include <memory>
#include <optional>
#include <vector>

namespace library
{
	class PointLightComponent;
	class ProxyModelComponent;
	class TextComponent;
	class AnimationPlayerComponent;

	class Model;
} // namespace library

class AnimationDemo
	: public library::ConcreteMaterialSceneComponent<library::SkinnedModelMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(AnimationDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit AnimationDemo();

	void Initialize(const library::Application& app) override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData(const library::MeshData& meshData) override;

private:
	void UpdateOptions();

	void UpdateAmbientLight(const library::Time& time);
	void UpdatePointLight(const library::Time& time);
	void UpdateSpecularLight(const library::Time& time);

	std::unique_ptr<library::TextComponent> m_text;
	std::unique_ptr<library::PointLightComponent> m_pointLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::AnimationPlayerComponent> m_animationPlayer;

	bool m_manualAdvanceMode;

	float m_specularPower;
	library::Color m_specularColor;
	library::Color m_ambientColor;
};
