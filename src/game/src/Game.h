#pragma once
#include <library/Application.h>
#include <library/Common.h>

#include <dinput.h>
#include <DirectXMath.h>

namespace library
{
	namespace components
	{
		class KeyboardComponent;
		class MouseComponent;
	} // namespace components
} // namespace library

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
} // namespace DirectX

class Game : public library::Application
{
public:
	explicit Game(
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
	std::shared_ptr<library::components::KeyboardComponent> m_keyboardComponent;
	std::shared_ptr<library::components::MouseComponent> m_mouseComponent;

	ComPtr<IDirectInput8> m_directInput;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

	DirectX::XMFLOAT2 m_mouseTextPosition;
}; // namespace library
