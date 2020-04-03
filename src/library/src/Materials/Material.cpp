#include "StdAfx.h"
#include "library/Materials/Material.h"

#include "library/Effect/Effect.h"
#include "library/Effect/EffectTechnique.h"
#include "library/Effect/EffectVariable.h"
#include "library/Effect/EffectPass.h"

namespace library
{
Material::Material(Effect& effect, const std::string& defaultTechniqueName /* = "" */)
	: m_effect(effect)
	, m_defaultTechniqueName(defaultTechniqueName)
	, m_currentTechnique(
		!defaultTechniqueName.empty() ? effect.GetTechnique(defaultTechniqueName)
									  : effect.GetTechnique(0)
	)
	, m_currentPass(m_currentTechnique.get().GetPass(0))
{
	assert(effect.IsInitialized());
}

//-------------------------------------------------------------------------


void Material::SetCurrentTechnique(const std::string& techniqueName, const std::string& passName /*= "p0"*/)
{
	m_currentTechnique = m_effect.GetTechnique(techniqueName);
	SetCurrentPass(passName);
}

void Material::SetCurrentPass(const std::string& passName)
{
	m_currentPass = m_currentTechnique.get().GetPass(passName);
	m_currentInputLayout = GetInputLayout(m_currentPass);
}

//-------------------------------------------------------------------------

EffectVariable& Material::operator[](const std::string& variableName)
{
	return m_effect.GetVariable(variableName);
}

//-------------------------------------------------------------------------

ID3D11InputLayout* Material::GetInputLayout(const EffectPass& pass) const
{
	const auto it = m_inputLayouts.find(&pass);
	if (it != m_inputLayouts.cend())
		return it->second.Get();

	return nullptr;
}

//-------------------------------------------------------------------------

void Material::Initialize()
{
	if (m_initialized)
		return;

	InitializeInternal();

	m_currentInputLayout = GetInputLayout(m_currentPass); // we must set it here

	m_initialized = true;
}

//-------------------------------------------------------------------------

void Material::CreateInputLayout(
	const InputElementDescArrayBuffer& descriptionsBuffer,
	const std::string& techniqueName,
	const std::string& passName /*= "p0"*/
)
{
	auto& technique = m_effect.GetTechnique(techniqueName);
	auto& pass = technique.GetPass(passName);

	CreateInputLayout(descriptionsBuffer, pass);
}

void Material::CreateInputLayout(const InputElementDescArrayBuffer& descriptionsBuffer, EffectPass& pass)
{
	auto inputLayout = pass.CreateInputLayout(descriptionsBuffer);
	m_inputLayouts.emplace(&pass, std::move(inputLayout));
}

} // namespace library
