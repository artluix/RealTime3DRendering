#pragma once
#include "library/RTTI.hpp"

namespace library
{
	class KeyboardComponent;
	class MouseComponent;

	class InputReceivableComponent
	{
		RTTI_CLASS_BASE(InputReceivableComponent)

	public:
		virtual ~InputReceivableComponent() = default;

		const KeyboardComponent* GetKeyboard() const { return m_keyboard; }
		void SetKeyboard(const KeyboardComponent& keyboard);

		const MouseComponent* GetMouse() const { return m_mouse; }
		void SetMouse(const MouseComponent& mouse);

	protected:
		explicit InputReceivableComponent() = default;

		const KeyboardComponent* m_keyboard = nullptr;
		const MouseComponent* m_mouse = nullptr;
	};
} // namespace library
