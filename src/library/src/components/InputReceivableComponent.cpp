#include "StdAfx.h"
#include "library/Components/InputReceivableComponent.h"

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
