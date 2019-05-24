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
class EffectPass;

class Effect : public NonCopyable<Effect>
{
public:
	using Technique = EffectTechnique;
	using Variable = EffectVariable;
	using Pass = EffectPass;

	~Effect();

	static std::shared_ptr<Effect> Create(
		const Application& app, const std::string& effectName, const bool compile = false);
	static void ClearAll();

	const Application& GetApp() const { return m_app; }
	const std::string& GetName() const { return m_name; }
	const Path& GetPath() const { return m_path; }

	ID3DX11Effect* GetEffect() const { return m_effect.Get(); }
	void SetEffect(ComPtr<ID3DX11Effect>&& effect);

	const D3DX11_EFFECT_DESC& GetEffectDesc() const { return m_effectDesc; }

	//-------------------------------------------------------------------------

	bool HasTechnique(const std::string& techniqueName) const;
	unsigned GetTechniquesCount() const { return static_cast<unsigned>(m_techniques.size()); }

	unsigned GetTechniqueIdx(const std::string& techniqueName) const;
	const Technique& GetTechnique(const std::string& techniqueName) const;
	Technique& GetTechnique(const std::string& techniqueName);

	const Technique& GetTechnique(const unsigned techniqueIdx) const;
	Technique& GetTechnique(const unsigned techniqueIdx);

	//-------------------------------------------------------------------------

	bool HasVariable(const std::string& variableName) const;
	std::size_t GetVariablesCount() const { return m_variables.size(); }

	unsigned GetVariableIdx(const std::string& variableName) const;
	const Variable& GetVariable(const std::string& variableName) const;
	Variable& GetVariable(const std::string& variableName);

	const Variable& GetVariable(const unsigned variableIdx) const;
	Variable& GetVariable(const unsigned variableIdx);

	//-------------------------------------------------------------------------

	bool IsInitialized() const { return m_initialized; }

private:
	using TechniquePtr = std::unique_ptr<Technique>;
	using VariablePtr = std::unique_ptr<Variable>;

	Effect(const Application& app, const std::string& name);

	void Compile();
	void LoadCompiled();

	void Initialize();

	static std::map<std::string, std::weak_ptr<Effect>> s_effectsCache;

	const Application& m_app;
	Path m_path;
	std::string m_name;

	ComPtr<ID3DX11Effect> m_effect;
	D3DX11_EFFECT_DESC m_effectDesc;

	bool m_initialized = false;

	std::vector<TechniquePtr> m_techniques;
	std::map<std::string, unsigned> m_techniquesMapping;

	std::vector<VariablePtr> m_variables;
	std::map<std::string, unsigned> m_variablesMapping;
};
} // namespace library
