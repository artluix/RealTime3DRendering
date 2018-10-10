#pragma once
#include <library/components/BaseComponent.h>

#include <cstdint>
#include <dinput.h>

namespace library
{
    struct Time;

    enum class MouseButton : std::uint8_t
    {
        Left = 0,
        Right,
        Middle,
        X1
    };

    namespace components
    {
        class MouseComponent : public rtti::Class<MouseComponent, BaseComponent>
        {
        public:
            explicit MouseComponent(const Application& app, ComPtr<IDirectInput8>& directInput);
            ~MouseComponent();

            void Initialize() override;
            void Update(const Time& time) override;

            // dinput doesn't return a pixel coordinates
            // so we should limit them
            long GetX() const { return m_x; }
            long GetY() const { return m_y; }
            long GetWheel() const { return m_wheel; }

            bool IsButtonUp(const MouseButton mb) const;
            bool IsButtonDown(const MouseButton mb) const;

            bool WasButtonUp(const MouseButton mb) const;
            bool WasButtonDown(const MouseButton mb) const;

            bool WasButtonPressed(const MouseButton mb) const;
            bool WasButtonReleased(const MouseButton mb) const;

            bool IsButtonHeldDown(const MouseButton mb) const;

        private:
            ComPtr<IDirectInput8> m_directInput;
            ComPtr<IDirectInputDevice8> m_directInputDevice;

            DIMOUSESTATE m_currentState;
            DIMOUSESTATE m_previousState;

            long m_x, m_y, m_wheel;
        };
    } // namespace components
} // namespace library
