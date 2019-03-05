#include "DemoApplication.h"

#include "TriangleDemo.h"
#include "CubeDemo.h"
#include "ModelDemo.h"
#include "TextureModelDemo.h"

#include "DiffuseLightingDemo.h"
#include "PointLightDemo.h"
#include "SpotlightDemo.h"

#include "BasicMaterialDemo.h"
#include "TextureMappingDemo.h"

#include "EnvironmentMappingDemo.h"
#include "DisplacementMappingDemo.h"
#include "NormalMappingDemo.h"
#include "TransparencyMappingDemo.h"
#include "FogDemo.h"

#include "ColorFilterDemo.h"
#include "GaussianBlurDemo.h"
#include "BloomDemo.h"
#include "DistortionMappingDemo.h"

#include "ProjectiveTextureMappingDemo.h"
#include "ProjectiveTextureMappingDepthMapDemo.h"
#include "ShadowMappingDemo.h"

//-------------------------------------------------------------------------

#include <library/Components/FpsComponent.h>
#include <library/Components/KeyboardComponent.h>
#include <library/Components/MouseComponent.h>
#include <library/Components/GridComponent.h>
#include <library/Components/FirstPersonCameraComponent.h>
#include <library/Components/SkyboxComponent.h>
#include <library/Components/FullScreenQuadComponent.h>

#include <library/RenderTargets/FullScreenRenderTarget.h>

#include <library/Effect/Effect.h>
#include <library/Effect/EffectVariable.h>
#include <library/Effect/EffectPass.h>
#include <library/Effect/EffectTechnique.h>

#include <library/Path.h>
#include <library/Utils.h>
#include <library/Exception.h>

//-------------------------------------------------------------------------

#include <sstream>
#include <iomanip>

using namespace library;

namespace
{
	const auto k_backgroundColor = Color::Black;
}

DemoApplication::DemoApplication(
	const HINSTANCE instanceHandle,
	const std::wstring& windowClass,
	const std::wstring& windowTitle,
	const int showCmd
)
	: Application(instanceHandle, windowClass, windowTitle, showCmd)
	, m_postProcessingEnabled(false)
{
	m_depthStencilBufferEnabled = true;
	m_multiSamplingEnabled = true;
}

DemoApplication::~DemoApplication() = default;

//-------------------------------------------------------------------------

const Path& DemoApplication::GetDataPath() const
{
	static const auto s_dataPath = utils::GetExecutableDirectory() + Path("../data");
	return s_dataPath;
}

