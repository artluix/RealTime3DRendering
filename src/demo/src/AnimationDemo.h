#pragma once
#include <library/Materials/SkinnedModelMaterial.h>
#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>

#include <library/Components/InputReceivableComponent.h>

#include <library/math/Color.h>
#include <library/CommonTypes.h>
#include <library/DirectXForwardDeclarations.h>

#include <memory>
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
	: public library::ConcreteMaterialPrimitiveComponent<library::SkinnedModelMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(AnimationDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit AnimationDemo();

	void Initialize() override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

private:
	void UpdateOptions();

	void UpdateAmbientLight(const library::Time& time);
	void UpdatePointLight(const library::Time& time);
	void UpdateSpecularLight(const library::Time& time);

	std::unique_ptr<library::TextComponent> m_text;
	std::unique_ptr<library::PointLightComponent> m_pointLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::AnimationPlayerComponent> m_animationPlayer;

	std::unique_ptr<Model> m_model;

	bool m_manualAdvanceMode;

	float m_specularPower;
	library::Color m_specularColor;
	library::Color m_ambientColor;
};
