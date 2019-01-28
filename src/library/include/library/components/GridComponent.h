#pragma once
#include "library/materials/BasicMaterial.h"

#include "library/components/SceneComponent.h"
#include "library/components/ConcreteMaterialComponent.hpp"
#include "library/components/InputReceivableComponent.h"

#include "library/Color.h"
#include "library/DirectXForwardDeclarations.h"

namespace library
{
	class GridComponent
		: public SceneComponent
		, public ConcreteMaterialComponent<BasicMaterial>
		, public InputReceivableComponent
	{
		RTTI_CLASS(GridComponent, SceneComponent, MaterialComponent, InputReceivableComponent)

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
		void Update(const Time& time) override;
		void Draw(const Time& time) override;

	private:
		void Build();

		unsigned m_size;
		unsigned m_scale;
		Color m_color;
	};
} // namespace library
