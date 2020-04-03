#include "StdAfx.h"
#include "library/Components/GridComponent.h"

#include "library/Components/CameraComponent.h"

#include "library/Application.h"
#include "library/Utils.h"
#include "library/Render/Vertex.h"

#include "library/Effect/Effect.h"
#include "library/Effect/EffectTechnique.h"
#include "library/Effect/EffectVariable.h"
#include "library/Effect/EffectPass.h"

namespace library
{

namespace
{
constexpr unsigned k_defaultSize = 16;
constexpr unsigned k_defaultScale = 16;
constexpr auto k_defaultColor = colors::White;
} // namespace

GridComponent::GridComponent() : m_size(k_defaultSize), m_scale(k_defaultScale), m_color(k_defaultColor)
{}

GridComponent::GridComponent(const unsigned size, const unsigned scale, const math::Color& color)
	: m_size(size)
	, m_scale(scale)
	, m_color(color)
{}

GridComponent::~GridComponent() = default;

//-------------------------------------------------------------------------

void GridComponent::SetSize(const unsigned size)
{
	if (m_size != size)
	{
		m_size = size;
		Build();
	}
}

void GridComponent::SetScale(const unsigned scale)
{
	if (m_scale != scale)
	{
		m_scale = scale;
		Build();
	}
}

void GridComponent::SetColor(const math::Color& color)
{
	if (m_color != color)
	{
		m_color = color;
		Build();
	}
}

//-------------------------------------------------------------------------

void GridComponent::InitializeInternal()
{
	CreateMaterialWithEffect("Basic");
	Build();
}

//-------------------------------------------------------------------------

void GridComponent::Draw_SetData(const PrimitiveData& primitiveData)
{
	auto wvp = GetWorldMatrix();
	if (auto camera = GetCamera())
		wvp *= camera->GetViewProjectionMatrix();

	m_material->GetWorldViewProjection() << wvp;

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}

//-------------------------------------------------------------------------

void GridComponent::Build()
{
	m_primitivesData.clear();
	auto& pd = m_primitivesData.emplace_back(PrimitiveData());

	pd.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	const float adjustedScale = m_scale * 0.1f;
	const float maxPosition = m_size * adjustedScale / 2;

	const unsigned verticesCount = 4 * (m_size + 1);

	const auto size = sizeof(VertexPositionColor) * verticesCount;

	DynArray<VertexPositionColor> vertices;
	vertices.reserve(verticesCount);

	for (unsigned i = 0; i < m_size + 1; i++)
	{
		const float position = maxPosition - (i * adjustedScale);

		// vertical line
		vertices.emplace_back(math::Vector4(position, maxPosition, 0.0f, 1.0f), m_color);
		vertices.emplace_back(math::Vector4(position, -maxPosition, 0.0f, 1.0f), m_color);

		// horizontal line
		vertices.emplace_back(math::Vector4(maxPosition, position, 0.0f, 1.0f), m_color);
		vertices.emplace_back(math::Vector4(-maxPosition, position, 0.0f, 1.0f), m_color);
	}

	pd.vertexBuffer = VertexBufferData(GetApp().GetDevice(), MakeArrayBuffer(vertices));
}
} // namespace library
