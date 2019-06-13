#include "StdAfx.h"
#include "library/Components/FullScreenQuadComponent.h"

#include "library/Effect/Effect.h"
#include "library/Effect/EffectTechnique.h"
#include "library/Effect/EffectPass.h"

#include "library/Materials/Material.h"

#include "library/VertexTypes.h"
#include "library/Application.h"

#include <array>
#include <cassert>

namespace library
{
FullScreenQuadComponent::FullScreenQuadComponent() = default;
FullScreenQuadComponent::~FullScreenQuadComponent() = default;

//-------------------------------------------------------------------------

void FullScreenQuadComponent::SetMaterial(Material& material)
{
	m_material = &material;
}

void FullScreenQuadComponent::SetMaterial(
	Material& material,
	const std::string& techniqueName,
	const std::string& passName /* = "p0"*/
)
{
	SetMaterial(material);
	SetActiveTechnique(techniqueName, passName);
}

//-------------------------------------------------------------------------

void FullScreenQuadComponent::SetActiveTechnique(
	const std::string& techniqueName,
	const std::string& passName /* = "p0"*/
)
{
	assert(!!m_material);

	auto& technique = m_material->GetEffect().GetTechnique(techniqueName);

	m_pass = &technique.GetPass(passName);
	m_inputLayout = m_material->GetInputLayout(*m_pass);
}

void FullScreenQuadComponent::SetMaterialUpdateFunction(const MaterialUpdateFunction& func)
{
	m_materialUpdateFunction = func;
}

void FullScreenQuadComponent::InitializeInternal()
{
	m_primitivesData.clear();
	auto& pd = m_primitivesData.emplace_back(PrimitiveData());

	// vertex buffer
	{
		using Vertex = VertexPositionTexture;

		const std::array<Vertex, 4> vertices = {
			Vertex(math::Vector4(-1.0f, -1.0f, 0.0f, 1.0f), math::Vector2(0.0f, 1.0f)),
			Vertex(math::Vector4(-1.0f, 1.0f, 0.0f, 1.0f), math::Vector2(0.0f, 0.0f)),
			Vertex(math::Vector4(1.0f, 1.0f, 0.0f, 1.0f), math::Vector2(1.0f, 0.0f)),
			Vertex(math::Vector4(1.0f, -1.0f, 0.0f, 1.0f), math::Vector2(1.0f, 1.0f)),
		};

		pd.vertexBuffer = VertexBufferData(GetApp().GetDevice(), vertices);
		pd.stride = sizeof(Vertex);
	}

	// index buffer
	{
		const std::array<unsigned, 6> indices = {0, 1, 2, 0, 2, 3};
		pd.indexBuffer = IndexBufferData(GetApp().GetDevice(), indices);
	}
}

void FullScreenQuadComponent::Update(const Time& time)
{}

void FullScreenQuadComponent::Draw_SetData(const PrimitiveData& primitiveData)
{
	if (!!m_materialUpdateFunction)
		m_materialUpdateFunction();

	m_pass->Apply(0, GetApp().GetDeviceContext());
}
} // namespace library
