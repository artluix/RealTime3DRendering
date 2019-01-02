#pragma once
#include "effects/BasicMaterial.h"

#include <library/components/SceneComponent.h>
#include <library/DirectXForwardDeclarations.h>

#include <library/effects/Effect.h>

namespace library
{
	class KeyboardComponent;
} // namespace library

namespace demo
{
	class MaterialComponent : public library::rtti::Class<MaterialComponent, library::SceneComponent>
	{
	public:
		explicit MaterialComponent(
			const library::Application& app,
			const library::CameraComponent& camera,
			const library::KeyboardComponent& keyboard
		);

		void Initialize() override;
		void Update(const library::Time& time) override;
		void Draw(const library::Time& time) override;

	private:
		BasicMaterial m_material;
		library::Effect m_effect;

		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11Buffer> m_vertexBuffer;

		unsigned m_indicesCount;

		std::reference_wrapper<const library::KeyboardComponent> m_keyboard;
	};

} // namespace demo
