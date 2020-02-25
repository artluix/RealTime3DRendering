#pragma once
#include "library/Components/InputComponent.h"
#include "library/Key.h"

#include <array>

namespace library
{
class KeyboardComponent : public InputComponent
{
	RTTI_CLASS(KeyboardComponent, InputComponent)

public:
	explicit KeyboardComponent(IDirectInput8& directInput);

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

	KeysStateArray m_keysState;
	KeysStateArray m_previousKeysState;
};
} // namespace library
