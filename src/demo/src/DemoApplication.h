#pragma once
#include <library/Application.h>

#include <dinput.h>

namespace library
{
class Path;

class KeyboardComponent;
class MouseComponent;

class GridComponent;

class FullScreenRenderTarget;
class PostProcessingComponent;

class MultipleRenderTarget;
class FullScreenQuadComponent;
} // namespace library

class MultipleLightsDemo;

class DemoApplication : public library::Application
{
public:
	DemoApplication(
		const HINSTANCE instanceHandle,
		const std::wstring& windowClass,
		const std::wstring& windowTitle,
		const int showCmd
	);
	~DemoApplication();

	void Initialize() override;
	void Update(const library::Time& time) override;
	void Draw(const library::Time& time) override;

	const library::Path& GetDataPath() const override;

protected:
	void Shutdown() override;

private:
	std::shared_ptr<library::KeyboardComponent> m_keyboard;
	std::shared_ptr<library::MouseComponent> m_mouse;

	std::shared_ptr<library::GridComponent> m_grid;

	std::unique_ptr<library::FullScreenRenderTarget> m_sceneRenderTarget;
	std::unique_ptr<library::PostProcessingComponent> m_postProcessing;
	bool m_postProcessingEnabled;

	std::unique_ptr<library::MultipleRenderTarget> m_multipleRenderTarget;
	std::unique_ptr<library::FullScreenQuadComponent> m_fullScreenQuad;
	bool m_deferredLightingEnabled;

	library::ComPtr<IDirectInput8> m_directInput;
};
