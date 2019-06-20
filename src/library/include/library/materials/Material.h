#pragma once
#include "library/RTTI.hpp"
#include "library/NonCopyable.hpp"
#include "library/Common.h"
#include "library/PrimitiveData.h"
#include "library/VertexTypes.h"
#include "library/DirectXForwardDeclarations.h"

#include <string>
#include <map>
#include <vector>
#include <array>
#include <functional>

struct D3D11_INPUT_ELEMENT_DESC;

namespace library
{
class Model;
class Mesh;

class Effect;
class EffectVariable;
class EffectTechnique;
class EffectPass;

class Material : public NonCopyable<Material>
{
	RTTI_CLASS_BASE(Material)

public:
	explicit Material(Effect& effect, const std::string& defaultTechniqueName = "");
	virtual ~Material() = default;

	//-------------------------------------------------------------------------

	const Effect& GetEffect() const { return m_effect; }
	Effect& GetEffect() { return m_effect; }

	const EffectTechnique& GetCurrentTechnique() const { return m_currentTechnique; }
	EffectTechnique& GetCurrentTechnique() { return m_currentTechnique; }

	const EffectPass& GetCurrentPass() const { return m_currentPass; }
	EffectPass& GetCurrentPass() { return m_currentPass; }

	ID3D11InputLayout* GetCurrentInputLayout() const { return m_currentInputLayout; }

	void SetCurrentTechnique(const std::string& techniqueName, const std::string& passName = "p0");
	void SetCurrentPass(const std::string& passName);

	//-------------------------------------------------------------------------

	EffectVariable& operator[](const std::string& variableName);

	ID3D11InputLayout* GetInputLayout(const EffectPass& pass) const;

	void Initialize();
	bool IsInitialized() const { return m_initialized; }

	virtual VertexBufferData CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh) const = 0;

	PrimitiveData CreatePrimitiveData(ID3D11Device* const device, const Mesh& mesh) const;
	std::vector<PrimitiveData> CreatePrimitivesData(ID3D11Device* const device, const Model& model) const;

	virtual unsigned GetVertexSize() const = 0;

protected:
	virtual void InitializeInternal();
	virtual void CreateInputLayout(const std::string& techniqueName, const std::string& passName = "p0");
	virtual void CreateInputLayout(const EffectPass& pass);

	std::vector<D3D11_INPUT_ELEMENT_DESC> m_inputElementDescriptions;
	std::map<const EffectPass*, ComPtr<ID3D11InputLayout>> m_inputLayouts;

private:
	void CreateInputLayout();

	Effect& m_effect;
	std::reference_wrapper<EffectTechnique> m_currentTechnique;
	std::reference_wrapper<EffectPass> m_currentPass;
	ID3D11InputLayout* m_currentInputLayout = nullptr;

	std::string m_defaultTechniqueName;
	bool m_initialized = false;
};
} // namespace library
