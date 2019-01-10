#pragma once
#include "library/NonCopyable.hpp"
#include "library/RTTI.hpp"
#include "library/Common.h"

namespace library
{
	class Application;
	struct Time;

	class Component : public NonCopyable<Component>
	{
		RTTI_CLASS_BASE(Component)

	public:
		virtual ~Component();

		bool IsEnabled() const { return m_enabled; }
		void SetEnabled(const bool enabled);

		virtual void Initialize() = 0;
		virtual void Update(const Time& time) = 0;

	protected:
		explicit Component() = default;

		bool m_enabled = true;
	};

} // namespace library
