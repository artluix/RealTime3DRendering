#include "BasicTessellationDemo.h"

#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>
#include <library/Components/TextComponent.h>

#include <library/Application.h>
#include <library/RasterizerStates.h>

#include <library/Math/Math.h>

#include <library/Effect/EffectVariable.h>

#include <sstream>
#include <iomanip>

using namespace library;

namespace
{
constexpr float k_maxTessellationFactor = 64.f;

constexpr float ClampTessellation(const float t)
{
	return math::Clamp(t, 1.f, k_maxTessellationFactor);
}
}

BasicTessellationDemo::BasicTessellationDemo()
	: m_topology(TopologyType::Triangle)
{
}

//-------------------------------------------------------------------------

const TessellationMaterial* BasicTessellationDemo::GetMaterial() const
{
	switch (m_topology)
	{
		case TopologyType::Triangle:
			return m_material.get();

		case TopologyType::Quad:
			return m_quadMaterial.get();

		default:
			assert("Undefined Topology" && false);
			return nullptr;
	}
}

TessellationMaterial* BasicTessellationDemo::GetMaterial()
{
	switch (m_topology)
	{
		case TopologyType::Triangle:
			return m_material.get();

		case TopologyType::Quad:
			return m_quadMaterial.get();

		default:
			assert("Undefined Topology" && false);
			return nullptr;
	}
}

//-------------------------------------------------------------------------

void BasicTessellationDemo::InitializeInternal()
{
	// triangle
	CreateMaterialWithEffect("TriangleTessellation");

	// quad
	{
		m_quadEffect = Effect::Create(GetApp(), "QuadTessellation");
		m_quadMaterial = std::make_unique<TessellationMaterial>(*m_quadEffect);
		m_quadMaterial->Initialize();
	}

	// fill primitives data
	{
		using Vertex = VertexPosition;

		m_primitivesData.resize(Topology::Count);

		// triangle
		{
			auto& trianglePrimitive = m_primitivesData[Topology::Triangle];

			constexpr std::array<Vertex, 3> triangleVertices =
			{
				Vertex(math::Vector4(-5.f, 1.f, 0.f, 1.f)),
				Vertex(math::Vector4(0.f, 6.f, 0.f, 1.f)),
				Vertex(math::Vector4(5.f, 1.f, 0.f, 1.f))
			};

			trianglePrimitive.vertexBuffer = VertexBufferData(GetApp().GetDevice(), triangleVertices);
			trianglePrimitive.stride = sizeof(Vertex);
			trianglePrimitive.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST;
		}

		// quad
		{
			auto& quadPrimitive = m_primitivesData[Topology::Quad];

			constexpr std::array<Vertex, 4> quadVertices =
			{
				Vertex(math::Vector4(-5.f, 6.f, 0.f, 1.f)),
				Vertex(math::Vector4(5.f, 6.f, 0.f, 1.f)),
				Vertex(math::Vector4(-5.f, 1.f, 0.f, 1.f)),
				Vertex(math::Vector4(5.f, 1.f, 0.f, 1.f))
			};

			quadPrimitive.vertexBuffer = VertexBufferData(GetApp().GetDevice(), quadVertices);
			quadPrimitive.stride = sizeof(Vertex);
			quadPrimitive.primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST;
		}
	}

	m_tessEdgeFactors.quad.fill(2);
	m_tessInsideFactors.quad.fill(2);

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction(
		[this]() -> std::wstring
		{
			std::wostringstream woss;

			const bool isTriangleTopology = (m_topology == TopologyType::Triangle);
			woss << "Topology (T): " << (isTriangleTopology ? "Triangle" : "Quad") <<
				"\nUniform Tessellation (Space): " << (m_isUniformTessellation ? "True" : "False");

			if (m_isUniformTessellation)
			{
				woss << "\nTessellationEdgeFactors (+Up/-Down): " << DumpEdgeFactors() <<
						"\nTessellationInsideFactors: " << DumpInsideFactors();
			}
			else
			{
				if (isTriangleTopology)
					woss << "\nTessellation Edge Factors (+U/-H, +I/-J, +O/-K): ";
				else
					woss << "\nTessellation Edge Factors (+U/-H, +I/-J, +O/-K, +P/-L): ";

				woss << DumpEdgeFactors();

				if (isTriangleTopology)
					woss << "\nTessellation Inside Factor: (+V/-B): ";
				else
					woss << "\nTessellation Inside Factors: (+V/-B, +N/-M): ";

				woss << DumpInsideFactors();
			}

			return woss.str();
		}
	);
	m_text->Initialize(GetApp());
}

