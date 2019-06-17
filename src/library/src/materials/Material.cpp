#include "StdAfx.h"
#include "library/Materials/Material.h"

#include "library/Effect/Effect.h"
#include "library/Effect/EffectTechnique.h"
#include "library/Effect/EffectVariable.h"
#include "library/Effect/EffectPass.h"

#include "library/Model/Model.h"
#include "library/Model/Mesh.h"

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

void Material::CreateInputLayout(const std::string& techniqueName, const std::string& passName /*= "p0"*/)
{
	const auto& technique = m_effect.GetTechnique(techniqueName);
	const auto& pass = technique.GetPass(passName);
	auto inputLayout = pass.CreateInputLayout(m_inputElementDescriptions);

	m_inputLayouts.emplace(&pass, std::move(inputLayout));
}

void Material::CreateInputLayout(const EffectPass& pass)
{
	auto inputLayout = pass.CreateInputLayout(m_inputElementDescriptions);
	m_inputLayouts.emplace(&pass, std::move(inputLayout));
}

void Material::CreateInputLayout()
{
	CreateInputLayout(m_defaultTechniqueName);
}

//-------------------------------------------------------------------------

void Material::InitializeInternal()
{
	assert(!m_defaultTechniqueName.empty());
	assert(!m_inputElementDescriptions.empty());

	CreateInputLayout();
}

//-------------------------------------------------------------------------

PrimitiveData Material::CreatePrimitiveData(ID3D11Device* const device, const Mesh& mesh) const
{
	return PrimitiveData(
		GetVertexSize(),
		mesh.GetPrimitiveTopology(),
		CreateVertexBufferData(device, mesh),
		mesh.CreateIndexBufferData()
	);
}

std::vector<PrimitiveData> Material::CreatePrimitivesData(ID3D11Device* const device, const Model& model) const
{
	std::vector<PrimitiveData> primitivesData;

	const auto meshesCount = model.GetMeshesCount();
	primitivesData.reserve(meshesCount);

	for (unsigned i = 0; i < meshesCount; i++)
	{
		const auto& mesh = model.GetMesh(i);
		primitivesData.push_back(CreatePrimitiveData(device, mesh));
	}

	return primitivesData;
}
} // namespace library
