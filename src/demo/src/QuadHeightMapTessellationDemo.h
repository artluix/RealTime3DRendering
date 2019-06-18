#pragma once
#include <library/Materials/QuadHeightMapTessellationMaterial.h>
#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>

#include <library/Components/InputReceivableComponent.h>

#include <memory>

namespace library
{
class TextComponent;
class UIComponent;
} // namespace library

class QuadHeightMapTessellationDemo
	: public library::ConcreteMaterialPrimitiveComponent<library::QuadHeightMapTessellationMaterial>
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

	std::array<float, 4> m_tessEdgeFactors;
	std::array<float, 2> m_tessInsideFactors;

	float m_displacementScale;

	library::math::Matrix4 m_textureMatrix;
	library::math::Vector2 m_texturePosition;
	bool m_isAnimationEnabled;

	std::unique_ptr<library::TextComponent> m_text;
	std::unique_ptr<library::UIComponent> m_uiHeightMap;
};
