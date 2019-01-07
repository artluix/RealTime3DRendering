#include "StdAfx.h"
#include "library/effect/EffectTechnique.h"

#include "library/Application.h"
#include "library/effect/EffectPass.h"
#include "library/effect/Effect.h"

namespace library
{
	EffectTechnique::EffectTechnique(const Application& app, const Effect& effect, ID3DX11EffectTechnique* const technique)
		: m_effect(effect)
		, m_technique(technique)
	{
		m_technique->GetDesc(&m_techniqueDesc);
		m_name = m_techniqueDesc.Name;

		for (unsigned i = 0; i < m_techniqueDesc.Passes; i++)
		{
			auto pass = std::make_unique<EffectPass>(app, *this, m_technique->GetPassByIndex(i));
			m_passesMap.emplace(pass->GetName(), pass.get());
			m_passes.push_back(std::move(pass));
		}
	}

	EffectTechnique::~EffectTechnique() = default;

	EffectPass* EffectTechnique::GetPass(const std::string& passName) const
	{
		auto it = m_passesMap.find(passName);
		if (it != m_passesMap.end())
			return nullptr;

		return it->second;
	}

	EffectPass* EffectTechnique::GetPass(const unsigned passIdx) const
	{
		if (passIdx >= m_passes.size())
			return nullptr;

		return m_passes[passIdx].get();
	}

} // namespace library
