#pragma once
#include "library/Color.h"

namespace library
{
	class Light
	{
	public:
		explicit Light(const Color& color = colors::White);
		virtual ~Light() = default;

		const Color& GetColor() const { return m_color; }
		void SetColor(const Color& color);

	protected:
		Color m_color;
	};

} // namespace library
