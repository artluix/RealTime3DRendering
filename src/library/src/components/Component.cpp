#include "StdAfx.h"
#include "library/Components/Component.h"

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
		assert(!!m_app);
	}
} // namespace library
