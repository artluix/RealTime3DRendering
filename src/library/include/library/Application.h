#pragma once
#include "library/NonCopyable.hpp"
#include "library/Stopwatch.h"
#include "library/Common.h"
#include "library/ServiceContainer.h"
#include "library/RenderStatesStorage.h"

#include <Windows.h>
#include <d3d11_1.h>
#include <vector>
#include <string>

namespace library
{
	class Component;
	using BaseComponentPtr = std::shared_ptr<Component>;

	class Application : public NonCopyable<Application>
	{
	public:
		explicit Application(
			const HINSTANCE instanceHandle,
			const std::wstring& windowClass,
			const std::wstring& windowTitle,
			const int showCmd
		);
		virtual ~Application();

		HINSTANCE GetInstanceHandle() const { return m_instanceHandle; }
		HWND GetWindowHandle() const { return m_windowHandle; }
		const WNDCLASSEX& GetWindow() const { return m_window; }
		const std::wstring& GetWindowClass() const { return m_windowClass; }
		const std::wstring& GetWindowTitle() const { return m_windowTitle; }
		unsigned GetScreenWidth() const { return m_screenWidth; }
		unsigned GetScreenHeight() const { return m_screenHeight; }
		const RECT& GetWindowRect() const { return m_windowRect; }

		ID3D11Device1* GetD3DDevice() const { return m_device.Get(); }
		ID3D11DeviceContext1* GetD3DDeviceContext() const { return m_deviceContext.Get(); }
		bool IsDepthBufferEnabled() const { return m_depthStencilBufferEnabled; }
		float GetAspectRatio() const;
		bool IsFullScreen() const { return m_isFullScreen; }
		const D3D11_TEXTURE2D_DESC& GetBackBufferDesc() const { return m_backBufferDesc; }
		const D3D11_VIEWPORT& GetViewport() const { return m_viewport; }

		const std::vector<BaseComponentPtr>& GetComponents() const { return m_components; }
		
		const ServiceContainer& GetServices() const { return m_services; }
		//ServiceContainer& GetServices() { return m_services; }

		virtual void Initialize();
		virtual void Run();
		virtual void Exit();

		virtual void Update(const Time& time);
		virtual void Draw(const Time& time);

	protected:
		virtual void InitializeWindow();
		virtual void InitializeDirectX();
		virtual void Shutdown();

		HINSTANCE m_instanceHandle;
		std::wstring m_windowClass;
		std::wstring m_windowTitle;
		int m_showCmd;

		HWND m_windowHandle;
		WNDCLASSEX m_window;

		unsigned m_screenWidth, m_screenHeight;
		RECT m_windowRect;

		Stopwatch m_stopwatch;
		Time m_time;

		std::vector<BaseComponentPtr> m_components;
		ServiceContainer m_services;
		RenderStatesStorage m_renderStatesStorage;

		// DirectX
		D3D_FEATURE_LEVEL m_featureLevel;

		ComPtr<ID3D11Device1> m_device;
		ComPtr<ID3D11DeviceContext1> m_deviceContext;
		ComPtr<IDXGISwapChain1> m_swapChain;

		unsigned m_frameRate;
		bool m_isFullScreen;
		bool m_depthStencilBufferEnabled;
		bool m_multiSamplingEnabled;
		unsigned m_multiSamplingCount;
		unsigned m_multiSamplingQualityLevels;

		ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
		D3D11_TEXTURE2D_DESC m_backBufferDesc;
		ComPtr<ID3D11RenderTargetView> m_renderTargetView;
		ComPtr<ID3D11DepthStencilView> m_depthStencilView;
		D3D11_VIEWPORT m_viewport;

	private:
		POINT CenterWindow(const unsigned windowWidth, const unsigned windowHeight);

		static LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
	};

} // namespace library
