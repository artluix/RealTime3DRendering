#pragma once
#include "Materials/PointSpriteMaterial.h"

#include <library/Components/ConcreteMaterialPrimitiveComponent.h>
#include <library/Components/InputReceivableComponent.h>

namespace library
{
class TextComponent;
} // namespace library

class GeometryShaderDemo
	: public library::ConcreteMaterialPrimitiveComponent<PointSpriteMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(GeometryShaderDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	GeometryShaderDemo();

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

	void InitializeRandomPoints();
	void InitializeFixedPoints();

	std::unique_ptr<library::TextComponent> m_text;

	bool m_showRandomPoints;
};
