#include "StdAfx.h"
#include "library/Application.h"

#include "library/Exception.h"
#include "library/components/DrawableComponent.h"
#include "library/components/SceneComponent.h"
#include "library/components/UIComponent.h"

namespace library
{
	namespace
	{
		constexpr unsigned k_defaultScreenWidth = 1280;
		constexpr unsigned k_defaultScreenHeight = 1024;
		constexpr unsigned k_defaultFrameRate = 60;
		constexpr unsigned k_defaultMultiSamplingCount = 4;
		constexpr bool k_defaultIsFullscreen = false;
	}

	Application::Application(
		const HINSTANCE instanceHandle,
		const std::wstring& windowClass,
		const std::wstring& windowTitle,
		const int showCmd
	)
		: m_instanceHandle(instanceHandle)
		, m_windowClass(windowClass)
		, m_windowTitle(windowTitle)
		, m_showCmd(showCmd)
		, m_windowHandle(nullptr)
		, m_window{}
		, m_screenWidth(k_defaultScreenWidth), m_screenHeight(k_defaultScreenHeight)
		, m_components{}
		, m_featureLevel(D3D_FEATURE_LEVEL_9_1)
		, m_frameRate(k_defaultFrameRate)
		, m_isFullScreen(k_defaultIsFullscreen)
		, m_depthStencilBufferEnabled(false)
		, m_multiSamplingEnabled(false)
		, m_multiSamplingCount(k_defaultMultiSamplingCount)
		, m_multiSamplingQualityLevels(0)
		, m_renderStatesStorage(*this)
		, m_viewport{}
	{
	}

	Application::~Application()
	{}

	float Application::GetAspectRatio() const
	{
		return static_cast<float>(m_screenWidth) / m_screenHeight;
	}

	//-------------------------------------------------------------------------

	void Application::Initialize()
	{
		for (const auto& component : m_components)
		{
			component->Initialize();
		}
	}

	void Application::Run()
	{
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
		std::vector<SceneComponent*> sceneComponents;
		std::vector<UIComponent*> uiComponents;

		for (const auto& component : m_components)
		{
			auto drawableComponent = component->As<DrawableComponent>();
			if (!!drawableComponent && drawableComponent->IsVisible())
			{
				if (auto sceneComponent = drawableComponent->As<SceneComponent>())
					sceneComponents.push_back(sceneComponent);

				if (auto uiComponent = drawableComponent->As<UIComponent>())
					uiComponents.push_back(uiComponent);
			}
		}

		// render scene first
		{
			for (auto sceneComponent : sceneComponents)
			{
				sceneComponent->Draw(time);
			}
		}

		// render UI after
		{
			m_renderStatesStorage.SaveState(RenderState::All);

			for (auto uiComponent : uiComponents)
			{
				uiComponent->Draw(time);
			}

			m_renderStatesStorage.RestoreState(RenderState::All);
		}
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

		RECT windowRect = { 0, 0, static_cast<int>(m_screenWidth), static_cast<int>(m_screenHeight) };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		RegisterClassEx(&m_window);

		POINT windowPosition = CenterWindow(m_screenWidth, m_screenHeight);

		m_windowHandle = CreateWindow(
			m_windowClass.c_str(),
			m_windowTitle.c_str(),
			WS_OVERLAPPEDWINDOW,
			windowPosition.x, windowPosition.y,
			windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
			nullptr,
			nullptr,
			m_instanceHandle,
			nullptr
		);

		::GetWindowRect(m_windowHandle, &m_windowRect);

		// set cursor position
		POINT p{ 0, 0 };
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
		//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		std::vector<D3D_FEATURE_LEVEL> featureLevels =
		{
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
				featureLevels.size(),
				D3D11_SDK_VERSION,
				device.GetAddressOf(),
				&m_featureLevel,
				deviceContext.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("D3D11CreateDevice() failed.", hr);
			}

			hr = device.As(&m_device);
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device As ID3D11Device1 failed.", hr);
			}

