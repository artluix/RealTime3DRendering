#include "StdAfx.h"
#include "library/Components/CubeComponent.h"

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
constexpr unsigned k_verticesCount = 8;
} // namespace

//-------------------------------------------------------------------------

CubeComponent::CubeComponent()
{
	m_primitivesData.resize(1);
}

CubeComponent::~CubeComponent() = default;

//-------------------------------------------------------------------------

void CubeComponent::SetMaterial(Material& material)
{
	m_material = &material;
}

void CubeComponent::SetMaterial(
	Material& material,
	const std::string& techniqueName,
	const std::string& passName /* = "p0"*/
)
{
	SetMaterial(material);
	SetActiveTechnique(techniqueName, passName);
}

//-------------------------------------------------------------------------

void CubeComponent::SetActiveTechnique(
	const std::string& techniqueName,
	const std::string& passName /* = "p0"*/
)
{
	assert(!!m_material);

	auto& technique = m_material->GetEffect().GetTechnique(techniqueName);

	m_pass = &technique.GetPass(passName);
	m_inputLayout = m_material->GetInputLayout(*m_pass);
}

void CubeComponent::SetMaterialUpdateFunction(const MaterialUpdateFunction& func)
{
	m_materialUpdateFunction = func;
}

void CubeComponent::InitializeInternal()
{
	// default initialization of vertex buffer
	if (!m_primitivesData[0].vertexBuffer)
	{
		using Vertex = VertexPosition;
		using math::Vector4;

		constexpr auto vertices = MakeArray(
			// bottom
			Vertex(Vector4(-1.0f, -1.0f, -1.0f, 1.0f)),
			Vertex(Vector4(-1.0f, 1.0f, -1.0f, 1.0f)),
			Vertex(Vector4(1.0f, 1.0f, -1.0f, 1.0f)),
			Vertex(Vector4(1.0f, -1.0f, -1.0f, 1.0f)),

			// top
			Vertex(Vector4(-1.0f, -1.0f, 1.0f, 1.0f)),
			Vertex(Vector4(-1.0f, 1.0f, 1.0f, 1.0f)),
			Vertex(Vector4(1.0f, 1.0f, 1.0f, 1.0f)),
			Vertex(Vector4(1.0f, -1.0f, 1.0f, 1.0f))
		);
		assert(vertices.size() == k_verticesCount);

		m_primitivesData[0].vertexBuffer = VertexBufferData(GetApp().GetDevice(), MakeArrayBuffer(vertices));
	}

	// index buffer
	{
		constexpr auto indices = MakeArray(
			0, 1, 2,
			0, 2, 3,

			4, 5, 6,
			4, 6, 7,

			0, 4, 5,
			0, 5, 1,

			3, 7, 6,
			3, 6, 2,

			0, 4, 7,
			0, 7, 3,

			1, 5, 6,
			1, 6, 2
		);
		m_primitivesData[0].indexBuffer = IndexBufferData(GetApp().GetDevice(), MakeArrayBuffer(indices));
	}
}

void CubeComponent::SetVertexBufferData(VertexBufferData&& vbd)
{
	assert(!!vbd.buffer && vbd.elementsCount == k_verticesCount);
	m_primitivesData[0].vertexBuffer = std::move(vbd);
}

void CubeComponent::Update(const Time& time)
{}

void CubeComponent::Draw_SetData(const PrimitiveData& primitiveData)
{
	if (!!m_materialUpdateFunction)
		m_materialUpdateFunction();

	m_pass->Apply(0, GetApp().GetDeviceContext());
}
} // namespace library
