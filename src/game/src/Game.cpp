#include "Game.h"

#include <library/components/FpsComponent.h>
#include <library/components/KeyboardComponent.h>
#include <library/components/MouseComponent.h>
#include <library/components/GridComponent.h>
#include <library/components/FirstPersonCameraComponent.h>

#include "TriangleDemoComponent.h"
#include "CubeDemoComponent.h"

#include <library/Exception.h>

#include <sstream>
#include <iomanip>

namespace
{
	constexpr auto k_backgroundColor = library::colors::Black;
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
	
	// mouse text component
	auto mouseTextComponent = std::make_shared<TextComponent>(*this);
	mouseTextComponent->SetGeneratorFunction([this]() -> std::wstring
		{
			static const std::wstring empty;

			if (!!m_mouseComponent)
			{
				std::wostringstream woss;
				woss <<
					L"Mouse Position: " << m_mouseComponent->GetX() << ", " << m_mouseComponent->GetY() << std::endl <<
					L"Mouse Wheel: " << m_mouseComponent->GetWheel();
				return woss.str();
			}

			return empty;
		}
	);
	mouseTextComponent->SetPosition(math::Vector2(0.f, 50.f));
	m_components.push_back(mouseTextComponent);

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
	m_d3dDeviceContext->ClearRenderTargetView(m_renderTargetView.Get(), static_cast<const float*>(k_backgroundColor));
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
	m_directInput.Reset();

	Application::Shutdown();
}
