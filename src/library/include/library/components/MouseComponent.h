#pragma once
#include "library/components/InputComponent.h"

#include <cstdint>

namespace library
{
	enum class MouseButton : std::uint8_t
	{
		Left = 0,
		Right,
		Middle,
		X1
	};

	class MouseComponent : public InputComponent
	{
		RTTI_CLASS(MouseComponent, InputComponent)

	public:
		explicit MouseComponent(ComPtr<IDirectInput8>& directInput);

		void Update(const Time& time) override;

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
		DIMOUSESTATE m_currentState;
		DIMOUSESTATE m_previousState;

		long m_x, m_y, m_wheel;
	};
} // namespace library
