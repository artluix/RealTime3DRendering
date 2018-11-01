#pragma once
#include "library/components/SceneComponent.h"
#include "library/Math.h"
#include "library/Path.h"

namespace library
{
	namespace components
	{

		class KeyboardComponent;

		class CubeDemoComponent : public rtti::Class<CubeDemoComponent, SceneComponent>
		{
		public:
			explicit CubeDemoComponent(
				const Application& app,
				const CameraComponent& camera,
				const KeyboardComponent& keyboard
			);

			void Initialize() override;
			void Update(const Time& time) override;
			void Draw(const Time& time) override;

		private:
			static const unsigned k_indicesCount;
			static const unsigned k_verticesCount;
			static const float k_rotationAngle;
			static const float k_movementRate;
			static const filesystem::Path k_effectPath;

			ComPtr<ID3DX11Effect> m_effect;
			ComPtr<ID3DX11EffectTechnique> m_technique;
			ComPtr<ID3DX11EffectPass> m_pass;
			ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;

			ComPtr<ID3D11InputLayout> m_inputLayout;
			ComPtr<ID3D11Buffer> m_indexBuffer;
			ComPtr<ID3D11Buffer> m_vertexBuffer;

			std::reference_wrapper<const KeyboardComponent> m_keyboard;
		};

	} // namespace components
} // namespace library
