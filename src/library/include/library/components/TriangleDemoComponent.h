#pragma once
#include "library/components/ViewedComponent.h"
#include "library/Math.h"

namespace library
{
	namespace components
	{

		class TriangleDemoComponent : public rtti::Class<TriangleDemoComponent, ViewedComponent>
		{
		public:
			explicit TriangleDemoComponent(const Application& app, const CameraComponent& camera);

			void Initialize() override;
			void Update(const Time& time) override;
			void Draw(const Time& time) override;

		private:
			ComPtr<ID3DX11Effect> m_effect;
			ComPtr<ID3DX11EffectTechnique> m_technique;
			ComPtr<ID3DX11EffectPass> m_pass;
			ComPtr<ID3DX11EffectMatrixVariable> m_wvpVariable;

			ComPtr<ID3D11InputLayout> m_inputLayout;
			ComPtr<ID3D11Buffer> m_vertexBuffer;
		};

	} // namespace components
} // namespace library