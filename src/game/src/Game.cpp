#include "Game.h"

#include <library/Exception.h>
#include <library/FpsComponent.h>

#include <DirectXColors.h>

namespace
{
    const XMVECTORF32 BackgroundColor = Colors::Blue;
}

Game::Game(
    const HINSTANCE instanceHandle,
    const std::wstring& windowClass,
    const std::wstring& windowTitle,
    const int showCmd
)
    : Application(instanceHandle, windowClass, windowTitle, showCmd)
{
    m_depthStencilBufferEnabled = true;
    m_multiSamplingEnabled = true;
}

void Game::Initialize()
{
    m_fpsComponent = std::make_unique<library::FpsComponent>(*this);
    m_fpsComponent->SetVisible(true);
    m_components.push_back(m_fpsComponent.get());

    Application::Initialize();
}

void Game::Update(const library::Time& time)
{
    Application::Update(time);
}

void Game::Draw(const library::Time& time)
{
    m_d3dDeviceContext->ClearRenderTargetView(m_renderTargetView.Get(), BackgroundColor);
    m_d3dDeviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    Application::Draw(time);

    HRESULT hr = m_swapChain->Present(0, 0);
    if (FAILED(hr))
    {
        throw library::Exception("IDXGISwapChain::Present() failed.", hr);
    }
}

void Game::Shutdown()
{
    m_fpsComponent = nullptr;

    Application::Shutdown();
}
