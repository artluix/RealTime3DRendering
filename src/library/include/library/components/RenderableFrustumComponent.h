#pragma once
#include "library/Materials/BasicMaterial.h"
#include "library/Components/ConcreteMaterialSceneComponent.hpp"

#include "library/Color.h"

namespace library
{
	class Frustum;

	class RenderableFrustumComponent : public ConcreteMaterialSceneComponent<BasicMaterial>
	{
		RTTI_CLASS(RenderableFrustumComponent, SceneComponent)

	public:
		explicit RenderableFrustumComponent();
		explicit RenderableFrustumComponent(const Color& color);

		void InitializeGeometry(const Frustum& frustum);

		void Initialize(const Application& app) override;

	protected:
		void Draw_SetData() override;

	private:
		void InitializeVertexBuffer(const Frustum& frustum);
		void InitializeIndexBuffer();

		Color m_color;
	};
} // namespace library

