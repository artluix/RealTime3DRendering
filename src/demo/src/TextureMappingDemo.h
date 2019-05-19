#pragma once
#include <library/Materials/TextureMappingMaterial.h>

#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>

class TextureMappingDemo
	: public library::ConcreteMaterialPrimitiveComponent<library::TextureMappingMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(TextureMappingDemo, library::SceneComponent, library::InputReceivableComponent)

public:
	explicit TextureMappingDemo();

	void Initialize() override;
	void Update(const library::Time& time) override;

protected:
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;
};
