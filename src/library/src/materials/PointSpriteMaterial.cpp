#include "StdAfx.h"
#include "library/Materials/PointSpriteMaterial.h"

#include "library/Effect/Effect.h"
#include "library/Effect/EffectTechnique.h"
#include "library/Effect/EffectPass.h"

#include "library/Exception.h"

namespace library
{
PointSpriteMaterial::PointSpriteMaterial(std::shared_ptr<Effect> effect)
	: Material(effect, "main11")

	, m_cameraPosition(effect->GetVariable("k_cameraPosition"))
	, m_cameraUp(effect->GetVariable("k_cameraUp"))

	, m_viewProjection(effect->GetVariable("k_viewProjection"))

	, m_colorTexture(effect->GetVariable("k_colorTexture"))
{}

//-------------------------------------------------------------------------

VertexBufferData PointSpriteMaterial::CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh) const
{
	throw Exception("This method is unsupported for model meshes.");
}

void PointSpriteMaterial::InitializeInternal()
{
	// clang-format off
	m_inputElementDescriptions =
	{
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"SIZE",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
	};
	// clang-format on

	for (unsigned i = 0, techniquesCount = GetEffect().GetTechniquesCount(); i < techniquesCount; i++)
	{
		const auto& technique = GetEffect().GetTechnique(i);

		for (unsigned j = 0, passesCount = technique.GetPassesCount(); j < passesCount; j++)
		{
			const auto& pass = technique.GetPass(j);
			CreateInputLayout(pass);
		}
	}
}
} // namespace library
