#include "StdAfx.h"
#include "library/components/MouseComponent.h"

#include "library/Application.h"
#include "library/Exception.h"
#include "library/Math.h"

namespace library
{
	namespace components
	{

		namespace
		{
			constexpr BYTE k_buttonDownMask = 0x80;
		}

		MouseComponent::MouseComponent(const Application& app, ComPtr<IDirectInput8>& directInput)
			: Class(app)
			, m_directInput(directInput)
			, m_directInputDevice()
			, m_currentState{}, m_previousState{}
			, m_x(0), m_y(0), m_wheel(0)
		{
		}

		MouseComponent::~MouseComponent()
		{
			if (!!m_directInputDevice)
			{
				m_directInputDevice->Unacquire();
				m_directInputDevice.Reset();
			}
		}

		void MouseComponent::Initialize()
		{
			auto hr = m_directInput->CreateDevice(GUID_SysMouse, m_directInputDevice.GetAddressOf(), nullptr);
			if (FAILED(hr))
			{
				throw Exception("IDirectInput8::CreateDevice() failed.", hr);
			}

			hr = m_directInputDevice->SetDataFormat(&c_dfDIMouse);
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

		void MouseComponent::Update(const Time& time)
		{
			if (!m_directInputDevice)
				return;

			m_previousState = m_currentState;

			auto hr = m_directInputDevice->GetDeviceState(sizeof(m_currentState), &m_currentState);
			if (FAILED(hr))
			{
				// try to reacquire the device
				hr = m_directInputDevice->Acquire();
				if (SUCCEEDED(hr))
				{
					m_directInputDevice->GetDeviceState(sizeof(m_currentState), &m_currentState);
				}
			}

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

	} // namespace components
} // namespace library
