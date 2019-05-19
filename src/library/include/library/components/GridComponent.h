#pragma once
#include "library/Materials/BasicMaterial.h"
#include "library/Components/ConcreteMaterialPrimitiveComponent.hpp"
#include "library/Color.h"
#include "library/DirectXForwardDeclarations.h"

namespace library
{
class GridComponent : public ConcreteMaterialPrimitiveComponent<BasicMaterial>
{
	RTTI_CLASS(GridComponent, PrimitiveComponent)

public:
	GridComponent();
	GridComponent(const unsigned size, const unsigned scale, const Color& color);

	~GridComponent();

	unsigned GetSize() const { return m_size; }
	unsigned GetScale() const { return m_scale; }
	const Color& GetColor() const { return m_color; }

	void SetSize(const unsigned size);
	void SetScale(const unsigned scale);
	void SetColor(const Color& color);

	void Initialize() override;

protected:
	void Draw_SetData(const PrimitiveData& primitiveData) override;

private:
	void Build();

	unsigned m_size;
	unsigned m_scale;
	Color m_color;
};
} // namespace library
