#include "Game.h"

#include <library/components/FpsComponent.h>
#include <library/components/KeyboardComponent.h>
#include <library/components/MouseComponent.h>
#include <library/components/GridComponent.h>
#include <library/components/FirstPersonCameraComponent.h>
#include <library/components/TriangleDemoComponent.h>
#include <library/components/CubeDemoComponent.h>

#include <library/Exception.h>
#include <library/Path.h>
#include <library/Utils.h>

#include <SpriteFont.h>
#include <SpriteBatch.h>
#include <DirectXColors.h>

#include <sstream>
#include <iomanip>

namespace
{
	const auto k_backgroundColor = DirectX::Colors::Black;
	const auto k_fontPath = library::utils::GetExecutableDirectory().Join(
		library::filesystem::Path(L"data/fonts/Arial_14_Regular.spritefont")
	);
}

Game::Game(
	const HINSTANCE instanceHandle,
	const std::wstring& windowClass,
	const std::wstring& windowTitle,
	const int showCmd
)
	: Application(instanceHandle, windowClass, windowTitle, showCmd)
	, m_keyboardComponent()
	, m_mouseComponent()
	, m_directInput()
	, m_spriteBatch()
	, m_spriteFont()
	, m_mouseTextPosition(0.f, 50.f)
{
	m_depthStencilBufferEnabled = true;
	m_multiSamplingEnabled = true;
}

void Game::Initialize()
{
	HRESULT hr = DirectInput8Create(
		m_instanceHandle,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		reinterpret_cast<LPVOID*>(m_directInput.GetAddressOf()),
		nullptr
	);
	if (FAILED(hr))
	{
		throw library::Exception("DirectInput8Create() failed", hr);
	}

	using namespace library;
	using namespace components;

	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(GetD3DDeviceContext());
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(GetD3DDevice(), k_fontPath.GetAsWideCString());

	m_keyboardComponent = std::make_shared<KeyboardComponent>(*this, m_directInput);
	m_components.push_back(m_keyboardComponent);
	m_services.AddService(m_keyboardComponent->GetTypeId(), m_keyboardComponent.get());

	m_mouseComponent = std::make_shared<MouseComponent>(*this, m_directInput);
	m_components.push_back(m_mouseComponent);
	m_services.AddService(m_mouseComponent->GetTypeId(), m_mouseComponent.get());

	auto cameraComponent = std::make_shared<FirstPersonCameraComponent>(*this, *m_keyboardComponent, *m_mouseComponent);
	m_components.push_back(cameraComponent);
	m_services.AddService(cameraComponent->GetTypeId(), cameraComponent.get());

	auto fpsComponent = std::make_shared<FpsComponent>(*this);
	m_components.push_back(fpsComponent);

	auto gridComponent = std::make_shared<GridComponent>(*this, *cameraComponent);
	m_components.push_back(gridComponent);

	auto triangleDemoComponent = std::make_shared<TriangleDemoComponent>(*this, *cameraComponent);
	//m_components.push_back(triangleDemoComponent);

	auto cubeDemoComponent = std::make_shared<CubeDemoComponent>(*this, *cameraComponent, *m_keyboardComponent);
	m_components.push_back(cubeDemoComponent);

	Application::Initialize();

	cameraComponent->SetPosition(math::Vector3(0.0f, 0.0f, 50.0f));
}

void Game::Update(const library::Time& time)
{
	if (m_keyboardComponent->WasKeyPressed(library::Key::Escape))
	{
		Exit();
	}

	Application::Update(time);
}

void Game::Draw(const library::Time& time)
{
	m_d3dDeviceContext->ClearRenderTargetView(m_renderTargetView.Get(), k_backgroundColor);
	m_d3dDeviceContext->ClearDepthStencilView(
		m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	Application::Draw(time);

	// render mouse info
	m_spriteBatch->Begin();

	std::wostringstream woss;
	woss <<
		L"Mouse Position: " << m_mouseComponent->GetX() << ", " << m_mouseComponent->GetY() << std::endl <<
		L"Mouse Wheel: " << m_mouseComponent->GetWheel();

	m_spriteFont->DrawString(m_spriteBatch.get(), woss.str().c_str(), m_mouseTextPosition);

	m_spriteBatch->End();

	HRESULT hr = m_swapChain->Present(0, 0);
	if (FAILED(hr))
	{
		throw library::Exception("IDXGISwapChain::Present() failed.", hr);
	}
}

void Game::Shutdown()
{
	m_spriteFont.reset();
	m_spriteBatch.reset();
	m_directInput.Reset();

	Application::Shutdown();
}
