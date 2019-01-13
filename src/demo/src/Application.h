#pragma once
#include <library/Application.h>
#include <library/CommonTypes.h>
#include <library/Color.h>

#include <dinput.h>

namespace library
{
	class KeyboardComponent;
	class MouseComponent;
} // namespace library

namespace demo
{
	class Application : public library::Application
	{
	public:
		explicit Application(
			const HINSTANCE instanceHandle,
			const std::wstring& windowClass,
			const std::wstring& windowTitle,
			const int showCmd
		);

		void Initialize() override;
		void Update(const library::Time& time) override;
		void Draw(const library::Time& time) override;

	protected:
		void Shutdown() override;

	private:
		std::shared_ptr<library::KeyboardComponent> m_keyboard;
		std::shared_ptr<library::MouseComponent> m_mouse;

		ComPtr<IDirectInput8> m_directInput;
	};
} // namespace demo
