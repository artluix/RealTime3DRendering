#include "StdAfx.h"
#include "library/components/ProxyModelComponent.h"

#include "library/components/CameraComponent.h"

#include "library/effect/Effect.h"
#include "library/effect/EffectVariable.h"
#include "library/effect/EffectFactory.h"

#include "library/Path.h"
#include "library/Utils.h"
#include "library/Application.h"
#include "library/RasterizerStatesStorage.h"

namespace library
{
	namespace
	{
		const auto k_effectPath = utils::GetExecutableDirectory().Join(
#if defined(DEBUG) || defined(DEBUG)
			fs::Path("../data/effects/BasicEffect_d.fxc")
#else
			fs::Path("../data/effects/BasicEffect.fxc")
#endif
		);
	}

	ProxyModelComponent::ProxyModelComponent(const Application& app, const fs::Path& modelPath, const float scale)
		: SceneComponent()
		, ConcreteMaterialComponent(app, modelPath)
		, m_direction(math::constants::Vector3::Forward)
		, m_up(math::constants::Vector3::Up)
		, m_right(math::constants::Vector3::Right)
	{
		SetScaling(math::Vector3(scale));
	}

	ProxyModelComponent::~ProxyModelComponent() = default;

	void ProxyModelComponent::SetWireframeVisible(const bool visible)
	{
		m_isWireframeVisible = visible;
	}

	void ProxyModelComponent::Initialize()
	{
		m_effect = EffectFactory::Create(m_app, k_effectPath);
		m_effect->LoadCompiled();

		m_material = std::make_unique<BasicMaterial>(*m_effect);
		m_material->Initialize();

		MaterialComponent::Initialize();
	}

	void ProxyModelComponent::Update(const Time& time)
	{
	}

	void ProxyModelComponent::SetEffectData()
	{
		auto wvp = m_worldMatrix;
		if (!!m_camera)
			wvp *= m_camera->GetViewProjectionMatrix();

		MaterialComponent::SetEffectData();
	}

	void ProxyModelComponent::Render()
	{
		auto deviceContext = m_app.GetD3DDeviceContext();

		if (m_isWireframeVisible)
		{
			deviceContext->RSSetState(RasterizerStateContainer::GetRasterizerState(RasterizerState::Wireframe));
			deviceContext->DrawIndexed(m_indicesCount, 0, 0);
			deviceContext->RSSetState(nullptr);
		}
		else
		{
			deviceContext->DrawIndexed(m_indicesCount, 0, 0);
		}
	}

} // namespace library
