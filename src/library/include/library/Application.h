#pragma once
#include "library/Common.h"
#include "library/NonCopyable.hpp"
#include "library/Clock.h"

namespace library
{
    class Component;

    class Application : public NonCopyable<Application>
    {
    public:
        explicit Application(const HINSTANCE instanceHandle, const std::wstring& windowClass, const std::wstring& windowTitle, const int showCmd);
        virtual ~Application();

        HINSTANCE GetInstanceHandle() const { return m_instanceHandle; }
        HWND GetWindowHandle() const { return m_windowHandle; }
        const WNDCLASSEX& GetWindow() const { return m_window; }
        const std::wstring& GetWindowClass() const { return m_windowClass; }
        const std::wstring& GetWindowTitle() const { return m_windowTitle; }
        unsigned GetScreenWidth() const { return m_screenWidth; }
        unsigned GetScreenHeight() const { return m_screenHeight; }

        ID3D11Device1* GetD3DDevice() const;
        ID3D11DeviceContext1* GetD3DDeviceContext() const;
        bool DepthBufferEnabled() const;
        float GetAspectRatio() const;
        bool IsFullScreen() const;
        const D3D11_TEXTURE2D_DESC& GetBackBufferDesc() const;
        const D3D11_VIEWPORT& GetViewport() const;

        const std::vector<Component*>& GetComponents() const { return m_components; }

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

        unsigned m_screenWidth;
        unsigned m_screenHeight;

        Clock m_clock;
        Time m_time;

        // components
        std::vector<Component*> m_components;

        // DirectX
        D3D_FEATURE_LEVEL m_featureLevel;
        ComPtr<ID3D11Device1> m_d3dDevice;
        ComPtr<ID3D11DeviceContext1> m_d3dDeviceContext;
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
}