void BasicTessellationDemo::Update(const Time& time)
{
	if (!!m_keyboard)
	{
		if (m_keyboard->WasKeyPressed(Key::Space))
		{
			m_isUniformTessellation = !m_isUniformTessellation;
		}

		if (m_keyboard->WasKeyPressed(Key::T))
		{
			m_topology = static_cast<TopologyType>((m_topology + 1) % TopologyType::Count);
		}

		if (m_isUniformTessellation)
		{
			if (m_keyboard->WasKeyPressed(Key::Up))
			{
				const auto tessellationFactor = ClampTessellation(m_tessEdgeFactors.quad[0] + 1.f);
				m_tessEdgeFactors.quad.fill(tessellationFactor);
				m_tessInsideFactors.quad.fill(tessellationFactor);
			}

			if (m_keyboard->WasKeyPressed(Key::Down))
			{
				const auto tessellationFactor = ClampTessellation(m_tessEdgeFactors.quad[0] - 1.f);
				m_tessEdgeFactors.quad.fill(tessellationFactor);
				m_tessInsideFactors.quad.fill(tessellationFactor);
			}
		}
		else
		{
			//-------------------------------------------------------------------------
			// Edge Factors
			//-------------------------------------------------------------------------

			if (m_keyboard->WasKeyPressed(Key::U))
				m_tessEdgeFactors.triangle[0] = ClampTessellation(m_tessEdgeFactors.triangle[0] + 1.f);
			if (m_keyboard->WasKeyPressed(Key::H))
				m_tessEdgeFactors.triangle[0] = ClampTessellation(m_tessEdgeFactors.triangle[0] - 1.f);

			if (m_keyboard->WasKeyPressed(Key::I))
				m_tessEdgeFactors.triangle[1] = ClampTessellation(m_tessEdgeFactors.triangle[1] + 1.f);
			if (m_keyboard->WasKeyPressed(Key::J))
				m_tessEdgeFactors.triangle[1] = ClampTessellation(m_tessEdgeFactors.triangle[1] - 1.f);

			if (m_keyboard->WasKeyPressed(Key::O))
				m_tessEdgeFactors.triangle[2] = ClampTessellation(m_tessEdgeFactors.triangle[2] + 1.f);
			if (m_keyboard->WasKeyPressed(Key::K))
				m_tessEdgeFactors.triangle[2] = ClampTessellation(m_tessEdgeFactors.triangle[2] - 1.f);

			if (m_topology == TopologyType::Quad && m_keyboard->WasKeyPressed(Key::P))
				m_tessEdgeFactors.quad[3] = ClampTessellation(m_tessEdgeFactors.quad[3] + 1.f);
			if (m_topology == TopologyType::Quad && m_keyboard->WasKeyPressed(Key::L))
				m_tessEdgeFactors.quad[3] = ClampTessellation(m_tessEdgeFactors.quad[3] - 1.f);

			//-------------------------------------------------------------------------
			// Inside Factors
			//-------------------------------------------------------------------------

			if (m_keyboard->WasKeyPressed(Key::V))
				m_tessInsideFactors.triangle = ClampTessellation(m_tessInsideFactors.triangle + 1.f);
			if (m_keyboard->WasKeyPressed(Key::B))
				m_tessInsideFactors.triangle = ClampTessellation(m_tessInsideFactors.triangle - 1.f);

			if (m_topology == TopologyType::Quad && m_keyboard->WasKeyPressed(Key::N))
				m_tessInsideFactors.quad[1] = ClampTessellation(m_tessInsideFactors.quad[1] + 1.f);
			if (m_topology == TopologyType::Quad && m_keyboard->WasKeyPressed(Key::M))
				m_tessInsideFactors.quad[1] = ClampTessellation(m_tessInsideFactors.quad[1] - 1.f);
		}
	}

	m_text->Update(time);

	PrimitiveComponent::Update(time);
}

//-------------------------------------------------------------------------

void BasicTessellationDemo::Draw(const library::Time& time)
{
	const auto& primitiveData = m_primitivesData[m_topology];
	Draw_SetIA(primitiveData);
	Draw_SetData(primitiveData);
	Draw_Render(primitiveData);
}

void BasicTessellationDemo::Draw_SetData(const PrimitiveData& primitiveData)
{
	auto wvp = GetWorldMatrix();
	if (auto camera = GetCamera())
		wvp *= camera->GetViewProjectionMatrix();
	
	auto material = GetMaterial();
	material->GetWVP() << wvp;

	switch (m_topology)
	{
		case TopologyType::Triangle:
			material->GetTessellationEdgeFactors() << m_tessEdgeFactors.triangle;
			material->GetTessellationInsideFactors() << m_tessInsideFactors.triangle;
			break;

		case TopologyType::Quad:
			material->GetTessellationEdgeFactors() << m_tessEdgeFactors.quad;
			material->GetTessellationInsideFactors() << m_tessInsideFactors.quad;
			break;

		default:
			break;
	}

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}

void BasicTessellationDemo::Draw_Render(const PrimitiveData& primitiveData)
{
	auto deviceContext = GetApp().GetDeviceContext();
	deviceContext->RSSetState(RasterizerStates::Wireframe);

	PrimitiveComponent::Draw_Render(primitiveData);
	GetApp().GetDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	GetApp().GetDeviceContext()->DSSetShader(nullptr, nullptr, 0);

	deviceContext->RSSetState(nullptr);
}

//-------------------------------------------------------------------------

std::wstring BasicTessellationDemo::DumpEdgeFactors() const
{
	std::wostringstream woss;

	woss << "[";

	if (m_topology == TopologyType::Triangle)
	{
		woss <<
			m_tessEdgeFactors.triangle[0] << ", " <<
			m_tessEdgeFactors.triangle[1] << ", " <<
			m_tessEdgeFactors.triangle[2];
	}
	else
	{
		woss <<
			m_tessEdgeFactors.quad[0] << ", " <<
			m_tessEdgeFactors.quad[1] << ", " <<
			m_tessEdgeFactors.quad[2] << ", " <<
			m_tessEdgeFactors.quad[3];
	}

	woss << "]";

	return woss.str();
}

std::wstring BasicTessellationDemo::DumpInsideFactors() const
{
	std::wostringstream woss;

	woss << "[";

	if (m_topology == TopologyType::Triangle)
		woss << m_tessInsideFactors.triangle;
	else
		woss << m_tessInsideFactors.quad[0] << ", " << m_tessInsideFactors.quad[1];

	woss << "]";

	return woss.str();
}
