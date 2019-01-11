#include "StdAfx.h"
#include "library/components/KeyboardComponent.h"

namespace library
{
	namespace
	{
		constexpr BYTE k_keyDownMask = 0x80;
	}

	KeyboardComponent::KeyboardComponent(const Application& app, const ComPtr<IDirectInput8>& directInput)
		: InputComponent(app, directInput, c_dfDIKeyboard, GUID_SysKeyboard, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)
	{
		m_keysState.fill(0);
		m_previousKeysState.fill(0);
	}

	void KeyboardComponent::Update(const Time& time)
	{
		if (!m_directInputDevice)
			return;

		m_previousKeysState = m_keysState;
		GetState(m_keysState.size(), m_keysState.data());
	}

	//-------------------------------------------------------------------------

	bool KeyboardComponent::IsKeyUp(const Key key) const
	{
		return (m_keysState[static_cast<unsigned>(key)] & k_keyDownMask) == 0;
	}

	bool KeyboardComponent::IsKeyDown(const Key key) const
	{
		return !IsKeyUp(key);
	}

	bool KeyboardComponent::WasKeyUp(const Key key) const
	{
		return (m_previousKeysState[static_cast<unsigned>(key)] & k_keyDownMask) == 0;
	}

	bool KeyboardComponent::WasKeyDown(const Key key) const
	{
		return !WasKeyUp(key);
	}

	bool KeyboardComponent::WasKeyPressed(const Key key) const
	{
		return IsKeyDown(key) && WasKeyUp(key);
	}

	bool KeyboardComponent::WasKeyReleased(const Key key) const
	{
		return IsKeyUp(key) && WasKeyDown(key);
	}

	bool KeyboardComponent::IsKeyHeldDown(const Key key) const
	{
		return IsKeyDown(key) && WasKeyDown(key);
	}
} // namespace library
