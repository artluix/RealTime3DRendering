#include "DemoApplication.h"

#include "Components/TriangleDemo.h"
#include "Components/CubeDemo.h"
#include "Components/ModelDemo.h"
#include "Components/TextureModelDemo.h"

#include "Components/BasicMaterialDemo.h"
#include "Components/TextureMappingDemo.h"

#include "Components/DiffuseLightingDemo.h"
#include "Components/PointLightDemo.h"
#include "Components/MultiplePointLightsDemo.h"
#include "Components/SpotlightDemo.h"

#include "Components/NormalMappingDemo.h"
#include "Components/EnvironmentMappingDemo.h"
#include "Components/TransparencyMappingDemo.h"
#include "Components/DisplacementMappingDemo.h"

#include "Components/FogDemo.h"

#include "Components/ColorFilterDemo.h"
#include "Components/GaussianBlurDemo.h"
#include "Components/BloomDemo.h"
#include "Components/DistortionMappingDemo.h"

#include "Components/ProjectiveTextureMappingDemo.h"
#include "Components/ProjectiveTextureMappingDepthMapDemo.h"
#include "Components/ShadowMappingDemo.h"
#include "Components/DirectionalShadowMappingDemo.h"

#include "Components/AnimationDemo.h"

#include "Components/GeometryShaderDemo.h"
#include "Components/BasicTessellationDemo.h"
#include "Components/QuadHeightMapTessellationDemo.h"
#include "Components/ModelTessellationDemo.h"

#include "Components/InstancingPointLightDemo.h"

#include "Components/ComputeShaderDemo.h"

//-------------------------------------------------------------------------

#include <library/Components/FpsComponent.h>
#include <library/Components/KeyboardComponent.h>
#include <library/Components/MouseComponent.h>
#include <library/Components/GridComponent.h>
#include <library/Components/FirstPersonCameraComponent.h>
#include <library/Components/SkyboxComponent.h>
#include <library/Components/FullScreenQuadComponent.h>

#include <library/RenderTargets/FullScreenRenderTarget.h>
#include <library/RenderTargets/MultipleRenderTarget.h>

//-------------------------------------------------------------------------

#include <library/Effect/Effect.h>
#include <library/Effect/EffectVariable.h>
#include <library/Effect/EffectPass.h>
#include <library/Effect/EffectTechnique.h>

#include <library/Path.h>
#include <library/Utils.h>

//-------------------------------------------------------------------------

#include <sstream>
#include <iomanip>

using namespace library;

namespace
{
constexpr auto k_mrtBackgroundColor = colors::Color();
constexpr auto k_backgroundColor = colors::CornFlower;
} // namespace

