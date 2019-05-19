#pragma once
#include "library/Materials/BasicMaterial.h"
#include "library/Components/ConcreteMaterialPrimitiveComponent.hpp"
#include "library/Color.h"

namespace library
{
class Frustum;

class RenderableFrustumComponent : public ConcreteMaterialPrimitiveComponent<BasicMaterial>
{
	RTTI_CLASS(RenderableFrustumComponent, SceneComponent)

public:
	RenderableFrustumComponent();
	explicit RenderableFrustumComponent(const Color& color);

	void InitializeGeometry(const Frustum& frustum);

	void Initialize() override;

protected:
	void Draw_SetData(const PrimitiveData& primitiveData) override;

private:
	void InitializeVertexBuffer(const Frustum& frustum);
	void InitializeIndexBuffer();

	Color m_color;
};
} // namespace library
