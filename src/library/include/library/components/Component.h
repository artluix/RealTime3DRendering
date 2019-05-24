#pragma once
#include "library/NonCopyable.hpp"
#include "library/RTTI.hpp"

namespace library
{
class Application;
struct Time;

// abstract component
class Component : public NonCopyable<Component>
{
	RTTI_CLASS_BASE(Component)

public:
	virtual ~Component() = default;

	bool IsEnabled() const { return m_enabled; }
	void SetEnabled(const bool enabled) { m_enabled = enabled; }

	bool IsInitialized() const { return m_initialized; }

	const Application& GetApp() const { return *m_app; }

	virtual void Update(const Time& time) = 0;

	void Initialize(const Application& app)
	{
		if (m_initialized)
			return;

		m_app = &app;
		InitializeInternal();

		m_initialized = true;
	}

protected:
	Component() = default;

	virtual void InitializeInternal() = 0;

private:
	bool m_enabled = true;
	bool m_initialized = false;
	const Application* m_app = nullptr;
};
} // namespace library
