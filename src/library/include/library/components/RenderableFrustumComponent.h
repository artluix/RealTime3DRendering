#pragma once
#include "library/Materials/BasicMaterial.h"
#include "library/Components/ConcreteMaterialPrimitiveComponent.hpp"
#include "library/math/Color.h"

namespace library
{
class Frustum;

class RenderableFrustumComponent : public ConcreteMaterialPrimitiveComponent<BasicMaterial>
{
	RTTI_CLASS(RenderableFrustumComponent, SceneComponent)

public:
	RenderableFrustumComponent();
	explicit RenderableFrustumComponent(const math::Color& color);

	void InitializeGeometry(const Frustum& frustum);

protected:
	void InitializeInternal() override;
	void Draw_SetData(const PrimitiveData& primitiveData) override;

private:
	void InitializeVertexBuffer(const Frustum& frustum);
	void InitializeIndexBuffer();

	math::Color m_color;
};
} // namespace library
