#include "StdAfx.h"
#include "library/Components/ProxyModelComponent.h"

#include "library/Components/CameraComponent.h"

#include "library/Effect/Effect.h"
#include "library/Effect/EffectVariable.h"

#include "library/Application.h"
#include "library/RasterizerStates.h"

namespace library
{
	ProxyModelComponent::ProxyModelComponent(const std::string& modelName, const float scale)
	{
		SetModelName(modelName);
		SetScaling(math::Vector3(scale));
	}

	ProxyModelComponent::~ProxyModelComponent() = default;

	//-------------------------------------------------------------------------

	void ProxyModelComponent::SetWireframeVisible(const bool visible)
	{
		m_isWireframeVisible = visible;
	}

	void ProxyModelComponent::Initialize(const Application& app)
	{
		InitializeMaterial(app, "Basic");
		SceneComponent::Initialize(app);
	}

	void ProxyModelComponent::Draw_SetData()
	{
		auto wvp = GetWorldMatrix();
		if (!!m_camera)
			wvp *= m_camera->GetViewProjectionMatrix();
		m_material->GetWorldViewProjection() << wvp;

		SceneComponent::Draw_SetData();
	}

	void ProxyModelComponent::Draw_Render()
	{
		auto deviceContext = GetApp()->GetDeviceContext();

		if (m_isWireframeVisible)
		{
			deviceContext->RSSetState(RasterizerStates::Wireframe);
			SceneComponent::Draw_Render();
			deviceContext->RSSetState(nullptr);
		}
		else
		{
			SceneComponent::Draw_Render();
		}
	}
} // namespace library
