#pragma once
#include "library/components/BaseComponent.h"
#include "library/Key.h"

#include <dinput.h>
#include <array>

namespace library
{
    namespace components
    {
        class Keyboard : public rtti::Class<Keyboard, Base>
        {
        public:
            explicit Keyboard(const Application& app, const ComPtr<IDirectInput8>& directInput);
            ~Keyboard();

            void Initialize() override;
            void Update(const Time& time) override;

            bool IsKeyUp(const Key key) const;
            bool IsKeyDown(const Key key) const;

            bool WasKeyUp(const Key key) const;
            bool WasKeyDown(const Key key) const;

            bool WasKeyPressed(const Key key) const;
            bool WasKeyReleased(const Key key) const;

            bool IsKeyHeldDown(const Key key) const;

        private:
            using KeysStateArray = std::array<BYTE, 256>;

            ComPtr<IDirectInput8> m_directInput;
            ComPtr<IDirectInputDevice8> m_directInputDevice;

            KeysStateArray m_keysState;
            KeysStateArray m_previousKeysState;
        };
    }
}
