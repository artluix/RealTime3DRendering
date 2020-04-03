#include "StdAfx.h"
#include "library/Effect/Effect.h"

#include "library/Application.h"
#include "library/Utils.h"

#include "library/Effect/EffectTechnique.h"
#include "library/Effect/EffectVariable.h"

#include "library/Logger.h"

namespace library
{
std::map<std::string, std::weak_ptr<Effect>> Effect::s_effectsCache;

Effect::Effect(const Application& app, const std::string& name) : m_app(app), m_name(name), m_effectDesc{}
{}

Effect::~Effect() = default;

//-------------------------------------------------------------------------

std::shared_ptr<Effect> Effect::Create(
	const Application& app,
	const std::string& effectName,
	const bool compile /* = false */
)
{
	auto it = s_effectsCache.find(effectName);
	if (it != s_effectsCache.end())
		if (auto effect = it->second.lock())
			return effect;

	auto effect = std::shared_ptr<Effect>(new Effect(app, effectName));
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

	auto hr = D3DX11CompileEffectFromFile(
		m_path.GetWideCString(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
#if defined(DEBUG) || defined(DEBUG)
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
#else
		0,
#endif
		0,
		m_app.GetDevice(),
		&m_effect,
		&errorBlob
	);
	if (FAILED(hr))
	{
		Logger::Log(Logger::Level::Error, static_cast<const char*>(errorBlob->GetBufferPointer()));
		assert("D3DX11CompileEffectFromFile() failed" && false);
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

	DynArray<std::byte> effectData;
	utils::LoadBinaryFile(m_path, effectData);
	assert("Load compiled effect failed." && !effectData.empty());

	auto hr = D3DX11CreateEffectFromMemory(
		effectData.data(), effectData.size(),
		0,
		m_app.GetDevice(),
		&m_effect
	);
	assert("D3DX11CreateEffectFromMemory() failed." && SUCCEEDED(hr));

	Initialize();
}

//-------------------------------------------------------------------------

void Effect::SetEffect(ComPtr<ID3DX11Effect>&& effect)
{
	m_techniquesMapping.clear();
	m_techniques.clear();

	m_variablesMapping.clear();
	m_variables.clear();

	m_initialized = false;

	m_effect = std::move(effect);

	Initialize();
}

//-------------------------------------------------------------------------

bool Effect::HasTechnique(const std::string& techniqueName) const
{
	if (m_techniquesMapping.find(techniqueName) == m_techniquesMapping.end())
	{
		Logger::Error("Effect: %s\nTechnique not found: %s", m_name.c_str(), techniqueName.c_str());
		return false;
	}

	return true;
}

unsigned Effect::GetTechniqueIdx(const std::string& techniqueName) const
{
	assert(HasTechnique(techniqueName));
	return m_techniquesMapping.at(techniqueName);
}

//-------------------------------------------------------------------------

const Effect::Technique& Effect::GetTechnique(const std::string& techniqueName) const
{
	return GetTechnique(GetTechniqueIdx(techniqueName));
}

Effect::Technique& Effect::GetTechnique(const std::string& techniqueName)
{
	return GetTechnique(GetTechniqueIdx(techniqueName));
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
	return m_variablesMapping.find(variableName) != m_variablesMapping.end();
}

unsigned Effect::GetVariableIdx(const std::string& variableName) const
{
	assert(HasVariable(variableName));
	return m_variablesMapping.at(variableName);
}

const Effect::Variable& Effect::GetVariable(const std::string& variableName) const
{
	return GetVariable(GetVariableIdx(variableName));
}

Effect::Variable& Effect::GetVariable(const std::string& variableName)
{
	return GetVariable(GetVariableIdx(variableName));
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
	if (m_initialized)
		return;

	auto hr = m_effect->GetDesc(&m_effectDesc);
	assert("ID3DX11Effect::GetDesc() failed." && SUCCEEDED(hr));

	for (unsigned i = 0; i < m_effectDesc.Techniques; i++)
	{
		auto technique = std::make_unique<Technique>(m_app, *this, m_effect->GetTechniqueByIndex(i));

		m_techniquesMapping.emplace(technique->GetName(), i);
		m_techniques.push_back(std::move(technique));
	}

	for (unsigned i = 0; i < m_effectDesc.GlobalVariables; i++)
	{
		auto variable = std::make_unique<Variable>(*this, m_effect->GetVariableByIndex(i));

		m_variablesMapping.emplace(variable->GetName(), i);
		m_variables.push_back(std::move(variable));
	}

	m_initialized = true;
}
} // namespace library
