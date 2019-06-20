#pragma once
#include "Materials/SkinnedModelMaterial.h"

#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>

#include <library/Math/Color.h>

namespace library
{
class PointLightComponent;
class ProxyModelComponent;
class TextComponent;
class AnimationPlayerComponent;

class Model;
} // namespace library

class AnimationDemo
	: public library::ConcreteMaterialPrimitiveComponent<SkinnedModelMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(AnimationDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	AnimationDemo();
	~AnimationDemo();

	void Update(const library::Time& time) override;

protected:
	void InitializeInternal() override;
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

private:
	struct Texture
	{
		enum Type : unsigned
		{
			Default = 0,

			//# Count
			Count
		};
	};

	void UpdateOptions();

	void UpdateAmbientLight(const library::Time& time);
	void UpdatePointLight(const library::Time& time);
	void UpdateSpecularLight(const library::Time& time);

	std::unique_ptr<library::TextComponent> m_text;
	std::unique_ptr<library::PointLightComponent> m_pointLight;
	std::unique_ptr<library::ProxyModelComponent> m_proxyModel;
	std::unique_ptr<library::AnimationPlayerComponent> m_animationPlayer;

	std::unique_ptr<library::Model> m_model;

	bool m_manualAdvanceMode;

	float m_specularPower;
	library::math::Color m_specularColor;
	library::math::Color m_ambientColor;
};
