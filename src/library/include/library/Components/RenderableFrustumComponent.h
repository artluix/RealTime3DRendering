#pragma once
#include "library/Materials/BasicMaterial.h"
#include "library/Components/ConcreteMaterialPrimitiveComponent.h"
#include "library/Math/Color.h"

namespace library
{

namespace math
{
class Frustum;
} // namespace math

class RenderableFrustumComponent : public ConcreteMaterialPrimitiveComponent<BasicMaterial>
{
	RTTI_CLASS(RenderableFrustumComponent, SceneComponent)

public:
	RenderableFrustumComponent();
	explicit RenderableFrustumComponent(const math::Color& color);

	void InitializeGeometry(const math::Frustum& frustum);

protected:
	void InitializeInternal() override;
	void Draw_SetData(const PrimitiveData& primitiveData) override;

private:
	void InitializeVertexBuffer(const math::Frustum& frustum);
	void InitializeIndexBuffer();

	math::Color m_color;
};
} // namespace library
