#pragma once
#include "Materials/ModelTessellationMaterial.h"
#include <library/Components/ConcreteMaterialPrimitiveComponent.h>

#include <library/Components/InputReceivableComponent.h>

namespace library
{
class TextComponent;
} // namespace library

class ModelTessellationDemo
	: public library::ConcreteMaterialPrimitiveComponent<ModelTessellationMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(ModelTessellationDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	ModelTessellationDemo();

	void Update(const library::Time& time) override;

protected:
	void InitializeInternal() override;

	void Draw_SetData(const library::PrimitiveData& primitiveData) override;
	void Draw_Render(const library::PrimitiveData& primitiveData) override;

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

	void UpdateTechnique();

	library::Array<float, 3> m_tessEdgeFactors;
	float m_tessInsideFactor;

	bool m_isWireframeVisible;
	bool m_isDistanceModeEnabled;

	float m_maxTessFactor;
	float m_minTessDistance;
	float m_maxTessDistance;

	std::unique_ptr<library::TextComponent> m_text;
};