void DemoApplication::Initialize()
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
	// don't initialize them before Application::Initialize

	// keyboard & mouse
	m_keyboard = std::make_shared<KeyboardComponent>(m_directInput);
	m_mouse = std::make_shared<MouseComponent>(m_directInput);

	// mouse text component
	auto mouseText = std::make_shared<TextComponent>();
	mouseText->SetTextUpdateFunction(
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
	auto camera = std::make_shared<FirstPersonCameraComponent>();
	camera->SetMouse(*m_mouse);
	camera->SetKeyboard(*m_keyboard);
	camera->SetPosition(math::Vector3(0.0f, 0.0f, 50.0f));

	// fps
	auto fps = std::make_shared<FpsComponent>();

	// skybox
	auto skybox = std::make_shared<SkyboxComponent>("Maskonaive2_1024", 100.f);
	skybox->SetCamera(*camera);

	// grid
	m_grid = std::make_shared<GridComponent>();
	m_grid->SetCamera(*camera);

	// triangle
	auto triangle = std::make_shared<TriangleDemo>();
	triangle->SetCamera(*camera);

	// cube
	auto cube = std::make_shared<CubeDemo>();
	cube->SetCamera(*camera);
	cube->SetKeyboard(*m_keyboard);

	// model
	auto model = std::make_shared<ModelDemo>();
	model->SetCamera(*camera);
	model->SetKeyboard(*m_keyboard);

	// texture with model
	auto textureModel = std::make_shared<TextureModelDemo>();
	textureModel->SetCamera(*camera);
	textureModel->SetKeyboard(*m_keyboard);
	textureModel->SetMouse(*m_mouse);

	// basic
	auto basic = std::make_shared<BasicMaterialDemo>();
	basic->SetCamera(*camera);
	basic->SetKeyboard(*m_keyboard);

	// texture mapping
	auto textureMapping = std::make_shared<TextureMappingDemo>();
	textureMapping->SetCamera(*camera);
	textureMapping->SetKeyboard(*m_keyboard);

	// diffuse lighting
	auto diffuseLighting = std::make_shared<DiffuseLightingDemo>();
	diffuseLighting->SetCamera(*camera);
	diffuseLighting->SetKeyboard(*m_keyboard);

	// point light
	auto pointLight = std::make_shared<PointLightDemo>();
	pointLight->SetCamera(*camera);
	pointLight->SetKeyboard(*m_keyboard);

	// spotlight
	auto spotlight = std::make_shared<SpotlightDemo>();
	spotlight->SetCamera(*camera);
	spotlight->SetKeyboard(*m_keyboard);

	// normal mapping
	auto normalMapping = std::make_shared<NormalMappingDemo>();
	normalMapping->SetCamera(*camera);
	normalMapping->SetKeyboard(*m_keyboard);

	// environment mapping
	auto environmentMapping = std::make_shared<EnvironmentMappingDemo>();
	environmentMapping->SetCamera(*camera);
	environmentMapping->SetKeyboard(*m_keyboard);

	// transparency mapping
	auto transparencyMapping = std::make_shared<TransparencyMappingDemo>();
	transparencyMapping->SetCamera(*camera);
	transparencyMapping->SetKeyboard(*m_keyboard);

	// displacement mapping
	auto displacementMapping = std::make_shared<DisplacementMappingDemo>();
	displacementMapping->SetCamera(*camera);
	displacementMapping->SetKeyboard(*m_keyboard);

	// fog
	auto fog = std::make_shared<FogDemo>();
	fog->SetCamera(*camera);
	fog->SetKeyboard(*m_keyboard);

	// render target
	m_sceneRenderTarget = std::make_unique<FullScreenRenderTarget>(*this);

	// post-processing
	{
		//auto postProcessing = new ColorFilter();
		//auto postProcessing = new GaussianBlurDemo();
		//auto postProcessing = new BloomDemo();
		auto postProcessing = new DistortionMappingDemo();
		postProcessing->SetKeyboard(*m_keyboard);
		postProcessing->SetCamera(*camera);
		postProcessing->SetSceneTexture(*(m_sceneRenderTarget->GetOutputTexture()));

		m_postProcessing = std::unique_ptr<PostProcessingComponent>(postProcessing);
	}

	// post-processing text component
	auto postProcessingText = std::make_shared<TextComponent>();
	postProcessingText->SetTextUpdateFunction(
		[this]() -> std::wstring
		{
			std::wostringstream woss;
			woss << L"Post-Processing: " << (m_postProcessingEnabled ? L"Enabled" : L"Disabled");
			return woss.str();
		}
	);
	postProcessingText->SetPosition(math::Vector2(0.f, 70.f));

	auto projectiveTextureMapping = std::make_shared<ProjectiveTextureMappingDemo>();
	projectiveTextureMapping->SetCamera(*camera);
	projectiveTextureMapping->SetKeyboard(*m_keyboard);

	auto projectiveTextureDepthMapMapping = std::make_shared<ProjectiveTextureMappingDepthMapDemo>();
	projectiveTextureDepthMapMapping->SetCamera(*camera);
	projectiveTextureDepthMapMapping->SetKeyboard(*m_keyboard);

	auto shadowMapping = std::make_shared<ShadowMappingDemo>();
	shadowMapping->SetCamera(*camera);
	shadowMapping->SetKeyboard(*m_keyboard);

	// push needed components
	m_components.push_back(m_keyboard);
	m_components.push_back(m_mouse);
	m_components.push_back(camera);
	m_components.push_back(m_grid);
	m_components.push_back(fps);
	//m_components.push_back(skybox);
	m_components.push_back(postProcessingText);
	m_components.push_back(shadowMapping);

	Application::Initialize();

	m_postProcessing->Initialize(*this);
}

void DemoApplication::Update(const Time& time)
{
	if (!!m_keyboard)
	{
		if (m_keyboard->WasKeyPressed(Key::Escape))
			Exit();

		if (m_keyboard->WasKeyPressed(Key::G))
			m_grid->SetVisible(!m_grid->IsVisible());

		if (m_keyboard->WasKeyPressed(Key::Tab))
			m_postProcessingEnabled = !m_postProcessingEnabled;
	}

	if (m_postProcessingEnabled)
		m_postProcessing->Update(time);

	Application::Update(time);
}

void DemoApplication::Draw(const Time& time)
{
	if (m_postProcessingEnabled)
	{
		// Render the scene to an off-screen texture
		m_sceneRenderTarget->Begin();

		m_deviceContext->ClearRenderTargetView(
			m_sceneRenderTarget->GetRenderTargetView(),
			static_cast<const float*>(k_backgroundColor)
		);
		m_deviceContext->ClearDepthStencilView(
			m_sceneRenderTarget->GetDepthStencilView(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.0f, 0
		);

		m_renderer->RenderScene(time);

		m_sceneRenderTarget->End();

		// Render a full-screen quad with a post-processing effect
		m_deviceContext->ClearRenderTargetView(
			m_renderTargetView.Get(),
			static_cast<const float*>(k_backgroundColor)
		);
		m_deviceContext->ClearDepthStencilView(
			m_depthStencilView.Get(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.0f, 0
		);

		m_postProcessing->Draw(time);

		m_renderer->RenderUI(time);
	}
	else
	{
		m_deviceContext->ClearRenderTargetView(
			m_renderTargetView.Get(),
			static_cast<const float*>(k_backgroundColor)
		);
		m_deviceContext->ClearDepthStencilView(
			m_depthStencilView.Get(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.0f, 0
		);

		Application::Draw(time);
	}

	HRESULT hr = m_swapChain->Present(0, 0);
	if (FAILED(hr))
	{
		throw Exception("IDXGISwapChain::Present() failed.", hr);
	}
}

void DemoApplication::Shutdown()
{
	m_directInput.Reset();

	Application::Shutdown();
}
