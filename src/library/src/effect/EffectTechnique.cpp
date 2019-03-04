#include "StdAfx.h"
#include "library/Effect/EffectTechnique.h"

#include "library/Effect/EffectPass.h"
#include "library/Effect/Effect.h"

#include "library/Application.h"

#include <cassert>

namespace library
{
	EffectTechnique::EffectTechnique(
		const Application& app,
		const Effect& effect,
		ID3DX11EffectTechnique* const technique
	)
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

	//-------------------------------------------------------------------------

	bool EffectTechnique::HasPass(const std::string& passName) const
	{
		return m_passesMap.find(passName) != m_passesMap.end();
	}

	EffectPass& EffectTechnique::GetPass(const std::string& passName) const
	{
		assert(HasPass(passName));
		return *m_passesMap.at(passName);
	}

	EffectPass& EffectTechnique::GetPass(const unsigned passIdx) const
	{
		assert(passIdx < m_passes.size());
		return *m_passes[passIdx];
	}
} // namespace library
