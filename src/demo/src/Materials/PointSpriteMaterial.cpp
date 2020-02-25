#include "Materials/PointSpriteMaterial.h"

#include <library/Effect/Effect.h>
#include "library/Effect/EffectTechnique.h"

using namespace library;

PointSpriteMaterial::PointSpriteMaterial(Effect& effect)
	: Material(effect, "main11")

	, m_cameraPosition(effect.GetVariable("CameraPosition"))
	, m_cameraUp(effect.GetVariable("CameraUp"))

	, m_viewProjection(effect.GetVariable("ViewProjection"))

	, m_colorTexture(effect.GetVariable("ColorTexture"))
{}

//-------------------------------------------------------------------------

VertexBufferData PointSpriteMaterial::CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh) const
{
	assert("This method is unsupported for model meshes." && false);
	return VertexBufferData();
}

void PointSpriteMaterial::InitializeInternal()
{
	EffectPass::InputElementDescArray inputElementDescriptions =
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

	for (unsigned i = 0, techniquesCount = GetEffect().GetTechniquesCount(); i < techniquesCount; i++)
	{
		auto& technique = GetEffect().GetTechnique(i);

		for (unsigned j = 0, passesCount = technique.GetPassesCount(); j < passesCount; j++)
		{
			auto& pass = technique.GetPass(j);
			CreateInputLayout(inputElementDescriptions, pass);
		}
	}
}
