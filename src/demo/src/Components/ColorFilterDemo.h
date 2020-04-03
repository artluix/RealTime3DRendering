#pragma once
#include "Materials/ColorFilterMaterial.h"

#include <library/Components/ConcreteMaterialPostProcessingComponent.h>
#include <library/Components/InputReceivableComponent.h>
#include <library/Math/Matrix.h>

namespace library
{
class TextComponent;
} // namespace library

class ColorFilterDemo
	: public library::ConcreteMaterialPostProcessingComponent<ColorFilterMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(ColorFilterDemo, library::PostProcessingComponent, library::InputReceivableComponent)

public:
	enum class Type
	{
		Grayscale,
		Inverse,
		Sepia,
		Generic,

		First = Grayscale,
		Last = Generic,
	};

	static std::string ToTechniqueName(const Type t);
	static std::string ToString(const Type t);

	static Type Next(const Type t);

	//-------------------------------------------------------------------------

	ColorFilterDemo();
	~ColorFilterDemo();

	void Update(const library::Time& time) override;
	void Draw(const library::Time& time) override;

protected:
	void InitializeInternal() override;

private:
	void UpdateGenericFilter(const library::Time& time);

	Type m_type;
	library::math::Matrix4 m_genericFilter;
	std::unique_ptr<library::TextComponent> m_text;
};
