#pragma once
#include <library/Application.h>
#include <library/Common.h>

#include <dinput.h>
#include <DirectXMath.h>

namespace library
{
    class FpsComponent;
    class KeyboardComponent;
    class MouseComponent;
}

namespace DirectX
{
    class SpriteBatch;
    class SpriteFont;
}


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
    std::unique_ptr<library::FpsComponent> m_fpsComponent;
    std::unique_ptr<library::KeyboardComponent> m_keyboardComponent;
    std::unique_ptr<library::MouseComponent> m_mouseComponent;

    ComPtr<IDirectInput8> m_directInput;

    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
    std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

    DirectX::XMFLOAT2 m_mouseTextPosition;
};
