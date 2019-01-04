#include "StdAfx.h"
#include "library/effect/EffectTechnique.h"

#include "library/Application.h"
#include "library/effect/EffectPass.h"
#include "library/effect/Effect.h"

namespace library
{
	EffectTechnique::EffectTechnique(const Application& app, const Effect& effect, const ComPtr<ID3DX11EffectTechnique>& technique)
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

	bool EffectTechnique::HasPass(const std::string& passName) const
	{
		return m_passesMap.find(passName) != m_passesMap.end();
	}

	EffectPass& EffectTechnique::GetPass(const std::string& passName) const
	{
		return *m_passesMap.at(passName);
	}

	EffectPass& EffectTechnique::GetPass(const unsigned passIdx) const
	{
		return *m_passes.at(passIdx);
	}

} // namespace library
