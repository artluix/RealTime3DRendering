#pragma once
#include <library/Materials/BasicMaterial.h>

#include <library/Components/ConcreteMaterialPrimitiveComponent.hpp>
#include <library/Components/InputReceivableComponent.h>

class BasicMaterialDemo
	: public library::ConcreteMaterialPrimitiveComponent<library::BasicMaterial>
	, public library::InputReceivableComponent
{
	RTTI_CLASS(BasicMaterialDemo, library::PrimitiveComponent, library::InputReceivableComponent)

public:
	BasicMaterialDemo() = default;

	void Update(const library::Time& time) override;

protected:
	void InitializeInternal() override;
	void Draw_SetData(const library::PrimitiveData& primitiveData) override;
};
