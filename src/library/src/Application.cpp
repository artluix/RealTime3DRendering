#include "StdAfx.h"
#include "library/Application.h"

#include "library/Path.h"
#include "library/Utils.h"

#include "library/Components/Component.h"

#include "library/Effect/Effect.h"

#include "library/RasterizerStates.h"
#include "library/BlendStates.h"
#include "library/SamplerStates.h"

#include <DDSTextureLoader.h>
#include <thread>

namespace library
{

namespace
{
constexpr unsigned k_defaultScreenWidth = 1280;
constexpr unsigned k_defaultScreenHeight = 1024;
constexpr unsigned k_defaultFrameRate = 60;
constexpr unsigned k_defaultMultiSamplingCount = 4;
constexpr bool k_defaultIsFullscreen = false;
} // namespace

//-------------------------------------------------------------------------

Application::Application(
	const HINSTANCE instanceHandle,
	const std::wstring& windowClass,
	const std::wstring& windowTitle,
	const int showCmd)
	: m_instanceHandle(instanceHandle)
	, m_windowClass(windowClass)
	, m_windowTitle(windowTitle)
	, m_showCmd(showCmd)
	, m_windowHandle(nullptr)
	, m_window{}
	, m_screenWidth(k_defaultScreenWidth)
	, m_screenHeight(k_defaultScreenHeight)
	, m_components{}
	, m_featureLevel(D3D_FEATURE_LEVEL_9_1)
	, m_frameRate(k_defaultFrameRate)
	, m_isFullScreen(k_defaultIsFullscreen)
	, m_depthStencilBufferEnabled(true)
	, m_multiSamplingEnabled(false)
	, m_multiSamplingCount(k_defaultMultiSamplingCount)
	, m_multiSamplingQualityLevels(0)
	, m_viewport{}
{}

Application::~Application() = default;

float Application::GetAspectRatio() const
{
	return static_cast<float>(m_screenWidth) / m_screenHeight;
}

//-------------------------------------------------------------------------

void Application::Initialize()
{
	m_renderer = std::make_unique<Renderer>(*this);

	for (const auto& component : m_components)
	{
		component->Initialize(*this);
	}
}

void Application::Run()
{
	using namespace std::chrono_literals;

	InitializeWindow();
	InitializeDirectX();
	Initialize();

	MSG message{};

	m_stopwatch.Reset();

	while (message.message != WM_QUIT)
	{
		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			m_stopwatch.UpdateTime(m_time);

			Update(m_time);
			Draw(m_time);

			std::this_thread::sleep_for(1ms);
		}
	}

	Shutdown();
}

void Application::Exit()
{
	PostQuitMessage(0);
}

//-------------------------------------------------------------------------

void Application::Update(const Time& time)
{
	for (const auto& component : m_components)
	{
		if (component->IsEnabled())
		{
			component->Update(time);
		}
	}
}

void Application::Draw(const Time& time)
{
	m_renderer->RenderScene(time);
	m_renderer->RenderUI(time);
}

//-------------------------------------------------------------------------

void Application::ResetRenderTargets() const
{
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView.Get());
}

void Application::UnbindPixelShaderResources(const unsigned startIdx, const unsigned count) const
{
	std::vector<ID3D11ShaderResourceView*> emptySrv(count, nullptr);
	m_deviceContext->PSSetShaderResources(startIdx, count, emptySrv.data());
}

//-------------------------------------------------------------------------

ComPtr<ID3D11ShaderResourceView> Application::LoadTexture(const std::string& textureName) const
{
	ComPtr<ID3D11ShaderResourceView> texture;

	if (textureName.empty())
		return texture;

	// texture name must be without extension
	assert(!Path(textureName).GetExt());

	const auto texturePath = GetTexturesPath() + Path(textureName + ".dds");

	std::vector<std::byte> textureData;
	utils::LoadBinaryFile(texturePath, textureData);
	assert("Load texture failed." && !textureData.empty());

	auto hr = DirectX::CreateDDSTextureFromMemory(
		m_device.Get(),
		m_deviceContext.Get(),
		reinterpret_cast<const std::uint8_t*>(textureData.data()),
		textureData.size(),
		nullptr,
		&texture
	);
	assert("CreateDDSTextureFromMemory() failed." && SUCCEEDED(hr));

	return texture;
}

