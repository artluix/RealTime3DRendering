#include "StdAfx.h"
#include "library/effects/EffectMaterial.h"

#include "library/effects/Effect.h"
#include "library/effects/EffectTechnique.h"
#include "library/effects/EffectVariable.h"
#include "library/effects/EffectPass.h"

#include "library/Model.h"
#include "library/Mesh.h"

#include <cassert>

namespace library
{
	EffectMaterial::EffectMaterial(Effect& effect, const std::string& defaultTechniqueName /* = ""*/)
		: m_effect(effect)
		, m_defaultTechniqueName(defaultTechniqueName)
		, m_currentTechnique(m_defaultTechniqueName.empty() ?
			m_effect.GetTechnique(0) :
			m_effect.GetTechnique(m_defaultTechniqueName)
		)
	{
	}

	EffectMaterial::~EffectMaterial() = default;

	EffectVariable& EffectMaterial::operator[](const std::string& variableName) const
	{
		return m_effect.GetVariable(variableName);
	}

	void EffectMaterial::SetCurrentTechnique(const EffectTechnique& technique)
	{
		m_currentTechnique = technique;
	}

	ID3D11InputLayout* EffectMaterial::GetInputLayout(const EffectPass& pass) const
	{
		const auto it = m_inputLayouts.find(pass);
		if (it != m_inputLayouts.cend())
			return it->second.Get();

		return nullptr;
	}

	void EffectMaterial::Initialize()
	{
	}

	void EffectMaterial::CreateInputLayout(
		const std::string& techniqueName,
		const std::string& passName,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescriptions
	)
	{
		const auto& technique = m_effect.GetTechnique(techniqueName);
		const auto& pass = technique.GetPass(passName);
		auto inputLayout = pass.CreateInputLayout(inputElementDescriptions);

		m_inputLayouts.emplace(pass, inputLayout);
	}

	std::vector<ComPtr<ID3D11Buffer>> EffectMaterial::CreateVertexBuffers(ID3D11Device* const device, const Model& model) const
	{
		std::vector<ComPtr<ID3D11Buffer>> vertexBuffers;

		const auto meshesCount = model.GetMeshesCount();
		vertexBuffers.reserve(meshesCount);

		for (unsigned i = 0; i < meshesCount; i++)
		{
			const auto& mesh = model.GetMesh(i);
			vertexBuffers.push_back(CreateVertexBuffer(device, mesh));
		}

		return vertexBuffers;
	}

} // namespace library
