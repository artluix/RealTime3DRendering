#include "Application.h"

#include "components/TriangleComponent.h"
#include "components/CubeComponent.h"
#include "components/ModelComponent.h"
#include "components/TextureModelComponent.h"

#include "components/DiffuseLightingComponent.h"
#include "components/PointLightComponent.h"
#include "components/SpotlightComponent.h"

#include "components/BasicComponent.h"
#include "components/TextureMappingComponent.h"

#include "components/EnvironmentMappingComponent.h"
#include "components/DisplacementMappingComponent.h"
#include "components/NormalMappingComponent.h"
#include "components/TransparencyMappingComponent.h"
#include "components/FogComponent.h"

#include <library/components/FpsComponent.h>
#include <library/components/KeyboardComponent.h>
#include <library/components/MouseComponent.h>
#include <library/components/GridComponent.h>
#include <library/components/FirstPersonCameraComponent.h>
#include <library/components/SkyboxComponent.h>

#include <library/components/FullScreenQuadComponent.h>
#include <library/FullScreenRenderTarget.h>

#include <library/components/GaussianBlurComponent.h>

#include <library/effect/Effect.h>
#include <library/effect/EffectVariable.h>
#include <library/effect/EffectPass.h>
#include <library/effect/EffectTechnique.h>

#include <library/Path.h>
#include <library/Utils.h>
#include <library/Exception.h>

#include <sstream>
#include <iomanip>

namespace demo
{
	namespace
	{
		constexpr float k_brightnessModulationRate = 3.0f;
		constexpr float k_blurModulationRate = 1.0f;

		const auto k_backgroundColor = library::Color::Black;

		const auto k_skyboxCubeMapPath = library::utils::GetExecutableDirectory().Join(library::Path("../data/textures/Maskonaive2_1024.dds"));
		const auto k_colorFilterEffectPath = library::utils::GetExecutableDirectory().Join(
#if defined(DEBUG) || defined(DEBUG)
			library::Path("../data/effects/ColorFilter_d.fxc")
#else
			library::Path("../data/effects/ColorFilter.fxc")
#endif
		);
	}

	Application::Application(
		const HINSTANCE instanceHandle,
		const std::wstring& windowClass,
		const std::wstring& windowTitle,
		const int showCmd
	)
		: library::Application(instanceHandle, windowClass, windowTitle, showCmd)
		, m_colorFilterType(library::ColorFilter::Original)
		, m_genericColorFilter(library::math::Matrix4::Identity)
	{
		m_depthStencilBufferEnabled = true;
		m_multiSamplingEnabled = true;
	}

	Application::~Application() = default;

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
						L"Mouse Position: " << m_mouse->GetX() << ", " << m_mouse->GetY() << "\n" <<
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
		auto basic = std::make_shared<BasicComponent>(*this);
		basic->SetCamera(*camera);
		basic->SetKeyboard(*m_keyboard);

		// texture mapping
		auto textureMapping = std::make_shared<TextureMappingComponent>(*this);
		textureMapping->SetCamera(*camera);
		textureMapping->SetKeyboard(*m_keyboard);

		// diffuse lighting
		auto diffuseLighting = std::make_shared<DiffuseLightingComponent>(*this);
		diffuseLighting->SetCamera(*camera);
		diffuseLighting->SetKeyboard(*m_keyboard);

		// point light
		auto pointLight = std::make_shared<PointLightComponent>(*this);
		pointLight->SetCamera(*camera);
		pointLight->SetKeyboard(*m_keyboard);

		// spotlight
		auto spotlight = std::make_shared<SpotlightComponent>(*this);
		spotlight->SetCamera(*camera);
		spotlight->SetKeyboard(*m_keyboard);

		// normal mapping
		auto normalMapping = std::make_shared<NormalMappingComponent>(*this);
		normalMapping->SetCamera(*camera);
		normalMapping->SetKeyboard(*m_keyboard);

		// environment mapping
		auto environmentMapping = std::make_shared<EnvironmentMappingComponent>(*this);
		environmentMapping->SetCamera(*camera);
		environmentMapping->SetKeyboard(*m_keyboard);

		// transparency mapping
		auto transparencyMapping = std::make_shared<TransparencyMappingComponent>(*this);
		transparencyMapping->SetCamera(*camera);
		transparencyMapping->SetKeyboard(*m_keyboard);

		// displacement mapping
		auto displacementMapping = std::make_shared<DisplacementMappingComponent>(*this);
		displacementMapping->SetCamera(*camera);
		displacementMapping->SetKeyboard(*m_keyboard);

		// fog
		auto fog = std::make_shared<FogComponent>(*this);
		fog->SetCamera(*camera);
		fog->SetKeyboard(*m_keyboard);

		// color filter
		{
			m_renderTarget = std::make_unique<FullScreenRenderTarget>(*this);

			m_colorFilterEffect = Effect::Create(*this, k_colorFilterEffectPath);
			m_colorFilterEffect->LoadCompiled();

			m_colorFilterMaterial = std::make_unique<ColorFilterMaterial>(*m_colorFilterEffect);
			m_colorFilterMaterial->Initialize();

			m_fullScreenQuad = std::make_unique<FullScreenQuadComponent>(*this);
			m_fullScreenQuad->SetMaterial(*m_colorFilterMaterial, "grayscale_filter", "p0");
			m_fullScreenQuad->Initialize();
			m_fullScreenQuad->SetMaterialUpdateFunction(
				[this]()
				{
					m_colorFilterMaterial->GetColorTexture() << m_renderTarget->GetOutputTexture();
					m_colorFilterMaterial->GetColorFilter() << m_genericColorFilter;
				}
			);
		}

