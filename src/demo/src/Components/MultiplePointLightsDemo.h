#pragma once
#include "Materials/MultiplePointLightsMaterial.h"

#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>

namespace library
{
class TextComponent;
class ProxyModelComponent;
class PointLightComponent;
class FullScreenQuadComponent;

class MultipleRenderTarget;
} // namespace library

class MultiplePointLightsDemo
	: public library::ConcreteMaterialPrimitiveComponent<MultiplePointLightsMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(MultiplePointLightsDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	MultiplePointLightsDemo();

	void Update(const library::Time& time) override;

protected:
	void InitializeInternal() override;
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;

private:
	// part from PointLightDemo
	struct Texture
	{
		enum Type : unsigned
		{
			Default = 0,

			//# Count
			Count
		};
	};

	struct Target
	{
		enum Type : unsigned
		{
			Color = 0,
			Normal,
			WorldPosition,

			//# Count
			Count
		};
	};

	//-------------------------------------------------------------------------

	void UpdateAmbientLight(const library::Time& time);
	void UpdatePointLight(const library::Time& time);
	void UpdateSpecularLight(const library::Time& time);

	float m_specularPower;
	library::math::Color m_specularColor;
	library::math::Color m_ambientColor;

	struct PointLightGlue
	{
		std::unique_ptr<library::ProxyModelComponent> model;
		std::unique_ptr<library::PointLightComponent> light;
	};

	std::vector<PointLightGlue> m_lightGlues;
	std::unique_ptr<library::TextComponent> m_text;

	library::math::Color m_lightColor; // use common color

	unsigned m_lightsCount;

	// deferred only
	bool m_isDeferred;
	std::unique_ptr<library::MultipleRenderTarget> m_renderTarget;
	std::unique_ptr<library::FullScreenQuadComponent> m_fullScreenQuad;
};
