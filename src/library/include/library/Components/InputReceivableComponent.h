#pragma once
#include "library/Components/Component.h"

namespace library
{
class KeyboardComponent;
class MouseComponent;

class InputReceivableComponent : public virtual Component
{
	RTTI_CLASS_BASE(InputReceivableComponent)

public:
	const KeyboardComponent* GetKeyboard() const { return m_keyboard; }
	void SetKeyboard(const KeyboardComponent& keyboard);

	const MouseComponent* GetMouse() const { return m_mouse; }
	void SetMouse(const MouseComponent& mouse);

protected:
	InputReceivableComponent() = default;

	const KeyboardComponent* m_keyboard = nullptr;
	const MouseComponent* m_mouse = nullptr;
};
} // namespace library
