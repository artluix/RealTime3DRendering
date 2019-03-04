#include "StdAfx.h"
#include "library/Effect/Effect.h"

#include "library/Application.h"
#include "library/Utils.h"
#include "library/Exception.h"

#include "library/Effect/EffectTechnique.h"
#include "library/Effect/EffectVariable.h"

#include "library/Logger.h"

namespace library
{
	std::map<std::string, Effect::WeakPtr> Effect::s_effectsCache;

	Effect::Effect(const Application& app, const std::string& name)
		: m_app(app)
		, m_name(name)
	{
	}

	Effect::~Effect() = default;

	//-------------------------------------------------------------------------

	Effect::SharedPtr Effect::Create(
		const Application& app,
		const std::string& effectName,
		const bool compile /* = false */
	)
	{
		auto it = s_effectsCache.find(effectName);
		if (it != s_effectsCache.end())
			if (auto effect = it->second.lock())
					return effect;

		auto effect = SharedPtr(new Effect(app, effectName));
		s_effectsCache.emplace(effectName, effect);

		if (compile)
			effect->Compile();
		else
			effect->LoadCompiled();

		return effect;
	}


	void Effect::ClearAll()
	{
		s_effectsCache.clear();
	}

	//-------------------------------------------------------------------------

	void Effect::Compile()
	{
		m_path = m_app.GetEffectsPath() + Path(m_name + ".fx");

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
			std::string error =
				std::string("D3DX11CompileEffectFromFile() failed: ") +
				std::string(
					static_cast<const char*>(errorBlob->GetBufferPointer()), errorBlob->GetBufferSize()
				);
			throw Exception(error.c_str(), hr);
		}

		hr = D3DX11CreateEffectFromMemory(
			shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
			0,
			m_app.GetDevice(),
			m_effect.GetAddressOf()
		);
		if (FAILED(hr))
		{
			throw Exception("D3DX11CreateEffectFromMemory() failed.", hr);
		}

		Initialize();
	}

	void Effect::LoadCompiled()
	{
		m_path = m_app.GetEffectsPath();
#if defined(DEBUG) || defined(DEBUG)
		m_path += Path(m_name + "_d.fxc");
#else
		m_path += Path(m_name + ".fxc");
#endif

		std::vector<std::byte> effectData;
		utils::LoadBinaryFile(m_path, effectData);
		if (effectData.empty())
		{
			throw Exception("Load compiled effect failed.");
		}

		auto hr = D3DX11CreateEffectFromMemory(
			effectData.data(), effectData.size(),
			0,
			m_app.GetDevice(),
			m_effect.GetAddressOf()
		);
		if (FAILED(hr))
		{
			throw Exception("D3DX11CreateEffectFromMemory() failed.", hr);
		}

		Initialize();
	}

	//-------------------------------------------------------------------------

	void Effect::SetEffect(const ComPtr<ID3DX11Effect>& effect)
	{
		//m_techniquesMap.clear();
		m_techniquesIndexMap.clear();
		m_techniques.clear();

		//m_variablesMap.clear();
		m_variablesIndexMap.clear();
		m_variables.clear();

		m_isInitialized = false;

		m_effect = effect;

		Initialize();
	}

	//-------------------------------------------------------------------------

	bool Effect::HasTechnique(const std::string& techniqueName) const
	{
		//return m_techniquesIndexMap.find(techniqueName) != m_techniquesIndexMap.end();

		if (m_techniquesIndexMap.find(techniqueName) == m_techniquesIndexMap.end())
		{
			Logger::Error("Effect: %s\nTechnique not found: %s", m_name.c_str(), techniqueName.c_str());
			return false;
		}

		return true;
	}

	//-------------------------------------------------------------------------

	const Effect::Technique& Effect::GetTechnique(const std::string& techniqueName) const
	{
		assert(HasTechnique(techniqueName));
		return GetTechnique(m_techniquesIndexMap.at(techniqueName));
	}

	Effect::Technique& Effect::GetTechnique(const std::string& techniqueName)
	{
		assert(HasTechnique(techniqueName));
		return GetTechnique(m_techniquesIndexMap.at(techniqueName));
	}

	//-------------------------------------------------------------------------

	const Effect::Technique& Effect::GetTechnique(const unsigned techniqueIdx) const
	{
		assert(techniqueIdx < m_techniques.size());
		return *m_techniques[techniqueIdx];
	}

	Effect::Technique& Effect::GetTechnique(const unsigned techniqueIdx)
	{
		assert(techniqueIdx < m_techniques.size());
		return *m_techniques[techniqueIdx];
	}

	//-------------------------------------------------------------------------

	bool Effect::HasVariable(const std::string& variableName) const
	{
		return m_variablesIndexMap.find(variableName) != m_variablesIndexMap.end();
	}

	const Effect::Variable& Effect::GetVariable(const std::string& variableName) const
	{
		assert(HasVariable(variableName));
		return GetVariable(m_variablesIndexMap.at(variableName));
	}

	Effect::Variable& Effect::GetVariable(const std::string& variableName)
	{
		assert(HasVariable(variableName));
		return GetVariable(m_variablesIndexMap.at(variableName));
	}

	//-------------------------------------------------------------------------

	const Effect::Variable& Effect::GetVariable(const unsigned variableIdx) const
	{
		assert(variableIdx < m_variables.size());
		return *m_variables[variableIdx];
	}

	Effect::Variable& Effect::GetVariable(const unsigned variableIdx)
	{
		assert(variableIdx < m_variables.size());
		return *m_variables[variableIdx];
	}

	//-------------------------------------------------------------------------

	void Effect::Initialize()
	{
		if (m_isInitialized)
			return;

		auto hr = m_effect->GetDesc(&m_effectDesc);
		if (FAILED(hr))
		{
			throw Exception("ID3DX11Effect::GetDesc() failed.", hr);
		}

		for (unsigned i = 0; i < m_effectDesc.Techniques; i++)
		{
			auto technique = std::make_unique<Technique>(
				m_app,
				*this,
				m_effect->GetTechniqueByIndex(i)
			);

			m_techniquesIndexMap.emplace(technique->GetName(), i);
			m_techniques.push_back(std::move(technique));
		}

		for (unsigned i = 0; i < m_effectDesc.GlobalVariables; i++)
		{
			auto variable = std::make_unique<Variable>(*this, m_effect->GetVariableByIndex(i));

			m_variablesIndexMap.emplace(variable->GetName(), i);
			m_variables.push_back(std::move(variable));
		}

		m_isInitialized = true;
	}
} // namespace library
