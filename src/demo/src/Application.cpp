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

#include "components/EnvironmentMappingEffectMaterialComponent.h"
#include "components/DisplacementMappingEffectMaterialComponent.h"
#include "components/NormalMappingEffectMaterialComponent.h"
#include "components/TransparencyMappingEffectMaterialComponent.h"
#include "components/FogEffectMaterialComponent.h"

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

		// create needed components

		// keyboard & mouse
		m_keyboard = std::make_shared<KeyboardComponent>(*this, m_directInput);
		m_mouse = std::make_shared<MouseComponent>(*this, m_directInput);

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

		// camera
		auto camera = std::make_shared<FirstPersonCameraComponent>(*this);
		camera->SetMouse(*m_mouse);
		camera->SetKeyboard(*m_keyboard);

		// fps
		auto fps = std::make_shared<FpsComponent>(*this);

		// skybox
		auto skybox = std::make_shared<SkyboxComponent>(*this, k_skyboxCubeMapPath, 100.f);
		skybox->SetCamera(*camera);

		// grid
		auto grid = std::make_shared<GridComponent>(*this);
		grid->SetCamera(*camera);

		// triangle
		auto triangle = std::make_shared<TriangleComponent>(*this);
		triangle->SetCamera(*camera);

		// cube
		auto cube = std::make_shared<CubeComponent>(*this);
		cube->SetCamera(*camera);
		cube->SetKeyboard(*m_keyboard);

		// model
		auto model = std::make_shared<ModelComponent>(*this);
		model->SetCamera(*camera);
		model->SetKeyboard(*m_keyboard);

		// texture with model
		auto textureModel = std::make_shared<TextureModelComponent>(*this);
		textureModel->SetCamera(*camera);
		textureModel->SetKeyboard(*m_keyboard);
		textureModel->SetMouse(*m_mouse);

		// basic
		auto basic = std::make_shared<BasicEffectMaterialComponent>(*this);
		basic->SetCamera(*camera);
		basic->SetKeyboard(*m_keyboard);

		// texture mapping
		auto textureMapping = std::make_shared<TextureMappingEffectMaterialComponent>(*this);
		textureMapping->SetCamera(*camera);
		textureMapping->SetKeyboard(*m_keyboard);

		// diffuse lighting
		auto diffuseLighting = std::make_shared<DiffuseLightingEffectMaterialComponent>(*this);
		diffuseLighting->SetCamera(*camera);
		diffuseLighting->SetKeyboard(*m_keyboard);

		// point light
		auto pointLight = std::make_shared<PointLightEffectMaterialComponent>(*this);
		pointLight->SetCamera(*camera);
		pointLight->SetKeyboard(*m_keyboard);

		// spotlight
		auto spotlight = std::make_shared<SpotlightEffectMaterialComponent>(*this);
		spotlight->SetCamera(*camera);
		spotlight->SetKeyboard(*m_keyboard);

		// normal mapping
		auto normalMapping = std::make_shared<NormalMappingEffectMaterialComponent>(*this);
		normalMapping->SetCamera(*camera);
		normalMapping->SetKeyboard(*m_keyboard);

		// environment mapping
		auto environmentMapping = std::make_shared<EnvironmentMappingEffectMaterialComponent>(*this);
		environmentMapping->SetCamera(*camera);
		environmentMapping->SetKeyboard(*m_keyboard);

		// transparency mapping
		auto transparencyMapping = std::make_shared<TransparencyMappingEffectMaterialComponent>(*this);
		transparencyMapping->SetCamera(*camera);
		transparencyMapping->SetKeyboard(*m_keyboard);

		// fog
		auto fog = std::make_shared<FogEffectMaterialComponent>(*this);
		fog->SetCamera(*camera);
		fog->SetKeyboard(*m_keyboard);

		// push needed components
		m_components.push_back(m_keyboard);
		m_components.push_back(m_mouse);
		m_components.push_back(mouseText);
		m_components.push_back(camera);
		m_components.push_back(fps);
		m_components.push_back(skybox);

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
