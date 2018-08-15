#pragma once
#include <library/Common.h>
#include <library/Application.h>

namespace library
{
    class FpsComponent;
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
};
