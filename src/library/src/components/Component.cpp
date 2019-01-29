#include "StdAfx.h"
#include "library/components/Component.h"

#include <cassert>

namespace library
{
	void Component::SetEnabled(const bool enabled)
	{
		m_enabled = enabled;
	}

	void Component::Initialize(const Application& app)
	{
		assert(!m_app);
		m_app = &app;
	}
} // namespace library
