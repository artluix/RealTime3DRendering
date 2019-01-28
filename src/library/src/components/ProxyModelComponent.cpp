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
	namespace
	{
		const auto k_effectPath = utils::GetExecutableDirectory().Join(
#if defined(DEBUG) || defined(DEBUG)
			Path("../data/effects/Basic_d.fxc")
#else
			Path("../data/effects/Basic.fxc")
#endif
		);
	}

	ProxyModelComponent::ProxyModelComponent(const Path& modelPath, const float scale)
		: m_direction(math::Vector3::Forward)
		, m_up(math::Vector3::Up)
		, m_right(math::Vector3::Right)
	{
		SetModelPath(modelPath);
		SetScaling(math::Vector3(scale));
	}

	ProxyModelComponent::~ProxyModelComponent() = default;

	void ProxyModelComponent::SetWireframeVisible(const bool visible)
	{
		m_isWireframeVisible = visible;
	}

	void ProxyModelComponent::Initialize(const Application& app)
	{
		InitializeMaterial(app, k_effectPath);
		MaterialComponent::Initialize(app);
	}

	void ProxyModelComponent::SetEffectData()
	{
		auto wvp = GetWorldMatrix();
		if (auto camera = GetCamera())
			wvp *= camera->GetViewProjectionMatrix();
		m_material->GetWorldViewProjection() << wvp;

		MaterialComponent::SetEffectData();
	}

	void ProxyModelComponent::Render()
	{
		auto deviceContext = m_app->GetDeviceContext();

		if (m_isWireframeVisible)
		{
			deviceContext->RSSetState(RasterizerStateHolder::GetRasterizerState(RasterizerState::Wireframe).Get());
			deviceContext->DrawIndexed(m_indicesCount, 0, 0);
			deviceContext->RSSetState(nullptr);
		}
		else
		{
			deviceContext->DrawIndexed(m_indicesCount, 0, 0);
		}
	}
} // namespace library
