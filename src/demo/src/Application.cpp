#include "Application.h"

#include <library/components/FpsComponent.h>
#include <library/components/KeyboardComponent.h>
#include <library/components/MouseComponent.h>
#include <library/components/GridComponent.h>
#include <library/components/FirstPersonCameraComponent.h>
#include <library/components/SkyboxComponent.h>

#include "components/TriangleComponent.h"
#include "components/CubeComponent.h"
#include "components/ModelComponent.h"
#include "components/TextureModelComponent.h"

#include "components/BasicMaterialComponent.h"
#include "components/TextureMappingMaterialComponent.h"

#include <library/Path.h>
#include <library/Utils.h>
#include <library/Exception.h>

#include <sstream>
#include <iomanip>

namespace demo
{
	namespace
	{
		constexpr auto k_backgroundColor = library::colors::Black;

		const auto k_skyboxCubeMapPath = library::utils::GetExecutableDirectory().Join(library::fs::Path("../data/textures/Maskonaive2_1024.dds"));
	}

	Application::Application(
		const HINSTANCE instanceHandle,
		const std::wstring& windowClass,
		const std::wstring& windowTitle,
		const int showCmd
	)
		: library::Application(instanceHandle, windowClass, windowTitle, showCmd)
		, m_keyboardComponent()
		, m_mouseComponent()
	{
		m_depthStencilBufferEnabled = true;
		m_multiSamplingEnabled = true;
	}

	void Application::Initialize()
	{
		using namespace library;

		HRESULT hr = DirectInput8Create(
			m_instanceHandle,
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			reinterpret_cast<LPVOID*>(m_directInput.GetAddressOf()),
			nullptr
		);
		if (FAILED(hr))
		{
			throw Exception("DirectInput8Create() failed", hr);
		}

		// keyboard
		m_keyboardComponent = std::make_shared<KeyboardComponent>(*this, m_directInput);
		m_components.push_back(m_keyboardComponent);
		//m_services.AddService(m_keyboardComponent->GetTypeId(), m_keyboardComponent.get());

		// mouse
		m_mouseComponent = std::make_shared<MouseComponent>(*this, m_directInput);
		m_components.push_back(m_mouseComponent);
		//m_services.AddService(m_mouseComponent->GetTypeId(), m_mouseComponent.get());

		// camera
		auto cameraComponent = std::make_shared<FirstPersonCameraComponent>(*this);
		cameraComponent->SetMouse(*m_mouseComponent);
		cameraComponent->SetKeyboard(*m_keyboardComponent);
		m_components.push_back(cameraComponent);
		//m_services.AddService(cameraComponent->GetTypeId(), cameraComponent.get());

		// fps
		auto fpsComponent = std::make_shared<FpsComponent>(*this);
		m_components.push_back(fpsComponent);

		// grid
		auto gridComponent = std::make_shared<GridComponent>(*this);
		gridComponent->SetCamera(*cameraComponent);
		m_components.push_back(gridComponent);

		// triangle
		//auto triangleComponent = std::make_shared<TriangleComponent>(*this, *cameraComponent);
		//m_components.push_back(triangleComponent);

		// cube
		//auto cubeComponent = std::make_shared<CubeComponent>(*this, *cameraComponent, *m_keyboardComponent);
		//m_components.push_back(cubeComponent);

		// model
		//auto modelComponent = std::make_shared<ModelComponent>(*this, *cameraComponent, *m_keyboardComponent);
		//m_components.push_back(modelComponent);

		// texture with model
		//auto textureModelComponent = std::make_shared<TextureModelComponent>(*this, *cameraComponent, *m_keyboardComponent, * m_mouseComponent);
		//m_components.push_back(textureModelComponent);

		// basic material
		//auto basicMaterialComponent = std::make_shared<BasicMaterialComponent>(*this, *cameraComponent, *m_keyboardComponent);
		//m_components.push_back(basicMaterialComponent);

		// texture mapping material
		auto textureMappingMaterialComponent = std::make_shared<TextureMappingMaterialComponent>(*this);
		textureMappingMaterialComponent->SetCamera(*cameraComponent);
		textureMappingMaterialComponent->SetKeyboard(*m_keyboardComponent);
		m_components.push_back(textureMappingMaterialComponent);

		// skybox
		auto skyboxComponent = std::make_shared<SkyboxComponent>(*this, k_skyboxCubeMapPath, 100.f);
		skyboxComponent->SetCamera(*cameraComponent);
		m_components.push_back(skyboxComponent);

		// mouse text component
		auto mouseTextComponent = std::make_shared<TextComponent>(*this);
		mouseTextComponent->SetGeneratorFunction(
			[this]() -> std::wstring
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
		mouseTextComponent->SetPosition(library::math::Vector2(0.f, 50.f));
		m_components.push_back(mouseTextComponent);

		library::Application::Initialize();

		cameraComponent->SetPosition(library::math::Vector3(0.0f, 0.0f, 50.0f));
	}

	void Application::Update(const library::Time& time)
	{
		if (m_keyboardComponent->WasKeyPressed(library::Key::Escape))
		{
			Exit();
		}

		library::Application::Update(time);
	}

	void Application::Draw(const library::Time& time)
	{
		m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), static_cast<const float*>(k_backgroundColor));
		m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		library::Application::Draw(time);

		HRESULT hr = m_swapChain->Present(0, 0);
		if (FAILED(hr))
		{
			throw library::Exception("IDXGISwapChain::Present() failed.", hr);
		}
	}

	void Application::Shutdown()
	{
		m_directInput.Reset();

		library::Application::Shutdown();
	}
} // namespace demo
