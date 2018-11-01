#include "library/components/KeyboardComponent.h"

#include "library/Application.h"
#include "library/Exception.h"

#include <cassert>

namespace library
{
	namespace components
	{

		KeyboardComponent::KeyboardComponent(const Application& app, const ComPtr<IDirectInput8>& directInput)
			: Class(app)
			, m_directInput(directInput)
			, m_directInputDevice()
		{
			assert(!!directInput);
			m_keysState.fill(0);
			m_previousKeysState.fill(0);
		}

		KeyboardComponent::~KeyboardComponent()
		{
			if (!!m_directInputDevice)
			{
				m_directInputDevice->Unacquire();
				m_directInputDevice.Reset();
			}
		}

		void KeyboardComponent::Initialize()
		{
			auto hr = m_directInput->CreateDevice(GUID_SysKeyboard, m_directInputDevice.GetAddressOf(), nullptr);
			if (FAILED(hr))
			{
				throw Exception("IDirectInput8::CreateDevice() failed.", hr);
			}

			hr = m_directInputDevice->SetDataFormat(&c_dfDIKeyboard);
			if (FAILED(hr))
			{
				throw Exception("IDirectInputDevice::SetDataFormat() failed.", hr);
			}

			hr = m_directInputDevice->SetCooperativeLevel(m_app.GetWindowHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			if (FAILED(hr))
			{
				throw Exception("IDirectInputDevice::SetCooperativeLevel() failed.", hr);
			}

			hr = m_directInputDevice->Acquire();
			if (FAILED(hr))
			{
				throw Exception("IDirectInputDevice8::Acquire() failed.", hr);
			}
		}

		void KeyboardComponent::Update(const Time& time)
		{
			if (!m_directInputDevice)
				return;

			m_previousKeysState = m_keysState;

			auto hr = m_directInputDevice->GetDeviceState(m_keysState.size(), m_keysState.data());
			if (FAILED(hr))
			{
				// try to reacquire the device
				hr = m_directInputDevice->Acquire();
				if (SUCCEEDED(hr))
				{
					m_directInputDevice->GetDeviceState(m_keysState.size(), m_keysState.data());
				}
			}
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

	} // namespace components
} // namespace library
