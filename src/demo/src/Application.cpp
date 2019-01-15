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

#include "components/DiffuseLightingEffectMaterialComponent.h"
#include "components/PointLightEffectMaterialComponent.h"
#include "components/SpotlightEffectMaterialComponent.h"

#include "components/BasicEffectMaterialComponent.h"
#include "components/TextureMappingEffectMaterialComponent.h"

#include "components/NormalMappingEffectMaterialComponent.h"

#include <library/Path.h>
#include <library/Utils.h>
#include <library/Exception.h>

#include <sstream>

namespace demo
{
	namespace
	{
		const auto k_backgroundColor = library::Color::Black;

		const auto k_skyboxCubeMapPath = library::utils::GetExecutableDirectory().Join(library::Path("../data/textures/Maskonaive2_1024.dds"));
	}

	Application::Application(
		const HINSTANCE instanceHandle,
		const std::wstring& windowClass,
		const std::wstring& windowTitle,
		const int showCmd
	)
		: library::Application(instanceHandle, windowClass, windowTitle, showCmd)
		, m_keyboard()
		, m_mouse()
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
		m_keyboard = std::make_shared<KeyboardComponent>(*this, m_directInput);
		m_components.push_back(m_keyboard);
		//m_services.AddService(m_keyboard->GetTypeId(), m_keyboard.get());

		// mouse
		m_mouse = std::make_shared<MouseComponent>(*this, m_directInput);
		m_components.push_back(m_mouse);
		//m_services.AddService(m_mouseComponent->GetTypeId(), m_mouseComponent.get());

		// camera
		auto camera = std::make_shared<FirstPersonCameraComponent>(*this);
		camera->SetMouse(*m_mouse);
		camera->SetKeyboard(*m_keyboard);
		m_components.push_back(camera);
		//m_services.AddService(cameraComponent->GetTypeId(), cameraComponent.get());

		// fps
		auto fps = std::make_shared<FpsComponent>(*this);
		m_components.push_back(fps);

		// grid
		auto grid = std::make_shared<GridComponent>(*this);
		grid->SetCamera(*camera);
		//m_components.push_back(grid);

		// triangle
		auto triangle = std::make_shared<TriangleComponent>(*this);
		triangle->SetCamera(*camera);
		//m_components.push_back(triangleComponent);

		// cube
		auto cube = std::make_shared<CubeComponent>(*this);
		cube->SetCamera(*camera);
		cube->SetKeyboard(*m_keyboard);
		//m_components.push_back(cubeComponent);

		// model
		auto model = std::make_shared<ModelComponent>(*this);
		model->SetCamera(*camera);
		model->SetKeyboard(*m_keyboard);
		//m_components.push_back(modelComponent);

		// texture with model
		auto textureModel = std::make_shared<TextureModelComponent>(*this);
		textureModel->SetCamera(*camera);
		textureModel->SetKeyboard(*m_keyboard);
		textureModel->SetMouse(*m_mouse);
		//m_components.push_back(textureModelComponent);

		// basic
		auto basic = std::make_shared<BasicEffectMaterialComponent>(*this);
		basic->SetCamera(*camera);
		basic->SetKeyboard(*m_keyboard);
		//m_components.push_back(basicMaterialComponent);

		// texture mapping
		auto textureMapping = std::make_shared<TextureMappingEffectMaterialComponent>(*this);
		textureMapping->SetCamera(*camera);
		textureMapping->SetKeyboard(*m_keyboard);
		//m_components.push_back(textureMapping);

		// diffuse lighting
		auto diffuseLighting = std::make_shared<DiffuseLightingEffectMaterialComponent>(*this);
		diffuseLighting->SetCamera(*camera);
		diffuseLighting->SetKeyboard(*m_keyboard);
		//m_components.push_back(diffuseLighting);

		// point light
		auto pointLight = std::make_shared<PointLightEffectMaterialComponent>(*this);
		pointLight->SetCamera(*camera);
		pointLight->SetKeyboard(*m_keyboard);
		//m_components.push_back(pointLight);

		// spotlight
		auto spotlight = std::make_shared<SpotlightEffectMaterialComponent>(*this);
		spotlight->SetCamera(*camera);
		spotlight->SetKeyboard(*m_keyboard);
		//m_components.push_back(spotlight);

		// spotlight
		auto normalMapping = std::make_shared<NormalMappingEffectMaterialComponent>(*this);
		normalMapping->SetCamera(*camera);
		normalMapping->SetKeyboard(*m_keyboard);
		m_components.push_back(normalMapping);

		// skybox
		auto skybox = std::make_shared<SkyboxComponent>(*this, k_skyboxCubeMapPath, 100.f);
		skybox->SetCamera(*camera);
		//m_components.push_back(skybox);

		// mouse text component
		auto mouseText = std::make_shared<TextComponent>(*this);
		mouseText->SetTextGeneratorFunction(
			[this]() -> std::wstring
			{
				static const std::wstring empty;

				if (!!m_mouse)
				{
					std::wostringstream woss;
					woss <<
						L"Mouse Position: " << m_mouse->GetX() << ", " << m_mouse->GetY() << std::endl <<
						L"Mouse Wheel: " << m_mouse->GetWheel();
					return woss.str();
				}

				return empty;
			}
		);
		mouseText->SetPosition(math::Vector2(0.f, 50.f));
		m_components.push_back(mouseText);

		library::Application::Initialize();

		camera->SetPosition(math::Vector3(0.0f, 0.0f, 50.0f));
	}

	void Application::Update(const library::Time& time)
	{
		if (m_keyboard->WasKeyPressed(library::Key::Escape))
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
