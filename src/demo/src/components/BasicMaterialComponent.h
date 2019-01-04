#pragma once
#include <library/components/SceneComponent.h>
#include <library/DirectXForwardDeclarations.h>

namespace library
{
	class KeyboardComponent;
	class Effect;
} // namespace library

namespace demo
{
	class BasicMaterial;

	class BasicMaterialComponent : public library::rtti::Class<BasicMaterialComponent, library::SceneComponent>
	{
	public:
		explicit BasicMaterialComponent(
			const library::Application& app,
			const library::CameraComponent& camera,
			const library::KeyboardComponent& keyboard
		);

		void Initialize() override;
		void Update(const library::Time& time) override;
		void Draw(const library::Time& time) override;

	private:
		std::unique_ptr<library::Effect> m_effect;
		std::unique_ptr<BasicMaterial> m_material;

		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11Buffer> m_vertexBuffer;

		unsigned m_indicesCount;

		std::reference_wrapper<const library::KeyboardComponent> m_keyboard;
	};

} // namespace demo