		// gaussian blur
		m_gaussianBlur = std::make_unique<GaussianBlurComponent>(*this);
		m_gaussianBlur->SetSceneTexture(*(m_renderTarget->GetOutputTexture()));
		m_gaussianBlur->Initialize();

		// text info component
		auto textInfo = std::make_shared<TextComponent>(*this);
		textInfo->SetTextGeneratorFunction(
			[this]() -> std::wstring
			{
				std::ostringstream oss;
				oss << "Color Filter (Space Bar): " << ColorFilter::ToString(m_colorFilterType) << "\n";

				if (m_colorFilterType == library::ColorFilter::Generic)
					oss << "Brightness (+Comma/-Period): " << m_genericColorFilter._11;
				else if (m_colorFilterType == library::ColorFilter::Original)
					oss << std::setprecision(2) << "Blur Amount (+J/-K): " << m_gaussianBlur->GetBlurAmount();

				return utils::ToWideString(oss.str());
			}
		);
		textInfo->SetPosition(math::Vector2(0.f, 45.f));

		// push needed components
		m_components.push_back(m_keyboard);
		m_components.push_back(m_mouse);
		m_components.push_back(camera);
		m_components.push_back(grid);
		m_components.push_back(fps);
		m_components.push_back(skybox);
		m_components.push_back(pointLight);
		m_components.push_back(textInfo);

		library::Application::Initialize();

		camera->SetPosition(math::Vector3(0.0f, 0.0f, 50.0f));
	}

	void Application::Update(const library::Time& time)
	{
		if (!!m_keyboard)
		{
			if (m_keyboard->WasKeyPressed(library::Key::Escape))
			{
				Exit();
			}

			if (m_keyboard->WasKeyPressed(library::Key::Space))
			{
				using library::ColorFilter;

				m_colorFilterType = static_cast<ColorFilter::Type>(m_colorFilterType + 1);

				if (m_colorFilterType == ColorFilter::Count)
				{
					m_colorFilterType = library::ColorFilter::Original;
				}
				else
				{
					m_fullScreenQuad->SetActiveTechnique(ColorFilter::ToTechniqueName(m_colorFilterType), "p0");
				}
			}
		}

		UpdateGenericColorFilter(time);
		UpdateBlurAmount(time);

		library::Application::Update(time);
	}

	void Application::Draw(const library::Time& time)
	{
		// Render the scene to an off-screen texture
		m_renderTarget->Begin();

		m_deviceContext->ClearRenderTargetView(m_renderTarget->GetRenderTargetView(), static_cast<const float*>(k_backgroundColor));
		m_deviceContext->ClearDepthStencilView(m_renderTarget->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		library::Application::Draw(time);

		m_renderTarget->End();

		// Render a full-screen quad with a post-processing effect
		m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), static_cast<const float*>(k_backgroundColor));
		m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// apply gaussian blur only to original image
		if (m_colorFilterType == library::ColorFilter::Original)
			m_gaussianBlur->Draw(time);
		else
			m_fullScreenQuad->Draw(time);

		UnbindPixelShaderResources(0, 1);

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

	void Application::UpdateGenericColorFilter(const library::Time& time)
	{
		static float brightness = 1.0f;

		if (!!m_keyboard)
		{
			using namespace library;

			const auto elapsedTime = time.elapsed.GetSeconds();

			if (m_keyboard->IsKeyDown(Key::Comma) && brightness < 1.f)
			{
				brightness += k_brightnessModulationRate * elapsedTime;
				brightness = math::Min(brightness, 1.f);
				m_genericColorFilter = math::Matrix4::Scaling(math::Vector3(brightness));
			}

			if (m_keyboard->IsKeyDown(Key::Period) && brightness > 0.f)
			{
				brightness -= k_brightnessModulationRate * elapsedTime;
				brightness = math::Max(brightness, 0.f);
				m_genericColorFilter = math::Matrix4::Scaling(math::Vector3(brightness));
			}
		}
	}

	void Application::UpdateBlurAmount(const library::Time& time)
	{
		static float blurAmount = m_gaussianBlur->GetBlurAmount();

		if (!!m_keyboard)
		{
			using namespace library;

			const auto elapsedTime = time.elapsed.GetSeconds();

			if (m_keyboard->IsKeyDown(Key::J))
			{
				blurAmount += k_blurModulationRate * elapsedTime;
				m_gaussianBlur->SetBlurAmount(blurAmount);
			}

			if (m_keyboard->IsKeyDown(Key::K) && blurAmount > 0.f)
			{
				blurAmount -= k_blurModulationRate * elapsedTime;
				blurAmount = math::Max(1e-10f, blurAmount);

				m_gaussianBlur->SetBlurAmount(blurAmount);
			}
		}
	}
} // namespace demo
