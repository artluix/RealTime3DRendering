#include "StdAfx.h"
#include "library/Components/ProxyModelComponent.h"

#include "library/Components/CameraComponent.h"

#include "library/Effect/Effect.h"
#include "library/Effect/EffectVariable.h"

#include "library/Application.h"
#include "library/Render/RasterizerStates.h"

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

void ProxyModelComponent::InitializeInternal()
{
	CreateMaterialWithEffect("Basic");

	Model model(GetApp(), m_modelName, true);
	CreatePrimitivesData(model);
}

void ProxyModelComponent::Draw_SetData(const PrimitiveData& primitiveData)
{
	auto wvp = GetWorldMatrix();
	if (auto camera = GetCamera())
		wvp *= camera->GetViewProjectionMatrix();

	m_material->GetWorldViewProjection() << wvp;

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}

void ProxyModelComponent::Draw_Render(const PrimitiveData& primitiveData)
{
	auto deviceContext = GetApp().GetDeviceContext();

	if (m_wireframeVisible)
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
