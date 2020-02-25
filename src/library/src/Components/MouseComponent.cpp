#include "StdAfx.h"
#include "library/Components/MouseComponent.h"

namespace library
{

namespace
{
constexpr BYTE k_buttonDownMask = 0x80;
}

MouseComponent::MouseComponent(IDirectInput8& directInput)
	: InputComponent(directInput, c_dfDIMouse, GUID_SysMouse, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)
	, m_currentState{}
	, m_previousState{}
	, m_x(0)
	, m_y(0)
	, m_wheel(0)
{}

void MouseComponent::Update(const Time& time)
{
	if (!m_directInputDevice)
		return;

	m_previousState = m_currentState;
	GetState(sizeof(m_currentState), &m_currentState);

	// accumulate position
	m_x += m_currentState.lX;
	m_y += m_currentState.lY;

	m_wheel += m_currentState.lZ;
}

//-------------------------------------------------------------------------

bool MouseComponent::IsButtonUp(const MouseButton mb) const
{
	return (m_currentState.rgbButtons[static_cast<unsigned>(mb)] & k_buttonDownMask) == 0;
}

bool MouseComponent::IsButtonDown(const MouseButton mb) const
{
	return !IsButtonUp(mb);
}

bool MouseComponent::WasButtonUp(const MouseButton mb) const
{
	return (m_previousState.rgbButtons[static_cast<unsigned>(mb)] & k_buttonDownMask) == 0;
}

bool MouseComponent::WasButtonDown(const MouseButton mb) const
{
	return !WasButtonUp(mb);
}

bool MouseComponent::WasButtonPressed(const MouseButton mb) const
{
	return IsButtonDown(mb) && WasButtonUp(mb);
}

bool MouseComponent::WasButtonReleased(const MouseButton mb) const
{
	return IsButtonUp(mb) && WasButtonDown(mb);
}

bool MouseComponent::IsButtonHeldDown(const MouseButton mb) const
{
	return IsButtonDown(mb) && WasButtonDown(mb);
}
} // namespace library
