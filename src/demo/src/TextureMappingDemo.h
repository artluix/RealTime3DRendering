#pragma once
#include <library/Materials/TextureMappingMaterial.h>
#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>

class TextureMappingDemo
	: public library::ConcreteMaterialPrimitiveComponent<library::TextureMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(TextureMappingDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	TextureMappingDemo() = default;

	void Initialize() override;
	void Update(const library::Time& time) override;

protected:
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
};
