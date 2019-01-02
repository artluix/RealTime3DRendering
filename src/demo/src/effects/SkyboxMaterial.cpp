#include "effects/SkyboxMaterial.h"

#include <library/effects/Effect.h>
#include <library/Mesh.h>
#include <library/Color.h>
#include <library/Exception.h>

namespace demo
{
	using namespace library;

	SkyboxMaterial::SkyboxMaterial(library::Effect& effect)
		: Class(effect, "main11")
	{
	}

	void SkyboxMaterial::Initialize()
	{

	}

	ComPtr<ID3D11Buffer> SkyboxMaterial::CreateVertexBuffer(ID3D11Device* const device, const library::Mesh& mesh) const
	{
		return ComPtr<ID3D11Buffer>();
	}

	ComPtr<ID3D11Buffer> SkyboxMaterial::CreateVertexBuffer(ID3D11Device* const device, const Vertices& vertices) const
	{
		return ComPtr<ID3D11Buffer>();
	}

} // namespace demo
