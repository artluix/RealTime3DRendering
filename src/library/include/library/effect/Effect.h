#pragma once
#include "library/CommonTypes.h"
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

	class Effect;
	using EffectPtr = std::shared_ptr<Effect>;

	class Effect : public NonCopyable<Effect>
	{
	public:
		explicit Effect(const Application& app, const Path& path);
		~Effect();

		static EffectPtr Create(const Application& app, const Path& path);
		static void ClearAll();

		void Compile();
		void LoadCompiled();

		const Application& GetApp() const { return m_app; }
		const Path& GetPath() const { return m_path; }
		const std::string& GetName() const { return m_name; }

		ID3DX11Effect* GetEffect() const { return m_effect.Get(); }
		void SetEffect(ID3DX11Effect* const effect);

		const D3DX11_EFFECT_DESC& GetEffectDesc() const { return m_effectDesc; }

		bool HasTechnique(const std::string& techniqueName) const;
		EffectTechnique& GetTechnique(const std::string& techniqueName) const;
		EffectTechnique& GetTechnique(const unsigned techniqueIdx) const;
		std::size_t GetTechniquesCount() const { return m_techniques.size(); }

		bool HasVariable(const std::string& variableName) const;
		EffectVariable& GetVariable(const std::string& variableName) const;
		EffectVariable& GetVariable(const unsigned variableIdx) const;
		std::size_t GetVariablesCount() const { return m_variables.size(); }

		bool IsInitialized() const { return m_isInitialized; }

	private:
		using EffectTechniquePtr = std::unique_ptr<EffectTechnique>;
		using EffectVariablePtr = std::unique_ptr<EffectVariable>;

		void Initialize();

		const Application& m_app;
		Path m_path;
		std::string m_name;

		ComPtr<ID3DX11Effect> m_effect;
		D3DX11_EFFECT_DESC m_effectDesc;

		bool m_isInitialized = false;

		std::vector<EffectTechniquePtr> m_techniques;
		std::map<std::string, EffectTechnique*> m_techniquesMap;

		std::vector<EffectVariablePtr> m_variables;
		std::map<std::string, EffectVariable*> m_variablesMap;

		static std::map<std::string, EffectPtr> s_effects;
	};
} // namespace library
