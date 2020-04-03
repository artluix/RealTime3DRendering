#pragma once
#include "Materials/QuadHeightMapTessellationMaterial.h"

#include <library/Components/ConcreteMaterialPrimitiveComponent.h>
#include <library/Components/InputReceivableComponent.h>

namespace library
{
class TextComponent;
class UIComponent;
} // namespace library

class QuadHeightMapTessellationDemo
	: public library::ConcreteMaterialPrimitiveComponent<QuadHeightMapTessellationMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(QuadHeightMapTessellationDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	QuadHeightMapTessellationDemo();

	void Update(const library::Time& time) override;

protected:
	void InitializeInternal() override;

	void Draw_SetData(const library::PrimitiveData& primitiveData) override;
	void Draw_Render(const library::PrimitiveData& primitiveData) override;

private:
	void UpdateDisplacementScale(const library::Time& time);

	struct Texture
	{
		enum Type : unsigned
		{
			HeightMap = 0,

			//# Count
			Count
		};
	};

	library::Array<float, 4> m_tessEdgeFactors;
	library::Array<float, 2> m_tessInsideFactors;

	float m_displacementScale;

	library::math::Matrix4 m_textureMatrix;
	library::math::Vector2 m_texturePosition;
	bool m_isAnimationEnabled;

	std::unique_ptr<library::TextComponent> m_text;
	std::unique_ptr<library::UIComponent> m_uiHeightMap;
};
