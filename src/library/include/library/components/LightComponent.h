#pragma once
#include "library/Color.h"

namespace library
{
	namespace components
	{
		class Light
		{
		public:
			virtual ~Light() = default;

			const Color& GetColor() const { return m_color; }
			void SetColor(const Color& color);

		protected:
			explicit Light(const Color& color = colors::White);

			Color m_color;
		};

	} // namespace components
} // namespace library
