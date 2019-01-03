#include "StdAfx.h"
#include "library/effects/Effect.h"

#include "library/Application.h"
#include "library/Path.h"
#include "library/Utils.h"
#include "library/Exception.h"

#include "library/effects/EffectTechnique.h"
#include "library/effects/EffectVariable.h"

namespace library
{
	Effect::Effect(const Application& app)
		: m_app(app)
	{
	}

	Effect::~Effect()
	{
		// variables
		std::for_each(m_variables.begin(), m_variables.end(), std::default_delete<EffectVariable>());
		m_variables.clear();
		m_variablesMap.clear();

		// techniques
		std::for_each(m_techniques.begin(), m_techniques.end(), std::default_delete<EffectTechnique>());
		m_techniques.clear();
		m_techniquesMap.clear();
	}

	ComPtr<ID3DX11Effect> Effect::CompileFromFile(ID3D11Device* const device, const fs::Path& path)
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

	ComPtr<ID3DX11Effect> Effect::LoadCompiledEffect(ID3D11Device* const device, const fs::Path& path)
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

	void Effect::CompileFromFile(const fs::Path& path)
	{
		m_effect = CompileFromFile(m_app.GetD3DDevice(), path);
		Initialize();
	}

	void Effect::LoadCompiledEffect(const fs::Path& path)
	{
		m_effect = LoadCompiledEffect(m_app.GetD3DDevice(), path);
		Initialize();
	}

	void Effect::SetEffect(const ComPtr<ID3DX11Effect>& effect)
	{
		if (m_effect != effect)
		{
			m_effect = effect;
		}
	}

	//-------------------------------------------------------------------------

	bool Effect::HasTechnique(const std::string& techniqueName) const
	{
		return m_techniquesMap.find(techniqueName) != m_techniquesMap.cend();
	}

	EffectTechnique& Effect::GetTechnique(const std::string& techniqueName) const
	{
		return *m_techniquesMap.at(techniqueName);
	}

	EffectTechnique& Effect::GetTechnique(const unsigned techniqueIdx) const
	{
		return *m_techniques.at(techniqueIdx);
	}

	//-------------------------------------------------------------------------

	bool Effect::HasVariable(const std::string& variableName) const
	{
		return m_variablesMap.find(variableName) != m_variablesMap.cend();
	}

	EffectVariable& Effect::GetVariable(const std::string& variableName) const
	{
		return *m_variablesMap.at(variableName);
	}

	EffectVariable& Effect::GetVariable(const unsigned variableIdx) const
	{
		return *m_variables.at(variableIdx);
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
			auto technique = new EffectTechnique(m_app, *this, m_effect->GetTechniqueByIndex(i));
			m_techniques.push_back(technique);
			m_techniquesMap.emplace(technique->GetName(), technique);
		}

		for (unsigned i = 0; i < m_effectDesc.GlobalVariables; i++)
		{
			auto variable = new EffectVariable(*this, m_effect->GetVariableByIndex(i));
			m_variables.push_back(variable);
			m_variablesMap.emplace(variable->GetName(), variable);
		}
	}

} // namespace library
