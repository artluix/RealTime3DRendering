#include "StdAfx.h"
#include "library/Components/FullScreenQuadComponent.h"

#include "library/Effect/Effect.h"
#include "library/Effect/EffectTechnique.h"
#include "library/Effect/EffectPass.h"

#include "library/Materials/Material.h"

#include "library/Render/Vertex.h"
#include "library/Application.h"

#include <cassert>

namespace library
{

namespace
{
constexpr unsigned k_verticesCount = 4;
} // namespace

FullScreenQuadComponent::FullScreenQuadComponent()
{
	m_primitivesData.resize(1);
}

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
	// default initialization of vertex buffer
	if (!m_primitivesData[0].vertexBuffer)
	{
		using Vertex = VertexPositionTexture;

		constexpr auto vertices = MakeArray(
			Vertex(math::Vector4(-1.0f, -1.0f, 0.0f, 1.0f), math::Vector2(0.0f, 1.0f)),
			Vertex(math::Vector4(-1.0f, 1.0f, 0.0f, 1.0f), math::Vector2(0.0f, 0.0f)),
			Vertex(math::Vector4(1.0f, 1.0f, 0.0f, 1.0f), math::Vector2(1.0f, 0.0f)),
			Vertex(math::Vector4(1.0f, -1.0f, 0.0f, 1.0f), math::Vector2(1.0f, 1.0f))
		);
		assert(vertices.size() == k_verticesCount);

		m_primitivesData[0].vertexBuffer = VertexBufferData(GetApp().GetDevice(), MakeArrayBuffer(vertices));
	}

	// index buffer
	{
		constexpr auto indices = MakeArray(
			0, 1, 2,
			0, 2, 3
		);
		m_primitivesData[0].indexBuffer = IndexBufferData(GetApp().GetDevice(), MakeArrayBuffer(indices));
	}
}

void FullScreenQuadComponent::SetVertexBufferData(VertexBufferData&& vbd)
{
	assert(!!vbd.buffer && vbd.elementsCount == k_verticesCount);
	m_primitivesData[0].vertexBuffer = std::move(vbd);
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
