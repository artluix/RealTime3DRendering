#pragma once
#include "library/components/SceneComponent.h"
#include "library/components/InputReceivableComponent.h"
#include "library/effect/materials/BasicEffectMaterial.h"
#include "library/Color.h"
#include "library/DirectXForwardDeclarations.h"

namespace library
{
	class Effect;

	class GridComponent
		: public SceneComponent
		, public InputReceivableComponent
	{
		RTTI_CLASS(GridComponent, SceneComponent, InputReceivableComponent)

	public:
		explicit GridComponent(const Application& app);
		explicit GridComponent(
			const Application& app,
			const unsigned size,
			const unsigned scale,
			const Color& color
		);

		~GridComponent() = default;

		unsigned GetSize() const { return m_size; }
		unsigned GetScale() const { return m_scale; }
		const Color& GetColor() const { return m_color; }

		void SetSize(const unsigned size);
		void SetScale(const unsigned scale);
		void SetColor(const Color& color);

		void Initialize() override;
		void Update(const Time& time) override;
		void Draw(const Time& time) override;

		const EffectMaterial* GetEffectMaterial() const override { return m_material.get(); }

	protected:
		EffectMaterial* GetEffectMaterial() override { return m_material.get(); }

	private:
		void Build();

		std::shared_ptr<Effect> m_effect;
		std::unique_ptr<BasicEffectMaterial> m_material;

		unsigned m_size;
		unsigned m_scale;
		Color m_color;
	};
} // namespace library
