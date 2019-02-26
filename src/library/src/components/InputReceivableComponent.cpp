#include "StdAfx.h"
#include "library/components/InputReceivableComponent.h"

namespace library
{
	void InputReceivableComponent::SetKeyboard(const KeyboardComponent& keyboard)
	{
		m_keyboard = &keyboard;
	}

	void InputReceivableComponent::SetMouse(const MouseComponent& mouse)
	{
		m_mouse = &mouse;
	}
} // namespace library
