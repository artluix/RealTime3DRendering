#pragma once
#include "library/Common.h"
#include "library/NonCopyable.h"
#include "library/Render/Misc.h"
#include "library/FxForward.h"

#include <d3dx11effect.h>
#include <string>

namespace library
{
class Application;
class EffectTechnique;

class EffectPass : public NonCopyable<EffectPass>
{
public:
	EffectPass(const Application& app, const EffectTechnique& technique, ID3DX11EffectPass* const pass);
	~EffectPass();

	const EffectTechnique& GetTechnique() const { return m_technique; }
	const std::string& GetName() const { return m_name; }

	ID3DX11EffectPass* GetPass() const { return m_pass; }
	const D3DX11_PASS_DESC& GetPassDesc() const { return m_passDesc; }

	ComPtr<ID3D11InputLayout> CreateInputLayout(const InputElementDescArrayBuffer& descriptionsBuffer);

	void Apply(const unsigned flags, ID3D11DeviceContext* const deviceContext);

private:
	const Application& m_app;

	ID3DX11EffectPass* m_pass;
	D3DX11_PASS_DESC m_passDesc;

	const EffectTechnique& m_technique;
	std::string m_name;
};
} // namespace library
