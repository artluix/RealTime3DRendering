#include "library/components/MouseComponent.h"

#include "library/Exception.h"
#include "library/Common.h"
#include "library/Math.hpp"

#include <cassert>

namespace library
{
    namespace
    {
        constexpr auto k_buttonDownMask = static_cast<BYTE>(0x80);
    }

    namespace components
    {
        Mouse::Mouse(const Application& app, ComPtr<IDirectInput8>& directInput)
            : Class(app)
            , m_directInput(directInput)
            , m_directInputDevice()
            , m_x(0), m_y(0), m_wheel(0)
            , m_currentState{}, m_previousState{}
        {
        }

        Mouse::~Mouse()
        {
            if (!!m_directInputDevice)
            {
                m_directInputDevice->Unacquire();
                m_directInputDevice.Reset();
            }
        }

        void Mouse::Initialize()
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

        void Mouse::Update(const Time& time)
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

        /////////////////////////////////////////////////////////////////

        bool Mouse::IsButtonUp(const MouseButton mb) const
        {
            return (m_currentState.rgbButtons[to_integral(mb)] & k_buttonDownMask) == 0;
        }

        bool Mouse::IsButtonDown(const MouseButton mb) const
        {
            return !IsButtonUp(mb);
        }

        bool Mouse::WasButtonUp(const MouseButton mb) const
        {
            return (m_previousState.rgbButtons[to_integral(mb)] & k_buttonDownMask) == 0;
        }

        bool Mouse::WasButtonDown(const MouseButton mb) const
        {
            return !WasButtonUp(mb);
        }

        bool Mouse::WasButtonPressed(const MouseButton mb) const
        {
            return IsButtonDown(mb) && WasButtonUp(mb);
        }

        bool Mouse::WasButtonReleased(const MouseButton mb) const
        {
            return IsButtonUp(mb) && WasButtonDown(mb);
        }

        bool Mouse::IsButtonHeldDown(const MouseButton mb) const
        {
            return IsButtonDown(mb) && WasButtonDown(mb);
        }
    }
}
