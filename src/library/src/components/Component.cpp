#include "StdAfx.h"
#include "library/components/Component.h"

#include "library/Application.h"

namespace library
{
	Component::Component(const Application& app)
		: m_app(app)
	{
	}

	Component::~Component() = default;

	void Component::SetEnabled(const bool enabled)
	{
		m_enabled = enabled;
	}

} // namespace library
