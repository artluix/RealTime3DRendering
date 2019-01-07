#include "StdAfx.h"
#include "library/effect/Effect.h"

#include "library/Application.h"
#include "library/Utils.h"
#include "library/Exception.h"

#include "library/effect/EffectTechnique.h"
#include "library/effect/EffectVariable.h"

namespace library
{
	Effect::Effect(const Application& app, const fs::Path& path)
		: m_app(app)
		, m_path(path)
		//, m_name(path.GetBaseName().SplitExt()[0].GetString())
	{
	}

	Effect::~Effect() = default;

	//-------------------------------------------------------------------------

	void Effect::Compile()
	{
		ComPtr<ID3DBlob> errorBlob;
		ComPtr<ID3DBlob> shaderBlob;

		auto hr = D3DCompileFromFile(
			m_path.GetWideCString(),
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
			m_app.GetD3DDevice(),
			m_effect.GetAddressOf()
		);
		if (FAILED(hr))
		{
			throw Exception("D3DX11CreateEffectFromMemory() failed.", hr);
		}
	}

	void Effect::LoadCompiled()
	{
		std::vector<library::byte> effectData;
		utils::LoadBinaryFile(m_path, effectData);
		if (effectData.empty())
		{
			throw Exception("Load compiled effect failed.");
		}

		auto hr = D3DX11CreateEffectFromMemory(
			effectData.data(), effectData.size(),
			0,
			m_app.GetD3DDevice(),
			m_effect.GetAddressOf()
		);
		if (FAILED(hr))
		{
			throw Exception("D3DX11CreateEffectFromMemory() failed.", hr);
		}
	}

	//-------------------------------------------------------------------------

	void Effect::SetEffect(ID3DX11Effect* const effect)
	{
		if (m_effect.Get() != effect)
		{
			m_effect = effect;
		}
	}

	//-------------------------------------------------------------------------

	EffectTechnique* Effect::GetTechnique(const std::string& techniqueName) const
	{
		auto it = m_techniquesMap.find(techniqueName);
		if (it == m_techniquesMap.end())
			return nullptr;

		return it->second;
	}

	EffectTechnique* Effect::GetTechnique(const unsigned techniqueIdx) const
	{
		if (techniqueIdx >= m_techniques.size())
			return nullptr;

		return m_techniques[techniqueIdx].get();
	}

	//-------------------------------------------------------------------------

	EffectVariable* Effect::GetVariable(const std::string& variableName) const
	{
		auto it = m_variablesMap.find(variableName);
		if (it == m_variablesMap.end())
			return nullptr;

		return it->second;
	}

	EffectVariable* Effect::GetVariable(const unsigned variableIdx) const
	{
		if (variableIdx >= m_variables.size())
			return nullptr;

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
			auto technique = std::make_unique<EffectTechnique>(m_app, *this, m_effect->GetTechniqueByIndex(i));
			m_techniquesMap.emplace(technique->GetName(), technique.get());
			m_techniques.push_back(std::move(technique));
		}

		for (unsigned i = 0; i < m_effectDesc.GlobalVariables; i++)
		{
			auto variable = std::make_unique<EffectVariable>(*this, m_effect->GetVariableByIndex(i));
			m_variablesMap.emplace(variable->GetName(), variable.get());
			m_variables.push_back(std::move(variable));
		}
	}

} // namespace library
