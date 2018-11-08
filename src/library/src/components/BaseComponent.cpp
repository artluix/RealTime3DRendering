#include "StdAfx.h"
#include "library/components/BaseComponent.h"

#include "library/Application.h"

namespace library
{
	namespace components
	{
		BaseComponent::BaseComponent(const Application& app)
			: m_app(app)
			, m_enabled(true)
		{
		}

		void BaseComponent::SetEnabled(const bool enabled)
		{
			m_enabled = enabled;
		}

		void BaseComponent::Initialize()
		{
		}

		void BaseComponent::Update(const Time& time)
		{
		}
	} // namespace components
} // namespace library