			hr = deviceContext.As(&m_deviceContext);
			if (FAILED(hr))
			{
				throw Exception("ID3D11DeviceContext As ID3D11DeviceContext1 failed.", hr);
			}
		}

		m_device->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			m_multiSamplingCount,
			&m_multiSamplingQualityLevels
		);
		if (!m_multiSamplingQualityLevels)
		{
			throw Exception("Unsupported multi-sampling quality.");
		}

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
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
			hr = m_device.As(&dxgiDevice);
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device as IDXGIDevice failed.", hr);
			}

			ComPtr<IDXGIAdapter> dxgiAdapter;
			hr = dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());
			if (FAILED(hr))
			{
				throw Exception("IDXGIDevice::GetAdapter() failed retrieving adapter.", hr);
			}

			ComPtr<IDXGIFactory2> dxgiFactory;
			hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), &dxgiFactory);
			if (FAILED(hr))
			{
				throw Exception("IDXGIAdapter::GetParent() failed retrieving factory.", hr);
			}

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc = {};
			fullScreenDesc.RefreshRate.Numerator = m_frameRate;
			fullScreenDesc.RefreshRate.Denominator = 1;
			fullScreenDesc.Windowed = !m_isFullScreen;

			hr = dxgiFactory->CreateSwapChainForHwnd(
				dxgiDevice.Get(),
				m_windowHandle,
				&swapChainDesc,
				&fullScreenDesc,
				nullptr,
				m_swapChain.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("IDXGIDevice::CreateSwapChainForHwnd() failed.", hr);
			}
		}

		// create back buffer
		{
			ComPtr<ID3D11Texture2D> backBuffer;
			hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
			if (FAILED(hr))
			{
				throw Exception("IDXGISwapChain::GetBuffer() failed.", hr);
			}

			backBuffer->GetDesc(&m_backBufferDesc);

			hr = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf());
			if (FAILED(hr))
			{
				throw Exception("IDXGI::CreateRenderTargetView() failed.", hr);
			}

			if (m_depthStencilBufferEnabled)
			{
				D3D11_TEXTURE2D_DESC depthStencilDesc = {};
				depthStencilDesc.Width = m_screenWidth;
				depthStencilDesc.Height = m_screenHeight;
				depthStencilDesc.MipLevels = 1;
				depthStencilDesc.ArraySize = 1;
				depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

				if (m_multiSamplingEnabled)
				{
					depthStencilDesc.SampleDesc.Count = m_multiSamplingCount;
					depthStencilDesc.SampleDesc.Quality = m_multiSamplingQualityLevels - 1;
				}
				else
				{
					depthStencilDesc.SampleDesc.Count = 1;
					depthStencilDesc.SampleDesc.Quality = 0;
				}

				hr = m_device->CreateTexture2D(&depthStencilDesc, nullptr, m_depthStencilBuffer.GetAddressOf());
				if (FAILED(hr))
				{
					throw Exception("IDXGIDevice::CreateTexture2D() failed.", hr);
				}

				hr = m_device->CreateDepthStencilView(
					m_depthStencilBuffer.Get(),
					nullptr,
					m_depthStencilView.GetAddressOf()
				);
				if (FAILED(hr))
				{
					throw Exception("IDXGIDevice::CreateDepthStencilView() failed.", hr);
				}
			}

			m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

			m_viewport.TopLeftX = 0.0f;
			m_viewport.TopLeftY = 0.0f;
			m_viewport.Width = static_cast<float>(m_screenWidth);
			m_viewport.Height = static_cast<float>(m_screenHeight);
			m_viewport.MinDepth = 0.0f;
			m_viewport.MaxDepth = 1.0f;

			m_deviceContext->RSSetViewports(1, &m_viewport);
		}
	}

	void Application::Shutdown()
	{
		m_components.clear();

		m_renderTargetView.Reset();
		m_depthStencilView.Reset();
		m_swapChain.Reset();
		m_depthStencilBuffer.Reset();

		if (!!m_deviceContext)
		{
			m_deviceContext->ClearState();
		}

		m_deviceContext.Reset();
		m_device.Reset();

		UnregisterClass(m_windowClass.c_str(), m_instanceHandle);
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
