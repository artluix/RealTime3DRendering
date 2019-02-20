#include "StdAfx.h"
#include "library/components/ProxyModelComponent.h"

#include "library/components/CameraComponent.h"

#include "library/effect/Effect.h"
#include "library/effect/EffectVariable.h"

#include "library/Path.h"
#include "library/Utils.h"
#include "library/Application.h"
#include "library/RasterizerStateHolder.h"

namespace library
{
	ProxyModelComponent::ProxyModelComponent(const std::string& modelName, const float scale)
		: m_direction(math::Vector3::Forward)
		, m_up(math::Vector3::Up)
		, m_right(math::Vector3::Right)
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
		MaterialSceneComponent::Initialize(app);
	}

	void ProxyModelComponent::Draw_SetData()
	{
		auto wvp = GetWorldMatrix();
		if (auto camera = GetCamera())
			wvp *= camera->GetViewProjectionMatrix();
		m_material->GetWorldViewProjection() << wvp;

		MaterialSceneComponent::Draw_SetData();
	}

	void ProxyModelComponent::Draw_Render()
	{
		auto deviceContext = m_app->GetDeviceContext();

		if (m_isWireframeVisible)
		{
			deviceContext->RSSetState(RasterizerStateHolder::GetRasterizerState(RasterizerState::Wireframe).Get());
			MaterialSceneComponent::Draw_Render();
			deviceContext->RSSetState(nullptr);
		}
		else
		{
			MaterialSceneComponent::Draw_Render();
		}
	}
} // namespace library
