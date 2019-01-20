#pragma once
#include <library/Application.h>
#include <library/CommonTypes.h>
#include <library/Color.h>
#include <library/math/Matrix.h>

#include <library/materials/ColorFilterMaterial.h>

#include <dinput.h>

namespace library
{
	class KeyboardComponent;
	class MouseComponent;

	class FullScreenQuadComponent;
	class FullScreenRenderTarget;

	class Effect;
} // namespace library

namespace demo
{
	class Application : public library::Application
	{
	public:
		explicit Application(
			const HINSTANCE instanceHandle,
			const std::wstring& windowClass,
			const std::wstring& windowTitle,
			const int showCmd
		);
		~Application();

		void Initialize() override;
		void Update(const library::Time& time) override;
		void Draw(const library::Time& time) override;

	protected:
		void Shutdown() override;

	private:
		void UpdateGenericColorFilter(const library::Time& time);

		std::shared_ptr<library::KeyboardComponent> m_keyboard;
		std::shared_ptr<library::MouseComponent> m_mouse;

		std::unique_ptr<library::FullScreenQuadComponent> m_fullScreenQuad;
		std::unique_ptr<library::FullScreenRenderTarget> m_renderTarget;

		std::shared_ptr<library::Effect> m_colorFilterEffect;
		std::unique_ptr<library::ColorFilterMaterial> m_colorFilterMaterial;
		library::ColorFilter::Type m_colorFilterType;
		library::math::Matrix4 m_genericColorFilter;

		ComPtr<IDirectInput8> m_directInput;
	};
} // namespace demo
