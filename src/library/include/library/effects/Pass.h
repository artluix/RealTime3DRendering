#pragma once
#include "library/Common.h"
#include "library/NonCopyable.hpp"

#include <d3dx11effect.h>
#include <string>
#include <vector>

interface ID3DX11EffectPass;
interface ID3D11DeviceContext;

namespace library
{
	class Application;

	namespace effects
	{
		class Technique;

		class Pass : public NonCopyable<Pass>
		{
		public:
			explicit Pass(const Application& app, Technique& technique, const ComPtr<ID3DX11EffectPass>& pass);

			Technique& GetTechnique() { return m_technique; }
			const std::string& GetName() const { return m_name; }

			ID3DX11EffectPass* const GetPass() const { return m_pass.Get(); }
			const D3DX11_PASS_DESC& GetPassDesc() const { return m_passDesc; }

			ComPtr<ID3D11InputLayout> CreateInputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescriptions);
			void Apply(const unsigned flags, ID3D11DeviceContext* const deviceContext);

		private:
			const Application& m_app;

			ComPtr<ID3DX11EffectPass> m_pass;
			D3DX11_PASS_DESC m_passDesc;

			Technique& m_technique;
			std::string m_name;
		};

		using PassPtr = std::shared_ptr<Pass>;

	} // namespace effects
} // namespace library
