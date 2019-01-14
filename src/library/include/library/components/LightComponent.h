#pragma once
#include "library/RTTI.hpp"
#include "library/NonCopyable.hpp"
#include "library/Color.h"

namespace library
{
	class LightComponent : public NonCopyable<LightComponent>
	{
		RTTI_CLASS_BASE(LightComponent)

	public:
		virtual ~LightComponent() = default;

		const Color& GetColor() const { return m_color; }
		void SetColor(const Color& color);

	protected:
		explicit LightComponent(const Color& color = Color::White);

	private:
		Color m_color;
	};
} // namespace library
