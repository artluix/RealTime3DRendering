#include "StdAfx.h"
#include "library/Components/ProxyModelComponent.h"

#include "library/Components/CameraComponent.h"

#include "library/Effect/Effect.h"
#include "library/Effect/EffectVariable.h"

#include "library/Application.h"
#include "library/RasterizerStates.h"

#include "library/Model/Model.h"

namespace library
{
ProxyModelComponent::ProxyModelComponent(const std::string& modelName, const float scale)
	: m_modelName(modelName)
{
	SetScaling(math::Vector3(scale));
}

ProxyModelComponent::~ProxyModelComponent() = default;

//-------------------------------------------------------------------------

void ProxyModelComponent::SetWireframeVisible(const bool visible)
{
	m_isWireframeVisible = visible;
}

void ProxyModelComponent::Initialize()
{
	InitializeMaterial("Basic");

	const auto& app = GetApp();

	Model model(app, m_modelName, true);
	m_primitivesData = m_material->CreatePrimitivesData(app.GetDevice(), model);
}

void ProxyModelComponent::Draw_SetData(const PrimitiveData& primitiveData)
{
	auto wvp = GetWorldMatrix();
	if (auto camera = GetCamera())
		wvp *= camera->GetViewProjectionMatrix();
	m_material->GetWorldViewProjection() << math::XMMatrix(wvp);

	PrimitiveComponent::Draw_SetData(primitiveData);
}

void ProxyModelComponent::Draw_Render(const PrimitiveData& primitiveData)
{
	auto deviceContext = GetApp().GetDeviceContext();

	if (m_isWireframeVisible)
	{
		deviceContext->RSSetState(RasterizerStates::Wireframe);
		PrimitiveComponent::Draw_Render(primitiveData);
		deviceContext->RSSetState(nullptr);
	}
	else
	{
		PrimitiveComponent::Draw_Render(primitiveData);
	}
}
} // namespace library
