#pragma once
#include <library/components/SceneComponent.h>

struct ID3D11Buffer;
struct ID3D11Device;

struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectPass;
struct ID3DX11EffectMatrixVariable;

namespace library
{
	class Mesh;

	namespace components
	{
		class KeyboardComponent;
	} // namespace components
} // namespace library

namespace demo
{
	class ModelComponent : public library::rtti::Class<ModelComponent, library::components::SceneComponent>
	{
	public:
		explicit ModelComponent(
			const library::Application& app,
			const library::components::CameraComponent& camera,
			const library::components::KeyboardComponent& keyboard
		);

		void Initialize() override;
		void Update(const library::Time& time) override;
		void Draw(const library::Time& time) override;

	private:
		void CreateVertexBuffer(const ComPtr<ID3D11Device>& device, const library::Mesh& mesh);

		ComPtr<ID3DX11Effect> m_effect;
		ComPtr<ID3DX11EffectTechnique> m_technique;
		ComPtr<ID3DX11EffectPass> m_pass;
		ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;

		ComPtr<ID3D11InputLayout> m_inputLayout;
		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11Buffer> m_vertexBuffer;

		unsigned m_indicesCount;

		std::reference_wrapper<const library::components::KeyboardComponent> m_keyboard;
	};
} // namespace demo
