#pragma once
#include "library/Common.h"
#include "library/NonCopyable.hpp"
#include "library/Path.h"

#include <d3dx11effect.h>
#include <vector>
#include <map>
#include <string>
#include <memory>

interface ID3D11Device;

namespace library
{
	class Application;
	class EffectTechnique;
	class EffectVariable;

	// ----------------------------------------------------------------------------------------------------------

	class Effect : public NonCopyable<Effect>
	{
	public:
		explicit Effect(const Application& app, const fs::Path& path);
		~Effect();

		void Compile();
		void LoadCompiled();

		const Application& GetApp() const { return m_app; }
		const fs::Path& GetPath() const { return m_path; }

		ID3DX11Effect* GetEffect() const { return m_effect.Get(); }
		void SetEffect(ID3DX11Effect* const effect);

		const D3DX11_EFFECT_DESC& GetEffectDesc() const { return m_effectDesc; }

		EffectTechnique* GetTechnique(const std::string& techniqueName) const;
		EffectTechnique* GetTechnique(const unsigned techniqueIdx) const;
		std::size_t GetTechniquesCount() const { return m_techniques.size(); }

		EffectVariable* GetVariable(const std::string& variableName) const;
		EffectVariable* GetVariable(const unsigned variableIdx) const;
		std::size_t GetVariablesCount() const { return m_variables.size(); }

	private:
		using EffectTechniquePtr = std::unique_ptr<EffectTechnique>;
		using EffectVariablePtr = std::unique_ptr<EffectVariable>;

		void Initialize();

		const Application& m_app;
		fs::Path m_path;

		ComPtr<ID3DX11Effect> m_effect;
		D3DX11_EFFECT_DESC m_effectDesc;

		std::vector<EffectTechniquePtr> m_techniques;
		std::map<std::string, EffectTechnique*> m_techniquesMap;

		std::vector<EffectVariablePtr> m_variables;
		std::map<std::string, EffectVariable*> m_variablesMap;
	};

} // namespace library