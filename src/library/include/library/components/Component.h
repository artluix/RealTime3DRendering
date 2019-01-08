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
		explicit Component(const Application& app);
		virtual ~Component();

		bool IsEnabled() const { return m_enabled; }
		void SetEnabled(const bool enabled);

		const Application& GetApp() const { return m_app; }

		virtual void Initialize() = 0;
		virtual void Update(const Time& time) = 0;

	protected:
		const Application& m_app;
		bool m_enabled = true;
	};

} // namespace library
