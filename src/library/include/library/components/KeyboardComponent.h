#pragma once
#include "library/components/Component.h"
#include "library/Key.h"

#include <dinput.h>
#include <array>

namespace library
{
	class KeyboardComponent : public Component
	{
		RTTI_CLASS(KeyboardComponent, Component)

	public:
		explicit KeyboardComponent(const Application& app, const ComPtr<IDirectInput8>& directInput);
		~KeyboardComponent();

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

} // namespace library
