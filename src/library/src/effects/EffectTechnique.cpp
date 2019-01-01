#include "StdAfx.h"
#include "library/effects/EffectTechnique.h"

#include "library/Application.h"
#include "library/effects/EffectPass.h"
#include "library/effects/Effect.h"

namespace library
{
	EffectTechnique::EffectTechnique(const Application& app, Effect& effect, const ComPtr<ID3DX11EffectTechnique>& technique)
		: m_effect(effect)
		, m_technique(technique)
	{
		m_technique->GetDesc(&m_techniqueDesc);
		m_name = m_techniqueDesc.Name;

		for (unsigned i = 0; i < m_techniqueDesc.Passes; i++)
		{
			auto pass = std::make_shared<EffectPass>(app, *this, m_technique->GetPassByIndex(i));
			m_passes.push_back(pass);
			m_passesMap.insert(std::make_pair(pass->GetName(), pass));
		}
	}

	EffectPass* const EffectTechnique::GetPass(const std::string& passName) const
	{
		auto it = m_passesMap.find(passName);
		if (it != m_passesMap.end())
			return it->second.get();

		return nullptr;
	}

	EffectPass* const EffectTechnique::GetPass(const unsigned passIdx) const
	{
		return m_passes[passIdx].get();
	}

} // namespace library
