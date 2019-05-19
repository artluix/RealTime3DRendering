#pragma once
#include "library/Application.h"
#include "library/NonCopyable.hpp"
#include "library/RTTI.hpp"

namespace library
{
struct Time;

// abstract component
class Component : public NonCopyable<Component>
{
	RTTI_CLASS_BASE(Component)

	// will set m_app
	friend void Application::Initialize();

public:
	virtual ~Component() = default;

	bool IsEnabled() const { return m_enabled; }
	void SetEnabled(const bool enabled) { m_enabled = enabled; }

	const Application& GetApp() const { return *m_app; }

	virtual void Initialize() = 0;
	virtual void Update(const Time& time) = 0;

protected:
	Component() = default;

private:
	bool m_enabled = true;
	const Application* m_app = nullptr;
};
} // namespace library