//-------------------------------------------------------------------------

const Path& Application::GetTexturesPath() const
{
	static const auto s_texturesPath = GetDataPath() + Path("textures");
	return s_texturesPath;
}

const Path& Application::GetEffectsPath() const
{
	static const auto s_effectsPath = GetDataPath() + Path("effects");
	return s_effectsPath;
}

const Path& Application::GetModelsPath() const
{
	static const auto s_modelsPath = GetDataPath() + Path("models");
	return s_modelsPath;
}

//-------------------------------------------------------------------------

void Application::InitializeWindow()
{
	// window
	m_window = {};
	m_window.cbSize = sizeof(WNDCLASSEX);
	m_window.style = CS_CLASSDC;
	m_window.lpfnWndProc = WndProc;
	m_window.hInstance = m_instanceHandle;
	m_window.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	m_window.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	m_window.hCursor = LoadCursor(nullptr, IDC_ARROW);
	m_window.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
	m_window.lpszClassName = m_windowClass.c_str();

	RECT windowRect = {0, 0, static_cast<int>(m_screenWidth), static_cast<int>(m_screenHeight)};
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	RegisterClassEx(&m_window);

	POINT windowPosition = CenterWindow(m_screenWidth, m_screenHeight);

	m_windowHandle = CreateWindow(
		m_windowClass.c_str(),
		m_windowTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		windowPosition.x,
		windowPosition.y,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,
		nullptr,
		m_instanceHandle,
		nullptr);

	::GetWindowRect(m_windowHandle, &m_windowRect);

	// set cursor position
	POINT p{0, 0};
	if (ClientToScreen(m_windowHandle, &p))
	{
		SetCursorPos(p.x, p.y);
	}

	ShowWindow(m_windowHandle, m_showCmd);
	UpdateWindow(m_windowHandle);
}

