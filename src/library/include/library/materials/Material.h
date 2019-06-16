#pragma once
#include "library/RTTI.hpp"
#include "library/NonCopyable.hpp"
#include "library/Common.h"
#include "library/PrimitiveData.h"
#include "library/DirectXForwardDeclarations.h"

#include <string>
#include <map>
#include <vector>
#include <array>
#include <memory>
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
	explicit Material(std::shared_ptr<Effect> effect, const std::string& defaultTechniqueName = "");
	virtual ~Material() = default;

	//-------------------------------------------------------------------------

	EffectVariable& operator[](const std::string& variableName);

	const Effect& GetEffect() const { return *m_effect; }
	Effect& GetEffect() { return *m_effect; }

	const EffectTechnique& GetCurrentTechnique() const { return m_currentTechnique; }
	EffectTechnique& GetCurrentTechnique() { return m_currentTechnique; }
	void SetCurrentTechnique(const std::string& techniqueName);
	void SetCurrentTechnique(EffectTechnique& technique);

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

	std::shared_ptr<Effect> m_effect;
	std::reference_wrapper<EffectTechnique> m_currentTechnique;

	std::string m_defaultTechniqueName;
	bool m_initialized = false;
};
} // namespace library