DemoApplication::DemoApplication(
	const HINSTANCE instanceHandle,
	const std::wstring& windowClass,
	const std::wstring& windowTitle,
	const int showCmd
)
	: Application(instanceHandle, windowClass, windowTitle, showCmd)
	, m_postProcessingEnabled(false)
	, m_deferredLightingEnabled(false)
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
		reinterpret_cast<LPVOID*>(&m_directInput),
		nullptr
	);
	assert("DirectInput8Create() failed" && SUCCEEDED(hr));

	// create needed components
	// don't initialize them before Application::Initialize

	// keyboard & mouse
	m_keyboard = std::make_shared<KeyboardComponent>(*m_directInput);
	m_mouse = std::make_shared<MouseComponent>(*m_directInput);

	// mouse text component
	auto mouseText = std::make_shared<TextComponent>();
	mouseText->SetTextUpdateFunction([this]() -> std::wstring {
		static const std::wstring empty;

		if (!!m_mouse)
		{
			std::wostringstream woss;
			woss << L"Mouse Position: " << m_mouse->GetX() << ", " << m_mouse->GetY() << "\n"
				 << L"Mouse Wheel: " << m_mouse->GetWheel();
			return woss.str();
		}

		return empty;
	});
	mouseText->SetPosition(math::Vector2(0.f, 50.f));

	// camera
	auto camera = std::make_shared<FirstPersonCameraComponent>();
	camera->SetMouse(*m_mouse);
	camera->SetKeyboard(*m_keyboard);
	camera->SetPosition(math::Vector3(0.0f, 0.0f, 30.0f));

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

	// multiple point lights
	//auto multiplePointLights = std::make_shared<MultiplePointLightsDemo>();
	//multiplePointLights->SetCamera(*camera);
	//multiplePointLights->SetKeyboard(*m_keyboard);

	m_multiplePointLights = std::make_shared<MultiplePointLightsDemo>();
	m_multiplePointLights->SetCamera(*camera);
	m_multiplePointLights->SetKeyboard(*m_keyboard);

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

	// post-processing
	{
		m_sceneRenderTarget = std::make_unique<FullScreenRenderTarget>(*this);

		// auto postProcessing = new ColorFilterDemo();
		auto postProcessing = new GaussianBlurDemo();
		// auto postProcessing = new BloomDemo();
		// auto postProcessing = new DistortionMappingDemo();
		postProcessing->SetKeyboard(*m_keyboard);
		// postProcessing->SetCamera(*camera);
		postProcessing->SetSceneTexture(*(m_sceneRenderTarget->GetOutputTexture()));

		m_postProcessing = std::unique_ptr<PostProcessingComponent>(postProcessing);

		auto postProcessingText = std::make_shared<TextComponent>();
		postProcessingText->SetTextUpdateFunction([this]() -> std::wstring {
			std::wostringstream woss;
			woss << L"Post-Processing: " << (m_postProcessingEnabled ? L"Enabled" : L"Disabled");
			return woss.str();
		});
		postProcessingText->SetPosition(math::Vector2(0.f, 45.f));

		m_components.push_back(postProcessingText);
	}

	// deferred lighting
	{
		m_multipleRenderTarget = std::make_unique<MultipleRenderTarget>(*this, 3);

		auto deferredLightingText = std::make_shared<TextComponent>();
		deferredLightingText->SetTextUpdateFunction([this]() -> std::wstring {
			std::wostringstream woss;
			woss << L"Deferred lighting: " << (m_deferredLightingEnabled ? L"Enabled" : L"Disabled");
			return woss.str();
		});
		deferredLightingText->SetPosition(math::Vector2(0.f, 210.f));

		m_components.push_back(deferredLightingText);
	}

	// projection
	auto projectiveTextureMapping = std::make_shared<ProjectiveTextureMappingDemo>();
	projectiveTextureMapping->SetCamera(*camera);
	projectiveTextureMapping->SetKeyboard(*m_keyboard);

	auto projectiveTextureDepthMapMapping = std::make_shared<ProjectiveTextureMappingDepthMapDemo>();
	projectiveTextureDepthMapMapping->SetCamera(*camera);
	projectiveTextureDepthMapMapping->SetKeyboard(*m_keyboard);

	auto shadowMapping = std::make_shared<ShadowMappingDemo>();
	shadowMapping->SetCamera(*camera);
	shadowMapping->SetKeyboard(*m_keyboard);

	auto directionalShadowMapping = std::make_shared<DirectionalShadowMappingDemo>();
	directionalShadowMapping->SetCamera(*camera);
	directionalShadowMapping->SetKeyboard(*m_keyboard);

	// animation
	auto animation = std::make_shared<AnimationDemo>();
	animation->SetCamera(*camera);
	animation->SetKeyboard(*m_keyboard);

	// geometry shader
	auto geometryShader = std::make_shared<GeometryShaderDemo>();
	geometryShader->SetCamera(*camera);
	geometryShader->SetKeyboard(*m_keyboard);

	// tessellation
	auto basicTessellation = std::make_shared<BasicTessellationDemo>();
	basicTessellation->SetCamera(*camera);
	basicTessellation->SetKeyboard(*m_keyboard);

	auto quadHeightMapTessellation = std::make_shared<QuadHeightMapTessellationDemo>();
	quadHeightMapTessellation->SetCamera(*camera);
	quadHeightMapTessellation->SetKeyboard(*m_keyboard);

	auto modelTessellation = std::make_shared<ModelTessellationDemo>();
	modelTessellation->SetCamera(*camera);
	modelTessellation->SetKeyboard(*m_keyboard);

	auto instancing = std::make_shared<InstancingPointLightDemo>();
	instancing->SetCamera(*camera);
	instancing->SetKeyboard(*m_keyboard);

	auto computeShader = std::make_shared<ComputeShaderDemo>();
	computeShader->SetCamera(*camera);

	// push needed components
	m_components.push_back(m_keyboard);
	m_components.push_back(m_mouse);
	m_components.push_back(camera);
	m_components.push_back(m_grid);
	m_components.push_back(fps);

	// m_components.push_back(triangle);
	// m_components.push_back(cube);
	// m_components.push_back(model);
	// m_components.push_back(textureModel);
	// m_components.push_back(skybox);
	// m_components.push_back(basic);
	// m_components.push_back(textureMapping);
	// m_components.push_back(diffuseLighting);
	// m_components.push_back(pointLight);
	// m_components.push_back(multiplePointLights);
	// m_components.push_back(spotlight);
	// m_components.push_back(normalMapping);
	// m_components.push_back(environmentMapping);
	// m_components.push_back(transparencyMapping);
	// m_components.push_back(displacementMapping);
	// m_components.push_back(fog);
	// m_components.push_back(projectiveTextureMapping);
	// m_components.push_back(projectiveTextureDepthMapMapping);
	// m_components.push_back(shadowMapping);
	// m_components.push_back(directionalShadowMapping);
	// m_components.push_back(animation);
	// m_components.push_back(geometryShader);
	// m_components.push_back(basicTessellation);
	// m_components.push_back(quadHeightMapTessellation);
	// m_components.push_back(modelTessellation);
	// m_components.push_back(instancing);
	// m_components.push_back(computeShader);

	// hide grid by default
	m_grid->SetVisible(false);

	Application::Initialize();

	m_multiplePointLights->Initialize(*this);
	m_postProcessing->Initialize(*this);

	// deferred shading for multiplePointLights only
	{
		auto material = m_multiplePointLights->GetMaterial();

		m_fullScreenQuad = std::make_unique<FullScreenQuadComponent>();
		m_fullScreenQuad->SetMaterial(*material, "deferred", "light");
		m_fullScreenQuad->SetMaterialUpdateFunction(
			[this, material]()
			{
				material->GetColorBufferTexture() << m_multipleRenderTarget->GetOutputTexture(0);
				material->GetNormalBufferTexture() << m_multipleRenderTarget->GetOutputTexture(1);
				material->GetWorldPositionBufferTexture() << m_multipleRenderTarget->GetOutputTexture(2);
				material->GetDepthBufferTexture() << m_multipleRenderTarget->GetDepthOutputTexture();
			}
		);
		m_fullScreenQuad->Initialize(*this);
	}
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

		if (m_keyboard->WasKeyPressed(Key::Space))
		{
			m_deferredLightingEnabled = !m_deferredLightingEnabled;

			auto material = m_multiplePointLights->GetMaterial();

			if (m_deferredLightingEnabled)
				material->SetCurrentTechnique("deferred", "geometry");
			else
				material->SetCurrentTechnique("forward");
		}
	}

	m_multiplePointLights->Update(time);

	if (m_deferredLightingEnabled)
		m_fullScreenQuad->Update(time);

	if (m_postProcessingEnabled)
		m_postProcessing->Update(time);

	Application::Update(time);
}