void Application::InitializeDirectX()
{
	HRESULT hr;

	unsigned createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	std::vector<D3D_FEATURE_LEVEL> featureLevels = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	// create D3DDevice and D3DDeviceContext
	{
		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> deviceContext;

		hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			createDeviceFlags,
			featureLevels.data(),
			static_cast<UINT>(featureLevels.size()),
			D3D11_SDK_VERSION,
			&device,
			&m_featureLevel,
			&deviceContext
		);
		assert("D3D11CreateDevice() failed." && SUCCEEDED(hr));

		hr = device.As(m_device);
		assert("ID3D11Device As ID3D11Device1 failed." && SUCCEEDED(hr));

		hr = deviceContext.As(m_deviceContext);
		assert("ID3D11DeviceContext As ID3D11DeviceContext1 failed." && SUCCEEDED(hr));
	}

	m_device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		m_multiSamplingCount,
		&m_multiSamplingQualityLevels
	);
	assert("Unsupported multi-sampling quality." && m_multiSamplingQualityLevels);

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = m_screenWidth;
	swapChainDesc.Height = m_screenHeight;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (m_multiSamplingEnabled)
	{
		swapChainDesc.SampleDesc.Count = m_multiSamplingCount;
		swapChainDesc.SampleDesc.Quality = m_multiSamplingQualityLevels - 1;
	}
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// create DXGISwapChain
	{
		ComPtr<IDXGIDevice> dxgiDevice;
		hr = m_device.As(dxgiDevice);
		assert("ID3D11Device as IDXGIDevice failed." && SUCCEEDED(hr));

		ComPtr<IDXGIAdapter> dxgiAdapter;
		hr = dxgiDevice->GetAdapter(&dxgiAdapter);
		assert("IDXGIDevice::GetAdapter() failed retrieving adapter." && SUCCEEDED(hr));

		ComPtr<IDXGIFactory2> dxgiFactory;
		hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory));
		assert("IDXGIAdapter::GetParent() failed retrieving factory." && SUCCEEDED(hr));

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc{};
		fullScreenDesc.RefreshRate.Numerator = m_frameRate;
		fullScreenDesc.RefreshRate.Denominator = 1;
		fullScreenDesc.Windowed = !m_isFullScreen;

		hr = dxgiFactory->CreateSwapChainForHwnd(
			dxgiDevice.Get(),
			m_windowHandle,
			&swapChainDesc,
			&fullScreenDesc,
			nullptr,
			&m_swapChain
		);
		assert("IDXGIDevice::CreateSwapChainForHwnd() failed." && SUCCEEDED(hr));
	}

	// create back buffer
	{
		ComPtr<ID3D11Texture2D> backBuffer;
		hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
		assert("IDXGISwapChain::GetBuffer() failed." && SUCCEEDED(hr));

		backBuffer->GetDesc(&m_backBufferDesc);

		hr = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_renderTargetView);
		assert("IDXGI::CreateRenderTargetView() failed." && SUCCEEDED(hr));

		if (m_depthStencilBufferEnabled)
		{
			D3D11_TEXTURE2D_DESC depthStencilBufferDesc{};
			depthStencilBufferDesc.Width = m_screenWidth;
			depthStencilBufferDesc.Height = m_screenHeight;
			depthStencilBufferDesc.MipLevels = 1;
			depthStencilBufferDesc.ArraySize = 1;
			depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

			if (m_multiSamplingEnabled)
			{
				depthStencilBufferDesc.SampleDesc.Count = m_multiSamplingCount;
				depthStencilBufferDesc.SampleDesc.Quality = m_multiSamplingQualityLevels - 1;
			}
			else
			{
				depthStencilBufferDesc.SampleDesc.Count = 1;
				depthStencilBufferDesc.SampleDesc.Quality = 0;
			}

			hr = m_device->CreateTexture2D(&depthStencilBufferDesc, nullptr, &m_depthStencilBuffer);
			assert("IDXGIDevice::CreateTexture2D() failed." && SUCCEEDED(hr));

			hr = m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), nullptr, &m_depthStencilView);
			assert("IDXGIDevice::CreateDepthStencilView() failed." && SUCCEEDED(hr));
		}

		m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView.Get());

		m_viewport.TopLeftX = 0.0f;
		m_viewport.TopLeftY = 0.0f;
		m_viewport.Width = static_cast<float>(m_screenWidth);
		m_viewport.Height = static_cast<float>(m_screenHeight);
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;

		m_deviceContext->RSSetViewports(1, &m_viewport);
	}

	// Set render targets and viewport through render target stack
	Begin();

	// states
	{
		auto const device = GetDevice();
		RasterizerStates::Initialize(device);
		BlendStates::Initialize(device);
		SamplerStates::Initialize(device);
	}
}

void Application::Shutdown()
{
	SamplerStates::Clear();
	BlendStates::Clear();
	RasterizerStates::Clear();

	Effect::ClearAll();

	m_components.clear();
	m_renderer.reset();

	m_renderTargetView.Reset();
	m_depthStencilView.Reset();
	m_swapChain.Reset();
	m_depthStencilBuffer.Reset();

	if (!!m_deviceContext)
	{
		m_deviceContext->ClearState();
		m_deviceContext.Reset();
	}

	m_device.Reset();

	UnregisterClass(m_windowClass.c_str(), m_instanceHandle);
}

//-------------------------------------------------------------------------

void Application::Begin()
{
	RenderTarget::
		Begin(m_deviceContext.Get(), Data(&m_renderTargetView, 1, m_depthStencilView.Get(), m_viewport));
}

void Application::End()
{
	RenderTarget::End(m_deviceContext.Get());
}

//-------------------------------------------------------------------------

POINT Application::CenterWindow(const unsigned windowWidth, const unsigned windowHeight)
{
	const auto screenWidth = static_cast<unsigned>(GetSystemMetrics(SM_CXSCREEN));
	const auto screenHeight = static_cast<unsigned>(GetSystemMetrics(SM_CYSCREEN));

	POINT center;
	center.x = (screenWidth - windowWidth) >> 1;
	center.y = (screenHeight - windowHeight) >> 1;

	return center;
}

LRESULT WINAPI Application::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		default:
			return DefWindowProc(windowHandle, message, wParam, lParam);
	}
}
} // namespace library
