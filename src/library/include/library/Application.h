#pragma once
#include "library/RenderTargets/RenderTarget.h"
#include "library/Render/RenderBuffers.h"

#include "library/Stopwatch.h"
#include "library/Common.h"

#include <Windows.h>
#include <vector>
#include <string>

namespace library
{
class Renderer;
class Path;

class Component;
using ComponentPtr = std::shared_ptr<Component>;

class Application : public RenderTarget
{
public:
	Application(
		const HINSTANCE instanceHandle,
		const std::wstring& windowClass,
		const std::wstring& windowTitle,
		const int showCmd);
	virtual ~Application();

	HINSTANCE GetInstanceHandle() const { return m_instanceHandle; }
	HWND GetWindowHandle() const { return m_windowHandle; }
	const WNDCLASSEX& GetWindow() const { return m_window; }
	const std::wstring& GetWindowClass() const { return m_windowClass; }
	const std::wstring& GetWindowTitle() const { return m_windowTitle; }
	unsigned GetScreenWidth() const { return m_screenWidth; }
	unsigned GetScreenHeight() const { return m_screenHeight; }
	const RECT& GetWindowRect() const { return m_windowRect; }

	ID3D11Device1* GetDevice() const { return m_device.Get(); }
	ID3D11DeviceContext1* GetDeviceContext() const { return m_deviceContext.Get(); }

	bool IsDepthBufferEnabled() const { return m_depthStencilBufferEnabled; }
	float GetAspectRatio() const;
	bool IsFullScreen() const { return m_isFullScreen; }
	const D3D11_TEXTURE2D_DESC& GetBackBufferDesc() const { return m_backBufferDesc; }

	const D3D11_VIEWPORT& GetViewport() const { return m_viewport; }

	const std::vector<ComponentPtr>& GetComponents() const { return m_components; }

	Renderer* GetRenderer() const { return m_renderer.get(); }

	virtual void Begin() override final;

	virtual void Initialize();
	virtual void Run();
	virtual void Exit();

	virtual void Update(const Time& time);
	virtual void Draw(const Time& time) = 0;

	virtual ComPtr<ID3D11ShaderResourceView> CreateCubeTextureSRV(const std::string& textureFilenameStr) const;
	virtual ComPtr<ID3D11ShaderResourceView> CreateTexture2DSRV(const std::string& textureFilenameStr) const;

	virtual const Path& GetDataPath() const = 0;
	virtual const Path& GetTexturesPath() const;
	virtual const Path& GetEffectsPath() const;
	virtual const Path& GetModelsPath() const;

protected:
	virtual void InitializeWindow();
	virtual void InitializeDirectX();

	virtual void Shutdown();

	const Application& GetApp() const { return *this; }

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

	std::vector<ComponentPtr> m_components;

	std::unique_ptr<Renderer> m_renderer;

	// DirectX
	D3D_FEATURE_LEVEL m_featureLevel;

	ComPtr<ID3D11Device1> m_device;
	ComPtr<ID3D11DeviceContext1> m_deviceContext;
	ComPtr<IDXGISwapChain1> m_swapChain;

	unsigned m_frameRate;
	bool m_isFullScreen;

	bool m_multiSamplingEnabled;
	unsigned m_multiSamplingCount;
	unsigned m_multiSamplingQualityLevels;

	D3D11_VIEWPORT m_viewport;

	D3D11_TEXTURE2D_DESC m_backBufferDesc;
	RenderTargetBufferPtr m_rtBuffer;

	bool m_depthStencilBufferEnabled;
	DepthStencilBufferPtr m_dsBuffer;

private:
	POINT CenterWindow(const unsigned windowWidth, const unsigned windowHeight);

	static LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
};
} // namespace library
