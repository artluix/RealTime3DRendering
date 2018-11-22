#pragma once
#include <library/components/SceneComponent.h>
#include <library/Math.h>

namespace library
{
	namespace components
	{
		class KeyboardComponent;
	} // namespace components
} // namespace library

namespace demo
{
	class CubeComponent : public library::rtti::Class<CubeComponent, library::components::SceneComponent>
	{
	public:
		explicit CubeComponent(
			const library::Application& app,
			const library::components::CameraComponent& camera,
			const library::components::KeyboardComponent& keyboard
		);

		void Initialize() override;
		void Update(const library::Time& time) override;
		void Draw(const library::Time& time) override;

	private:
		ComPtr<ID3DX11Effect> m_effect;
		ComPtr<ID3DX11EffectTechnique> m_technique;
		ComPtr<ID3DX11EffectPass> m_pass;
		ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;

		ComPtr<ID3D11InputLayout> m_inputLayout;
		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11Buffer> m_vertexBuffer;

		std::reference_wrapper<const library::components::KeyboardComponent> m_keyboard;
	};
} // namespace demo
