#pragma once
#include <library/Application.h>

#include <dinput.h>

namespace library
{
    class FpsComponent;
    class KeyboardComponent;
}

class Game : public library::Application
{
public:
    explicit Game(const HINSTANCE instanceHandle, const std::wstring& windowClass, const std::wstring& windowTitle, const int showCmd);

    void Initialize() override;
    void Update(const library::Time& time) override;
    void Draw(const library::Time& time) override;

protected:
    void Shutdown() override;

private:
    std::unique_ptr<library::FpsComponent> m_fpsComponent;
    std::unique_ptr<library::KeyboardComponent> m_keyboardComponent;

    ComPtr<IDirectInput8> m_directInput;
};
