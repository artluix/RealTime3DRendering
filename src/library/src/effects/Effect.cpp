#include "StdAfx.h"
#include "library/effects/Effect.h"

#include "library/Application.h"
#include "library/Path.h"
#include "library/Utils.h"
#include "library/Exception.h"

#include "library/effects/Technique.h"
#include "library/effects/Variable.h"

namespace library
{
	namespace effects
	{
		Effect::Effect(const Application& app)
			: m_app(app)
		{
		}

		ComPtr<ID3DX11Effect> Effect::CompileFromFile(ID3D11Device* const device, const filesystem::Path& path)
		{
			ComPtr<ID3DX11Effect> effect;

			ComPtr<ID3DBlob> errorBlob;
			ComPtr<ID3DBlob> shaderBlob;

			auto hr = D3DCompileFromFile(
				path.GetWideCString(),
				nullptr,
				nullptr,
				nullptr,
				"fx_5_0",
#if defined(DEBUG) || defined(DEBUG)
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
#else
				0,
#endif
				0,
				shaderBlob.GetAddressOf(),
				errorBlob.GetAddressOf()
			);
			if (FAILED(hr))
			{
				std::string error = std::string("D3DX11CompileEffectFromFile() failed: ")
					+ std::string(static_cast<const char*>(errorBlob->GetBufferPointer()), errorBlob->GetBufferSize());
				throw Exception(error.c_str(), hr);
			}

			hr = D3DX11CreateEffectFromMemory(
				shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
				0,
				device,
				effect.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("D3DX11CreateEffectFromMemory() failed.", hr);
			}

			return effect;
		}

		ComPtr<ID3DX11Effect> Effect::LoadCompiledEffect(ID3D11Device* const device, const filesystem::Path& path)
		{
			std::vector<library::byte> effectData;
			utils::LoadBinaryFile(path, effectData);
			if (effectData.empty())
			{
				throw Exception("Load compiled effect failed.");
			}

			ComPtr<ID3DX11Effect> effect;

			auto hr = D3DX11CreateEffectFromMemory(
				effectData.data(), effectData.size(),
				0,
				device,
				effect.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("D3DX11CreateEffectFromMemory() failed.", hr);
			}

			return effect;
		}

		void Effect::SetEffect(const ComPtr<ID3DX11Effect>& effect)
		{
			if (m_effect != effect)
			{
				m_effect = effect;
			}
		}

		//-------------------------------------------------------------------------

		Technique* Effect::GetTechnique(const std::string& techniqueName) const
		{
			const auto it = m_techniquesMap.find(techniqueName);
			if (it != m_techniquesMap.cend())
				return it->second.get();

			return nullptr;
		}

		Technique* Effect::GetTechnique(const unsigned techniqueIdx) const
		{
			return m_techniques[techniqueIdx].get();
		}

		//-------------------------------------------------------------------------

		Variable* const Effect::GetVariable(const std::string& variableName) const
		{
			const auto it = m_variablesMap.find(variableName);
			if (it != m_variablesMap.cend())
				return it->second.get();

			return nullptr;
		}

		Variable* const  Effect::GetVariable(const unsigned variableIdx) const
		{
			return m_variables[variableIdx].get();
		}

		//-------------------------------------------------------------------------

		void Effect::Initialize()
		{
			auto hr = m_effect->GetDesc(&m_effectDesc);
			if (FAILED(hr))
			{
				throw Exception("ID3DX11Effect::GetDesc() failed.", hr);
			}

			for (unsigned i = 0; i < m_effectDesc.Techniques; i++)
			{
				auto technique = std::make_shared<Technique>(m_app, *this, m_effect->GetTechniqueByIndex(i));
				m_techniques.push_back(technique);
				m_techniquesMap.insert(std::make_pair(technique->GetName(), technique));
			}
		}

	} // namespace effects
} // namespace library
