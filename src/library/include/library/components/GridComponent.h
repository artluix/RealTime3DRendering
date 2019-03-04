#pragma once
#include "library/Materials/BasicMaterial.h"

#include "library/Components/ConcreteMaterialSceneComponent.hpp"

#include "library/Color.h"
#include "library/DirectXForwardDeclarations.h"

namespace library
{
	class GridComponent : public ConcreteMaterialSceneComponent<BasicMaterial>
	{
		RTTI_CLASS(GridComponent, SceneComponent)

	public:
		explicit GridComponent();
		explicit GridComponent(
			const unsigned size,
			const unsigned scale,
			const Color& color
		);

		~GridComponent();

		unsigned GetSize() const { return m_size; }
		unsigned GetScale() const { return m_scale; }
		const Color& GetColor() const { return m_color; }

		void SetSize(const unsigned size);
		void SetScale(const unsigned scale);
		void SetColor(const Color& color);

		void Initialize(const Application& app) override;

	protected:
		void Draw_SetData() override;

	private:
		void Build();

		unsigned m_size;
		unsigned m_scale;
		Color m_color;
	};
} // namespace library
