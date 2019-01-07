#include "StdAfx.h"
#include "library/components/ProxyModelComponent.h"

namespace library
{
	ProxyModelComponent::ProxyModelComponent(const Application& app, const CameraComponent& camera, const fs::Path& modelPath, const float scale)
		: SceneComponent(app, camera)
		, m_modelPath(modelPath)
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

	}

	void ProxyModelComponent::Update(const Time& time)
	{

	}

	void ProxyModelComponent::Draw(const Time& time)
	{

	}

} // namespace library
