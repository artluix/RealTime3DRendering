#include "StdAfx.h"
#include "library/components/InputReceivableComponent.h"

namespace library
{
	void InputReceivableComponent::SetKeyboard(const KeyboardComponent& keyboard)
	{
		if (m_keyboard != &keyboard)
			m_keyboard = &keyboard;
	}

	void InputReceivableComponent::SetMouse(const MouseComponent& mouse)
	{
		if (m_mouse != &mouse)
			m_mouse = &mouse;
	}

} // namespace library
