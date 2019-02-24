#include "StdAfx.h"
#include "library/effect/Effect.h"

#include "library/Application.h"
#include "library/Utils.h"
#include "library/Exception.h"

#include "library/effect/EffectTechnique.h"
#include "library/effect/EffectVariable.h"

namespace library
{
	std::map<std::string, std::weak_ptr<Effect>> Effect::s_effects;

	Effect::Effect(const Application& app, const std::string& name)
		: m_app(app)
		, m_name(name)
	{
	}

	EffectPtr Effect::Create(
		const Application& app,
		const std::string& effectName,
		const bool compile /* = false */
	)
	{
		auto it = s_effects.find(effectName);
		if (it != s_effects.end())
			if (auto effect = it->second.lock())
					return effect;

		auto effect = EffectPtr(new Effect(app, effectName));
		s_effects.emplace(effectName, effect);

		if (compile)
			effect->Compile();
		else
			effect->LoadCompiled();

		return effect;
	}

	Effect::~Effect() = default;

	void Effect::ClearAll()
	{
		s_effects.clear();
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

		std::vector<library::byte> effectData;
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
		if (m_effect != effect)
			m_effect = effect;
	}

	//-------------------------------------------------------------------------

	bool Effect::HasTechnique(const std::string& techniqueName) const
	{
		return m_techniquesMap.find(techniqueName) != m_techniquesMap.end();
	}

	EffectTechnique& Effect::GetTechnique(const std::string& techniqueName) const
	{
		assert(HasTechnique(techniqueName));
		return *m_techniquesMap.at(techniqueName);
	}

	EffectTechnique& Effect::GetTechnique(const unsigned techniqueIdx) const
	{
		assert(techniqueIdx < m_techniques.size());
		return *m_techniques[techniqueIdx];
	}

	//-------------------------------------------------------------------------

	bool Effect::HasVariable(const std::string& variableName) const
	{
		return m_variablesMap.find(variableName) != m_variablesMap.end();
	}

	EffectVariable& Effect::GetVariable(const std::string& variableName) const
	{
		assert(HasVariable(variableName));
		return *m_variablesMap.at(variableName);
	}

	EffectVariable& Effect::GetVariable(const unsigned variableIdx) const
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
			auto technique = std::make_unique<EffectTechnique>(
				m_app,
				*this,
				m_effect->GetTechniqueByIndex(i)
			);
			m_techniquesMap.emplace(technique->GetName(), technique.get());
			m_techniques.push_back(std::move(technique));
		}

		for (unsigned i = 0; i < m_effectDesc.GlobalVariables; i++)
		{
			auto variable = std::make_unique<EffectVariable>(*this, m_effect->GetVariableByIndex(i));
			m_variablesMap.emplace(variable->GetName(), variable.get());
			m_variables.push_back(std::move(variable));
		}

		m_isInitialized = true;
	}
} // namespace library
