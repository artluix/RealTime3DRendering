#pragma once
#include "library/NonCopyable.hpp"
#include "library/RTTI.hpp"

namespace library
{
	class Application;
	struct Time;

	class Component : public NonCopyable<Component>
	{
		RTTI_CLASS_BASE(Component)

	public:
		virtual ~Component() = default;

		bool IsEnabled() const { return m_enabled; }
		void SetEnabled(const bool enabled);

		virtual void Initialize(const Application& app);
		virtual void Update(const Time& time) {}

	protected:
		explicit Component() = default;

		const Application* m_app = nullptr;
		bool m_enabled = true;
	};
} // namespace library