void DemoApplication::Draw(const Time& time)
{
	// clear main RT
	m_deviceContext->ClearRenderTargetView(
		m_renderTargetView.Get(),
		static_cast<const float*>(k_backgroundColor)
	);
	m_deviceContext->ClearDepthStencilView(
		m_depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0
	);

	// push post-processing RT
	if (m_postProcessingEnabled)
	{
		m_sceneRenderTarget->Begin();

		m_deviceContext->ClearRenderTargetView(
			m_sceneRenderTarget->GetRenderTargetView(),
			static_cast<const float*>(k_backgroundColor)
		);
		m_deviceContext->ClearDepthStencilView(
			m_sceneRenderTarget->GetDepthStencilView(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.0f,
			0
		);
	}

	// push deferred lighting RT
	if (m_deferredLightingEnabled)
	{
		// Render the scene to an off-screen texture
		m_multipleRenderTarget->Begin();

		for (unsigned i = 0, count = m_multipleRenderTarget->GetCount(); i < count; i++)
		{
			m_deviceContext->ClearRenderTargetView(
				m_multipleRenderTarget->GetRenderTargetView(i),
				static_cast<const float*>(k_mrtBackgroundColor)
			);
		}

		m_deviceContext->ClearDepthStencilView(
			m_multipleRenderTarget->GetDepthStencilView(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.0f,
			0
		);
	}

	m_multiplePointLights->Draw(time);

	// pop deferred lighting RT
	if (m_deferredLightingEnabled)
	{
		m_multipleRenderTarget->End();
		m_fullScreenQuad->Draw(time);
		UnbindPixelShaderResources(0, 4);
	}

	m_renderer->RenderScene(time);

	// pop post-processing RT
	if (m_postProcessingEnabled)
	{
		m_sceneRenderTarget->End();
		m_postProcessing->Draw(time);
	}

	m_renderer->RenderUI(time);

	HRESULT hr = m_swapChain->Present(0, 0);
	assert("IDXGISwapChain::Present() failed." && SUCCEEDED(hr));
}

void DemoApplication::Shutdown()
{
	m_directInput.Reset();

	Application::Shutdown();
}
